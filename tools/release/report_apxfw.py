#!/usr/bin/python

import argparse
import base64
import difflib
import os
import zlib
from distutils.version import StrictVersion
from glob import glob

import simplejson
from colorama import Fore, Style
from prettytable import PrettyTable

import release_utils as utils

# Parse commandline
parser = argparse.ArgumentParser(description='Report APXFW firmware changes.')
parser.add_argument('--ref', action='store', help='folder to search for reference apxfw files, will use standard path by default')
parser.add_argument('--src', action='store', required=False, help='folder or apxfw/zip file to compare to')
parser.add_argument('--colored', action='store', default=True, help='colored output')
args = parser.parse_args()


def colored_out(s, color):
    if args.colored:
        return color+s+Style.RESET_ALL
    return s


def colored_value(s, value):
    if args.colored:
        return colored_out(s, Fore.LIGHTGREEN_EX if value <= 0 else Fore.YELLOW)
    return s


def humanbytes(B):
    'Return the given bytes as a human friendly KB, MB, GB, or TB string'
    B = float(B)
    KB = float(1024)
    MB = float(KB ** 2)  # 1,048,576
    GB = float(KB ** 3)  # 1,073,741,824
    TB = float(KB ** 4)  # 1,099,511,627,776
    if B < KB:
        return '{0} {1}'.format(int(B), 'Bytes' if B > 1 or B == 0 else 'Byte')
    elif KB <= B < MB:
        return '{0:.2f} KB'.format(B/KB)
    elif MB <= B < GB:
        return '{0:.2f} MB'.format(B/MB)
    elif GB <= B < TB:
        return '{0:.2f} GB'.format(B/GB)
    elif TB <= B:
        return '{0:.2f} TB'.format(B/TB)


class DatasetsCompare(dict):
    def __init__(self, name, prefix, ref_json, src_json):
        dict.__init__(self, name=name, prefix=prefix)

        self.valid = False
        self.size = 0
        self.diff = 0
        self.ratio = 0
        self.status = 'NONE'

        if name not in ref_json or name not in src_json:
            self.status = ('no '+name).upper()
            return
        self.valid = True
        self.size = int(src_json[name]['size'])
        self.diff = int(ref_json[name]['size'])-self.size
        rep = list()
        if self.diff != 0:
            rep.append(colored_value('{:+1}'.format(self.diff), self.diff))
        s = difflib.SequenceMatcher(None, self.get_data(ref_json[name]['data']), self.get_data(src_json[name]['data']))
        self.ratio = int(s.quick_ratio()*100)
        if self.ratio != 100:
            rep.append(' ({0}%)'.format(self.ratio))
        if len(rep) == 0:
            self.status = ''
        else:
            rep.insert(0, prefix)
            self.status = ''.join(rep)

    def get_data(self, base64data):
        return zlib.decompress(base64.b64decode(base64data))

    def report_size(self):
        if self.size == 0:
            return ''
        return humanbytes(self.size)


class FilesCompare(dict):
    def __init__(self, ref, src):
        dict.__init__(self, ref=ref, src=src)

        with open(ref, 'r') as f:
            ref_json = simplejson.loads(str(f.read()))
        with open(src, 'r') as f:
            src_json = simplejson.loads(str(f.read()))
        assert ref_json['node'] == src_json['node']
        assert ref_json['hardware'] == src_json['hardware']
        self.node = ref_json['node']
        self.hw = ref_json['hardware']
        self.ref_ver = ref_json['version']
        self.src_ver = src_json['version']
        self.firmware = DatasetsCompare('firmware', 'fw', src_json, ref_json)
        self.loader = DatasetsCompare('loader', 'ld', src_json, ref_json)

    def reports_list(self):
        reports = list()
        reports.append('{0}.{1}'.format(self.node, self.hw))
        reports.append(self.firmware.report_size())
        reports.append(self.loader.report_size())
        reports.append(self.firmware.status)
        reports.append(self.loader.status)
        reports.append('{0}->{1}'.format(self.ref_ver, self.src_ver))
        return reports

    def print_report(self):
        rlist = self.reports_list()
        row_format = '{:<15} ' * len(rlist)
        print(row_format.format(*rlist))


class DirsCompare(list):
    def __init__(self, ref_dir, src_dir, do_report=False):
        list.__init__(self)

        self.missing = list()

        for r in sorted(glob(os.path.join(ref_dir, '*.apxfw'))):
            # node, hw, ver = os.path.splitext(os.path.basename(r))[0].split('-')
            with open(r, 'r') as f:
                json = simplejson.loads(str(f.read()))

            node = json['node']
            hw = json['hardware']

            # find max version to compare
            versions = list()
            for f in glob(os.path.join(src_dir, '{0}-{1}-*.apxfw'.format(node, hw))):
                with open(f, 'r') as f:
                    cjson = simplejson.loads(str(f.read()))
                versions.append(cjson['version'])
            if len(versions) < 1:
                self.missing.append('{0}.{1}'.format(node, hw))
                continue
            versions = list(set(versions))  # remove duplicates
            versions.sort(key=StrictVersion)
            src = os.path.join(src_dir, '{0}-{1}-{2}.apxfw'.format(node, hw, versions[-1]))
            c = FilesCompare(r, src)
            self.append(c)
            if do_report:
                c.print_report()

        if do_report:
            self.print_missing()
            self.print_diff()

    def print_report(self):
        t = PrettyTable(['node', 'fw size', 'ld size', 'firmware', 'loader', 'version'])
        for c in self:
            t.add_row(c.reports_list())
        print t
        self.print_missing()
        self.print_diff()

    def print_missing(self):
        if len(self.missing) > 0:
            print('Missing: ({}) '.format(len(self.missing))+','.join(self.missing))

    def print_diff(self):
        diff = 0
        ratio_min = 100
        for c in self:
            diff = diff+c.firmware.diff+c.loader.diff
            ratio_min = min(ratio_min, c.firmware.ratio, c.loader.ratio)
        sign = '+' if diff >= 0 else '-'
        print('Size difference: {}'.format(colored_value(sign+humanbytes(diff), diff)))
        print('Similarity: {}%'.format(ratio_min))


#########################
if args.src:
    src_dir = args.src
else:
    src_dir = '/Users/uavinda/Documents/UAVOS/Firmware/development'
    # src_dir = '/Volumes/Projects/Autopilot/apx/build-fw-gcc_arm_none_eabi-Release/Release/install-root'
print('Comparing firmware: '+src_dir)

ref = args.ref

if not ref:
    path = os.path.join(os.path.expanduser('~'), '**', 'UAVOS/Firmware')
    for d in glob(os.path.join(path, 'APX_Nodes_Firmware-*')):
        if not os.path.isdir(d):
            continue
        version = utils.asset_version(d)
        if not version:
            continue
        ref = d
        break

assert os.path.exists(ref)
assert os.path.isdir(ref)
print('Refrence: '+ref)

version = utils.basename_version(os.path.split(ref)[-1])
if version:
    print('Reference version: '+version)

dc = DirsCompare(ref, src_dir, False)
dc.print_report()
