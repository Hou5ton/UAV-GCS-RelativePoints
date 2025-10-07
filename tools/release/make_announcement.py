#!/usr/bin/python

import argparse

from announcement import Letter
from changelog.changelog import Changelog

# Parse commandline
parser = argparse.ArgumentParser(description='Release announcement for the APX system.')
parser.add_argument('--remote', action='store', help='github repository name')
parser.add_argument('--token', action='store', help='GitHub personal access token <user>:<token>')
args = parser.parse_args()


#########################
ch = Changelog(args.remote, args.token)

ch.update()
if not ch.published:
    print('Release not published')
    exit(1)

# announcement letter
letter = Letter(ch.version, ch.branch, ch.changes)

# letter.send(10206673)  # test
letter.send(2271)  # subscribers from docs.uavos.com
