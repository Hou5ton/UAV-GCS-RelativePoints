#!/usr/bin/python

import os


def basename_split(f):
    return f.split('-')


def basename_version(f):
    a_f = basename_split(f)
    if len(a_f) >= 2:
        return a_f[1]
    return None


def asset_version(f):
    f = os.path.basename(f)
    f = f[:f.rfind('.')]
    return basename_version(f)
