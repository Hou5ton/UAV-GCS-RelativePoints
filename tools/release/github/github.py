#!/usr/bin/python
#
# APX release generator for github
#

import os
import github_release as ghr
from time import sleep


class GithubRelease:
    def __init__(self, remote, token, version, name):
        self.remote = remote
        ghr._github_token_cli_arg = token
        self.version = version
        self.name = name

        self.release = ghr.get_release(repo_name=remote, tag_name=version)
        if self.release is None:
            print('Creating GitHub release...')
            self.release = ghr.gh_release_create(repo_name=remote, tag_name=version, name=name + ' v' + version)
        else:
            print('Updating GitHub release...')

    def upload(self, asset):
        if not os.path.exists(asset):
            print('Asset file not exists: ' + asset)
            exit(1)
        print('Uploading asset: ' + asset)

        retry = 0
        while retry < 5:
            sleep(1)
            try:
                ghr.gh_asset_upload(repo_name=self.remote, tag_name=self.version, pattern=[asset], verbose=True)
            except:
                retry += 1
                continue
            break
        assert retry < 5

    def asset_download(self, asset):
        return ghr.gh_asset_download(repo_name=self.remote, tag_name=self.version, pattern=asset)

    def asset_delete(self, asset):
        return ghr.gh_asset_delete(repo_name=self.remote, tag_name=self.version, pattern=asset)

    def publish(self):
        print('Publishing GitHub release...')
        ghr.gh_release_edit(repo_name=self.remote, current_tag_name=self.version, draft=False)

    def update(self, **kwargs):
        ghr.gh_release_edit(repo_name=self.remote, current_tag_name=self.version, **kwargs)
