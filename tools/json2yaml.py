#!/usr/bin/env python

import simplejson
import yaml
import os

import argparse

# Parse commandline
parser = argparse.ArgumentParser(description='Convert JSON to YAML')
parser.add_argument('file', action='store', help='json file path')
parser.add_argument('-p', action='store_true', help='print start and end marks')
args = parser.parse_args()

print('Converting: ' + args.file)

with open(args.file, 'r') as f:
    yml=yaml.dump(simplejson.loads(str(f.read())), default_flow_style=False, explicit_start=args.p, explicit_end=args.p)
    f.close()
    with open(os.path.splitext(args.file)[0]+'.yml', 'w') as f2:
        f2.write(yml)
