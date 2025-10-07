#!/usr/bin/python

import argparse
import os
import subprocess
import os.path
import stat
import sys

import simplejson
import urllib.request

if __name__ == "__main__":
    # Parse commandline
    parser = argparse.ArgumentParser(description='Deploy linux AppImage for APX app.')
    parser.add_argument('--appdata', action='store', required=True, help='APX app metadata json file')
    parser.add_argument('--apprun', action='store', help='AppImage AppRun script')
    args = parser.parse_args()
    with open(args.appdata, 'r') as f:
        json = simplejson.loads(str(f.read()))
        f.close()
    path = os.path.dirname(args.appdata)
    app = json['app']
    app_name = app['name']
    app_path = os.path.abspath(os.path.join(path, app['bundle_path'], '..'))
    packages_path = os.path.join(path, app['packages_path'])
    bundle = os.path.basename(app_path)
    platform = app['platform']

    filename = os.path.join(app_name.replace(' ', '_')+'-'+app['version']+'-'+platform+'-'+app['arch']+'.AppImage')
    volume_name = app_name+' ('+app['version']+')'

    print('Deploy image ({})...'.format(filename))

    apprun = args.apprun
    if apprun:
        dest = os.path.join(app_path, 'AppRun')
        if os.path.exists(dest):
            os.remove(dest)
        subprocess.check_call(['cp', '-af', apprun, dest])
        subprocess.check_call(['chmod', '+x', dest])

    pargs = [
        'env', 'ARCH=x86_64',
        os.path.join(app['qt_bin'], 'appimagetool'),
        '--verbose',
        app_path,
        os.path.join(packages_path, filename)
    ]
    subprocess.check_call(pargs, cwd=packages_path)

    print('Application image created.')
