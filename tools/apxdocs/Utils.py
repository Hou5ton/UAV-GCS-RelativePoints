import os
import fnmatch
import shutil
import re
import yaml


def files(path):
    for f in os.listdir(path):
        if os.path.isfile(os.path.join(path, f)):
            yield os.path.abspath(os.path.join(path, f))


def find_files(directory, pattern):
    for root, dirs, files in os.walk(directory):
        for basename in files:
            if fnmatch.fnmatch(basename, pattern):
                filename = os.path.abspath(os.path.join(root, basename))
                yield filename


def dirs(path):
    for f in os.listdir(path):
        if os.path.isdir(os.path.join(path, f)):
            yield os.path.abspath(os.path.join(path, f))


def copytree(src, dst, symlinks=False, ignore=None):
    for item in os.listdir(src):
        s = os.path.join(src, item)
        d = os.path.join(dst, item)
        if os.path.isdir(s):
            shutil.copytree(s, d, symlinks, ignore)
        else:
            shutil.copy2(s, d)


FM_BOUNDARY = re.compile(r'^-{3,}$', re.MULTILINE)


def parse_frontmatter(text):
    if not text.startswith('---\n'):
        return {}, text
    # Parse text with frontmatter, extract metadata and content.
    try:
        _, fm, content = FM_BOUNDARY.split(text, 2)
    except ValueError:
        print 'YAML error:', text.splitlines()[0]
        return {}, text
    return yaml.load(fm, Loader=yaml.SafeLoader), content.strip()
