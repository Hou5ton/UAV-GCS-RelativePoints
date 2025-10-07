import os
from collections import defaultdict
import re
import yaml

import Utils
import glob

from Articles import Articles


class Node:
    def __init__(self, src, root):
        self.src = src
        self.root = root
        self.name = os.path.basename(self.src)

        self.articles = set()

        self.modules = defaultdict(str)    # name, path
        self.assets = set()    # (path, dest)

        self.frontmatter = {}
        self.text = ''

        self.order = 999999999

        default_title = self.name.upper() + ' node'

        # read markdown file
        md = os.path.join(src, self.name + '.md')
        if not os.path.exists(md):
            print 'Missing node:', md
        else:
            with open(md, 'r') as f:
                self.frontmatter, self.text = Utils.parse_frontmatter(f.read().strip())
            if 'title' not in self.frontmatter:
                print 'Invalid front matter:', md

        if 'title' not in self.frontmatter:
            self.frontmatter['title'] = default_title

        self.frontmatter['badge'] = self.name

        if 'order' in self.frontmatter:
            self.order = self.frontmatter['order']

        # find modules
        for qbs in Utils.find_files(self.src, '*.qbs'):
            with open(qbs, 'r') as f:
                for s in re.findall('\[(.*?)\]', f.read().replace('\r', '').replace('\n', ' ')):
                    for sm in s.split(','):
                        self.find_module(sm.replace('\"', ' ').strip())

        # find and parse markdown files
        for f in Utils.find_files(self.src, '*.md'):
            if f.endswith('/{}.md'.format(self.name)): continue
            self.articles.add(Nodes.articles.add_markdown_file(f))

        for m in self.modules:
            for f in Utils.find_files(self.modules[m], '*.md'):
                self.articles.add(Nodes.articles.add_markdown_file(f))

        for f in Utils.find_files(self.src, '*_conf.h'):
            self.articles.update(Nodes.articles.add_src_file(f))

        for m in self.modules:
            for f in Utils.find_files(self.modules[m], '*_conf.h'):
                self.articles.update(Nodes.articles.add_src_file(f))

        # print self.modules.keys()
        # print self.articles
        # print 'Node:', self.name
        # print self.modules.keys()
        # print map(lambda a: a.title, self.articles)

        # update page content
        self.text += '\n'
        with open(os.path.join(src, '../features.md'), 'r') as f:
            self.text += '\n{}\n'.format(f.read().strip())
        # append node articles as references and pinouts
        if self.articles:
            self.text += '\n## Capabilities\n\n';
            for a in sorted(self.articles, key=lambda sa: sa.title):
                self.text += '- [{0}](../../fw/conf/{1}.md)\n'.format(a.title, a.label)

        # Pinouts
        pinouts = defaultdict(str)
        pdir = os.path.join(src, 'pinouts')
        if os.path.exists(pdir):
            for f in os.listdir(pdir):
                if not os.path.isfile(os.path.join(pdir, f)): continue
                pinouts[(os.path.splitext(f)[0])] = f
            if pinouts:
                self.text += '\n## PCB Pinouts\n\n';
                for p in sorted(pinouts.keys()):
                    self.text += '- [{0}](pinouts/{1})\n'.format(p, pinouts[p])

        # assets
        asset = os.path.join(src, 'pinouts')
        if os.path.exists(asset):
            self.assets.add(asset)
        asset = os.path.join(src, 'img')
        if os.path.exists(asset):
            self.assets.add(asset)
        self.text = self.text.strip() + '\n'

    def __str__(self):
        return self.name

    def find_module(self, s):
        s = unicode(s.strip())
        if not s: return
        for c in '*? .':
            if c in s: return
        if s in self.modules:
            return

        mlist = glob.glob(os.path.join(self.root, 'fw/src/modules/*', s, '*.yml'))
        mlist.extend(glob.glob(os.path.join(self.root, 'fw/src/modules', s, '*.yml')))
        mlist.extend(glob.glob(os.path.join(self.root, 'fw/src/drivers/*', s, '*.yml')))
        mlist.extend(glob.glob(os.path.join(self.root, 'fw/src/drivers', s, '*.yml')))
        for f in mlist:
            path = os.path.dirname(f)
            self.modules[s] = path
            # check depends
            for j in Utils.find_files(path, '*.yml'):
                with open(j, 'r') as fm:
                    m = yaml.load(fm.read(), Loader=yaml.Loader)
                    if 'depends' in m and isinstance(m['depends'], list):
                        for dep in m['depends']:
                            self.find_module(dep)


class Nodes:

    articles = Articles()

    def __init__(self, root):
        self.root = root
        self.src = os.path.join(root, 'fw/src/nodes')

        print 'Searching for nodes...'
        self.nodes = defaultdict(Node)

        for node_name in os.listdir(self.src):
            if 'test' in node_name:
                continue
            node_path = os.path.join(self.src, node_name)
            if not os.path.isdir(node_path):
                continue
            if node_name in self.nodes:
                continue
            self.nodes[node_name] = Node(node_path, self.root)

        print 'Collected {0} nodes: {1}'.format(len(self.nodes), self)

        # find nodes for each article
        for a in self.articles.list:
            for node in self.nodes.itervalues():
                if a in node.articles:
                    a.nodes.add(node)

    def __str__(self):
        return ', '.join(self.nodes)
