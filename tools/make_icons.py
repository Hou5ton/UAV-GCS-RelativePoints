#!/usr/bin/python
#
# APX css parser for material design icons css
#


import os
import argparse
import cssutils
import simplejson




# Parse commandline
parser = argparse.ArgumentParser(description='Material Design Icons CSS parser for the APX system.')
parser.add_argument('--src', action='store', required=True, help='source CSS file path')
parser.add_argument('--dest', action='store', required=True, help='destination JSON file path')
args = parser.parse_args()


print("Parsing CSS...")

css = cssutils.parseFile(args.src)

icons = {}


for rule in css:
    if rule.type == rule.STYLE_RULE:
        style = rule.selectorText
        if not style.startswith('.mdi-'):
            continue
        if not style.endswith('::before'):
            continue
        icon = style[5:][:-8]
        # print icon
        for item in rule.style:
            if item.name == 'content':
                s = item.value.strip('\"')
                if s.startswith('\\F'):
                    continue
                icons[icon] = s.encode('utf8')

with open(args.dest, 'w') as f:
    f.write(simplejson.dumps(icons, indent=2))

print 'Found {} icons.'.format(len(icons))
