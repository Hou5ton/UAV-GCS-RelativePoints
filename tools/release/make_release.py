#!/usr/bin/python
#
# APX release generator
# Creates changelogs of 'uavos/apx' in separate public repository
# Changes are tracked from tag 'apx' with msg containing hash to point to 'apx' commit
#

import argparse
import fnmatch
import os

import release_utils as utils
from changelog.changelog import Changelog
from github.github import GithubRelease

# Parse commandline
parser = argparse.ArgumentParser(description='Release generator for the APX system.')
parser.add_argument('--remote', action='store', help='github repository name')
parser.add_argument('--token', action='store', help='GitHub personal access token <user>:<token>')
parser.add_argument('--name', action='store', help='release name', default='APX')
parser.add_argument('--assets', action='store', help='Release assets folder')
parser.add_argument('--asset', action='append', help='Release asset file path')
parser.add_argument('--publish', action='store_true', help='Publish release (push to remote)')
parser.add_argument('--changes', action='store_true', help='Just generate and show changelog and exit')
args = parser.parse_args()

#########################
ch = Changelog(args.remote, args.token)

if args.changes:
    ch.update()
    print ch.changes
    exit(0)


ghr = GithubRelease(ch.remote, args.token, ch.version, args.name)

# assets to upload and put to commit on publish
artifacts = ['*.xml']


def is_artifact(f):
    for a in artifacts:
        if fnmatch.fnmatch(f, a):
            return True
    return False


# parse assets
assets = args.asset if args.asset else []
if args.assets:
    for f in os.listdir(args.assets):
        if utils.asset_version(f) != ch.version and not is_artifact(f):
            continue
        a = os.path.join(args.assets, f)
        if os.path.isfile(a):
            assets.append(a)

# upload assets
for a in assets:
    ghr.upload(a)

# publish release
if args.publish:
    ch.update()
    # copy artifacts
    for a in ghr.release['assets']:
        f = a['name']
        if not is_artifact(f):
            continue
        if not ghr.asset_download(f):
            continue
        dest = os.path.join(ch.deploy_repo.working_tree_dir, f)
        if os.path.exists(dest):
            os.unlink(dest)
        os.rename(f, dest)
        ghr.asset_delete(f)
        ch.deploy_repo.index.add([dest])
    # commit deploy repo
    ch.publish()
    ghr.update(body=ch.changes)
    ghr.publish()
