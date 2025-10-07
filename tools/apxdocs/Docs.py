#!/usr/bin/python
#
# APX docs generator helpers
#

import os
import shutil
import yaml
from distutils.dir_util import copy_tree, mkpath
from distutils.file_util import move_file, copy_file

from Nodes import Node, Nodes
import Utils


class DocsFiles:
    def __init__(self, root, dest):
        self.root = root
        self.dest = dest

        self.docs_dest = os.path.join(self.dest, '_docs')
        self.data_dest = os.path.join(self.dest, '_data')

    def init_dest(self, copy_config):
        print 'Updating destination: \'{}\'...'.format(self.dest)
        if not os.path.exists(self.dest):
            os.makedirs(self.dest)
        docs_src = os.path.join(self.root, 'docs')

        # copy pages and config
        if os.path.exists(self.docs_dest):
            shutil.rmtree(self.docs_dest)
        copy_tree(os.path.join(docs_src, 'pages'), self.docs_dest)

        # move yml files to _data
        if os.path.exists(self.data_dest):
            shutil.rmtree(self.data_dest)
        if not os.path.exists(self.data_dest):
            os.makedirs(self.data_dest)
        for f in Utils.find_files(self.docs_dest, '*.yml'):
            move_file(f, self.data_dest)

        # copy configs
        if copy_config:
            for f in Utils.files(docs_src):
                copy_file(f, self.dest)


class Docs(DocsFiles):
    def __init__(self, root, dest, copy_config=True):
        DocsFiles.__init__(self, root, dest)

        self.nodes = Nodes(root)

        self.init_dest(copy_config)

        # create hw/nodes articles about nodes
        nodes_dest = os.path.join(self.docs_dest, 'hw', 'nodes')
        mkpath(nodes_dest)
        pages = list()
        pages_order = list()
        for node in self.nodes.nodes.itervalues():
            with open(os.path.join(nodes_dest, node.name + '.md'), 'w') as f:
                f.write('---\n')
                yaml.safe_dump(node.frontmatter, f, default_flow_style=False, encoding='utf-8', allow_unicode=False)
                f.write('---\n\n')
                f.write(node.text)
            pages.append('nodes/' + node.name)
            pages_order.append(node.order)
            # copy assets
            for a in node.assets:
                dest = os.path.join(nodes_dest, os.path.basename(a))
                mkpath(dest)
                Utils.copytree(a, dest)

        if pages:
            pages = [x for _, x in sorted(zip(pages_order, pages))]

            with open(os.path.join(self.data_dest, 'hw.yml'), "r") as f:
                yml = yaml.load(f, Loader=yaml.Loader)
            if 'docs' not in yml:
                yml['docs'] = [{'pages': pages}]
            else:
                yml['docs'][0]['pages'].extend(pages)
            with open(os.path.join(self.data_dest, 'hw.yml'), "w") as f:
                yaml.safe_dump(yml, f, default_flow_style=False, encoding='utf-8', allow_unicode=False)

        # create /sw/conf/ files for modules
        conf_dest = os.path.join(self.docs_dest, 'fw', 'conf')
        mkpath(conf_dest)
        pages = list()
        for a in sorted(self.nodes.articles.list, key=lambda sa: sa.title):
            if not a.label:
                print 'No label:', a
                continue
            if not a.title:
                print 'No title:', a.label
                continue
            with open(os.path.join(conf_dest, a.label + '.md'), "w") as f:
                f.write('---\n')
                f.write(a.font_matter)
                f.write('---\n\n')
                f.write('>Configuration: `{}`\n'.format(a.conf))
                if a.nodes:
                    s = map(lambda x: '[`{0}`](../../hw/nodes/{1}.md)'.format(x.name, x.name), a.nodes)
                    f.write('> | Nodes: {}\n'.format(' '.join(s)))
                f.write('\n')

                f.write(a.text)
            pages.append('conf/' + a.label)

        if pages:
            with open(os.path.join(self.data_dest, 'fw.yml'), "r") as f:
                yml = yaml.load(f, Loader=yaml.Loader)
            # yml['docs'].append({
            #     'pages': pages,
            #     'title': 'Modules'
            # })
            if 'docs' not in yml:
                yml['docs'] = [{'pages': pages}]
            else:
                yml['docs'][0]['pages'].extend(pages)
            with open(os.path.join(self.data_dest, 'fw.yml'), "w") as f:
                yaml.safe_dump(yml, f, default_flow_style=False, encoding='utf-8', allow_unicode=False)

