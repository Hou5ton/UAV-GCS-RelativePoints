#!/usr/bin/python
#
# APX changelog generator
#

import os

from jinja2 import Environment, FileSystemLoader
from mailjet_rest import Client

__version__ = '1.0.0'


class Letter:
    def __init__(self, version, branch, changes):
        self.version = version
        self.branch = branch
        self.changes = changes.replace('\n* ', '\n * ')

        # Set up the templating engine
        loader = FileSystemLoader(os.path.dirname(os.path.abspath(__file__)))
        env = Environment(loader=loader, trim_blocks=True, lstrip_blocks=True)
        template = env.get_template('letter.jinja2')

        self.text = template.render(data=self).strip().replace('\r', '')

    def send(self, listID):
        print 'Sending announcement email...'
        mj = Client(auth=(os.environ['MJ_APIKEY_PUBLIC'], os.environ['MJ_APIKEY_PRIVATE']))

        # create campaign
        data = {
            'Locale': 'en_US',
            'SenderName': 'UAVOS Inc',
            'Sender': 'Aliaksei Stratsilatau',
            'SenderEmail': 'sa@uavos.com',
            'Subject': 'Update available v{}'.format(self.version),
            'ContactsListID': listID,
            'Title': 'Release {}'.format(self.version)
        }
        result = mj.campaigndraft.create(data=data)
        print result.json()
        assert result.status_code < 400

        j = result.json()
        assert j['Count'] == 1

        dataID = j['Data'][0]['ID']

        # add body text
        data = {
            'Text-part': self.text
        }
        result = mj.campaigndraft_detailcontent.create(id=dataID, data=data)
        print result.json()
        assert result.status_code < 400
        j = result.json()
        assert j['Count'] == 1

        # send campaign
        result = mj.campaigndraft_send.create(id=dataID)
        # print result.json()
        assert result.status_code < 400

    def __repr__(self):
        return self.text
