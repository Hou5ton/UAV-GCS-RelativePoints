import os
import pyparsing
import yaml

import Utils


class Article:

    def __init__(self, label, file_name, text, metadata=None):     # label is module name
        self.file_names = set()
        self.file_names.add(file_name)
        self.text = text.strip()
        self.nodes = set()

        if (not metadata) and self.text:
            metadata, self.text = Utils.parse_frontmatter(text)

        # if not ('label' in metadata):
        #     print 'Missing front matter:', file_name, metadata
        #     return
        # if 'label' in metadata: self.label = metadata['label']

        self.label = metadata['label'] if 'label' in metadata else label
        self.title = metadata['title'].strip() if 'title' in metadata else self.label.capitalize()
        self.conf = metadata['conf'] if 'conf' in metadata else self.title

        fm = {}
        for k in metadata:
            if k == 'label': continue
            fm[k] = metadata[k]
        fm['title'] = self.title
        fm['conf'] = self.conf

        self.font_matter = yaml.safe_dump(fm, default_flow_style=False, encoding='utf-8', allow_unicode=False)

        # print 'Article:', self.title

    def __str__(self):
        return self.text

    def __hash__(self):
        return hash(self.label)

    def __eq__(self, other):
        if not isinstance(other, type(self)): return NotImplemented
        return self.label == other.label


class Articles:
    def __init__(self):
        self.list = set()

        self.skip_files = set()

    def by_file_name(self, file_name):
        return next((x for x in self.list if file_name in x.file_names ), None)

    def by_label(self, label):
        return next((x for x in self.list if x.label == label), None)

    def add_markdown_file(self, file_name):
        a = self.by_file_name(file_name)
        if a: return a
        with open(file_name, 'r') as f:
            a = Article(os.path.splitext(os.path.basename(file_name))[0], file_name, f.read().strip())
            if self.by_label(a.label):
                print 'Duplicate article:', file_name
            else:
                self.list.add(a)
            return a

    def add_src_file(self, file_name):
        # parse file for comments
        # comment start from /** and contain front matter yaml with label
        # print "\nParsing conf: "+fname
        found_articles = set()
        a = self.by_file_name(file_name)
        if a:
            found_articles.add(a)
            return found_articles
        if file_name in self.skip_files:
            return found_articles
        self.skip_files.add(file_name)

        with open(file_name, 'r') as f:
            default_label = ''
            # find comments and add content to existing articles or create new
            for cmt, start, end in pyparsing.cStyleComment.scanString(f.read()):
                lines = cmt[0].splitlines()
                if len(lines) < 3:
                    continue
                if not (lines[0].strip() == "/**" and lines[-1].strip() == "*/"):
                    continue
                text = '\n'.join(lines[1:-1]).strip()
                if not (default_label or text.startswith('---\n')):
                    print 'Missing front matter:', file_name
                    continue
                metadata, text = Utils.parse_frontmatter(text)
                label = metadata['label'] if 'label' in metadata else default_label
                if not label:
                    print 'Missing label:', file_name
                    continue

                default_label = label
                a = self.by_label(label)
                if a:
                    a.text += '\n\n' + text
                    a.file_names.add(file_name)
                    # print 'Appended text:', label
                    found_articles.add(a)
                    continue

                # new article in src file - label must persist
                if 'label' not in metadata:
                    print 'Missing label:', file_name
                    continue

                a = Article(label, file_name, text, metadata)
                self.list.add(a)
                # print 'Article in src:', a.title
                found_articles.add(a)

        return found_articles

    def __str__(self):
        return ' '.join(map(lambda a: a.label, self.list))
