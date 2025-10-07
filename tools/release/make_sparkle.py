#!/usr/bin/python
import argparse
import os
import xml.etree.ElementTree as ElementTree
from distutils.version import StrictVersion
from glob import glob
from subprocess import call, check_call
from urllib import unquote

import release_utils as utils
from changelog.changelog import Changelog

max_delta = 3

# Parse commandline
parser = argparse.ArgumentParser(description='Sparkle appcast generator for the APX system.')
parser.add_argument('--remote', action='store', help='github repository name')
parser.add_argument('--token', action='store', help='GitHub personal access token <user>:<token>')
parser.add_argument('--assets', action='store', help='Assets folder path with DMG file')
args = parser.parse_args()

#########################
ch = Changelog(args.remote, args.token)

path = args.assets


def clean_packages(path, max_cnt):
    versions = list()
    for f in glob(os.path.join(path, '*.dmg')):
        v = utils.asset_version(f)
        if v:
            versions.append(v)
        else:
            os.remove(os.path.join(path, f))

    for f in glob(os.path.join(path, '*.delta')):
        f = os.path.basename(f)
        a = f[:f.rfind('.')].split('-')
        if len(a) == 3:
            versions.append(a[1])
        else:
            os.remove(os.path.join(path, f))

    versions = list(set(versions))  # remove duplicates
    versions.sort(key=StrictVersion)
    while len(versions) > max_cnt:
        ver = versions.pop(0)
        print ver
        for f in glob(os.path.join(path, '*-{}-*.dmg'.format(ver))):
            os.remove(os.path.join(path, f))
        for f in glob(os.path.join(path, '*-{}-*.delta'.format(ver))):
            os.remove(os.path.join(path, f))
    return versions


# find topmost DMG
version = clean_packages(path, max_delta+1)[-1]
assert version
dmg = glob(os.path.join(path, '*-{}-*.dmg'.format(version)))[0]
assert os.path.exists(dmg)

dmg_f = os.path.basename(dmg)
dmg_f = dmg_f[:dmg_f.rfind('.')]
dmg_f = dmg_f.split('-')
assert len(dmg_f) == 4
dmg_name, dmg_version, dmg_platform, dmg_arch = dmg_f
assert version == dmg_version


# find previous versions
versions = list()
for f in glob(os.path.join(ch.deploy_repo.working_tree_dir, 'notes', 'release-*.md')):
    versions.append(utils.asset_version(f))
versions = list(set(versions))  # remove duplicates
versions.sort(key=StrictVersion)
while len(versions) > max_delta:
    versions.pop(0)

print('Delta: {}'.format(versions))
# download missing dmg old versions
for v in versions:
    asset = '{0}-{1}-{2}-{3}.dmg'.format(dmg_name, v, dmg_platform, dmg_arch)
    if os.path.exists(os.path.join(path, asset)):
        continue
    print('Downloading asset {}...'.format(asset))
    url = 'https://github.com/{2}/releases/download/{0}/{1}'.format(v, asset, ch.remote)
    call(['curl', '--fail', '-L', url, '-o', os.path.join(path, asset)])


# generate appcast.xml
print 'Generating appcast.xml...'
appcast_file = os.path.join(path, 'appcast.xml')
if os.path.exists(appcast_file):
    os.remove(appcast_file)
check_call([os.path.join(os.path.dirname(os.path.abspath(__file__)), 'sparkle', 'generate_appcast'), path])

# fix appcast.xml
ns = 'http://www.andymatuschak.org/xml-namespaces/sparkle'
ElementTree.register_namespace('sparkle', ns)
xml = ElementTree.parse(appcast_file)
for item in xml.getroot().iter('item'):
    e = item.find('enclosure')
    url = e.attrib['url']
    ver = e.attrib['{' + ns + '}version']
    dl_url = 'https://github.com/{1}/releases/download/{0}'.format(ver, ch.remote)
    e.attrib['url'] = dl_url + url[url.rfind('/'):]
    e2 = ElementTree.SubElement(item, 'sparkle:releaseNotesLink')
    ruser, rname = ch.remote.split('/')
    e2.text = 'https://{0}.github.io/{1}/notes/release-{2}.html'.format(ruser, rname, ver)

    deltas = item.find('{' + ns + '}deltas')
    if deltas is None:
        continue
    for e in deltas.iter('enclosure'):
        url = e.attrib['url']
        ver = e.attrib['{' + ns + '}version']
        ver_from = e.attrib['{' + ns + '}deltaFrom']
        dl_url = 'https://github.com/{1}/releases/download/{0}'.format(ver, ch.remote)
        file_name = unquote(url[url.rfind('/') + 1:])
        file_rename = 'update_dmg-{0}-{1}.delta'.format(ver, ver_from)
        e.attrib['url'] = dl_url + '/' + file_rename
        if os.path.exists(os.path.join(path, file_name)):
            os.rename(os.path.join(path, file_name), os.path.join(path, file_rename))
# print xml
xml.write(appcast_file, xml_declaration=True)
