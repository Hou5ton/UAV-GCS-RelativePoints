#!/usr/bin/python
#
# APX docs generator
#

# git subtree push --prefix docs origin gh-pages
# git push origin `git subtree split --prefix docs master`:gh-pages --force


import argparse
import os
import shutil

import apxdocs.Docs as apx


def main():
    print("Updating docs...")

    # Parse commandline
    parser = argparse.ArgumentParser(description='Docs generator for the APX system.')
    parser.add_argument('--root', action='store', help='APX root folder')
    parser.add_argument('--dest', action='store', default='docs', help='destination folder relative to git tree')
    parser.add_argument('--theme', action='store', help='use theme dir as destination folder')
    parser.add_argument('--clean', action='store_true', help='clean destination folder')
    args = parser.parse_args()

    if args.theme:
        dest_dir = args.theme
    elif args.dest.startswith('/'):
        dest_dir = args.dest
    else:
        dest_dir = os.path.join(args.root, args.dest)

    if args.clean and (not args.theme):
        print('Cleaning directory:', dest_dir)
        if os.path.exists(dest_dir):
            shutil.rmtree(dest_dir)

    copy_config = False if args.theme else True

    apx.Docs(args.root, dest_dir, copy_config=copy_config)


if __name__ == "__main__":
    main()
