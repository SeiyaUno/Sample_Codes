import os
import json
import sys
import nltk
import pickle
import psutil
from bs4 import BeautifulSoup
from simhash import Sim
from nltk.corpus import stopwords

nltk.download('stopwords')

stemmer = nltk.stem.PorterStemmer()
stop_words = set(stopwords.words('english'))


def parse_file(path):
    with open(path) as fp:
        # fp is a json file with the following keys
        # "url" - the url of the page
        # "content" - the html content of the page
        # "encoding" - the encoding scheme of the page
        data = json.load(fp)
        url = data["url"]  # get page url
        # encoding = data["encoding"] # get encoding scheme of page
        content = data["content"]  # get html content of json file
        htmlParse = BeautifulSoup(content,
                                  'html.parser')  # parse the file into html, not sure if this is the correct thing to do
        text = ""
        for para in htmlParse.find_all(
                ["p", "h1", "h2", "h3", "h4", "h5", "h6", "b", "strong", "title"]):  # please add more tag if neccesary
            text += para.get_text()

        # for identifying words with some importance
        weighted_text = ""
        for w in htmlParse.find_all(["h1", "h2", "h3", "b", "strong", "title"]):
            weighted_text += w.get_text()

        return (url, text, weighted_text)


def tokenizer(text):
    result = text.split()
    final = []

    for i, str in enumerate(result):
        if "'" in str:  # take care of apostrophe
            str = str[:str.index("'")]

        # check each character in a word
        word = ''.join(cha for cha in str if cha.isalnum())

        # add to finalResult if it is not empty
        if word != "":
            word = word.lower()
            final.append(word)

    return final


class Posting:
    """
    A Posting for use in an Inverted Index

    docid: Unique document ID of the document where the term is located
    tf: Term Frequency - num of times term occurs in the document
    """

    def __init__(self, docid, tf):
        self.docid = docid
        self.tf = tf
        self.iw_score = 0

    def __str__(self):
        return "[docid: " + str(self.docid) + ", tf: " + str(self.tf) + "]"


class Index:
    """Inverted Index"""

    def __init__(self, corpus_path):
        """
        Create new Inverted Index instance.
        """
        self._index = dict()
        self._corpus_path = corpus_path
        self._partial_index_files = []  # store partial index file names
        self._doc_ids = {}
        self._full_index_file = "full.txt"  # store full index file name
        self._index_subdirs = [str(i) for i in range(10)]  # subdirs 0-9
        self._index_subdirs += [chr(i) for i in range(ord('a'), ord('z') + 1)]  # subdirs a-z
        self._index_subdirs.append("other")
        self._alpha_fps = dict()
        self._index_offsets = dict()  # index with offsets into inverted index

    def get_sorted_tokens(self, index):
        return [key for key in sorted(index.keys())]

    def mem_thresh_exceeded(self, threshold=15.0):
        # check if memory utilization has exceeded the threshold
        mem = psutil.virtual_memory()
        return True if (mem.percent >= threshold) else False

    def dump_doc_ids(self):
        os.chdir("..")
        with open("doc-ids.p", "wb") as fp:
            pickle.dump(self._doc_ids, fp)
        os.chdir(self._corpus_path)

    def dump_index_offsets(self):
        with open("index-offsets.p", "wb") as fp:
            pickle.dump(self._index_offsets, fp)
        os.chdir(self._corpus_path)

    def dump_partial_index(self):
        os.chdir("../partial")
        file_name = "partial" + str(len(self._partial_index_files) + 1) + ".p"
        sorted_tokens = self.get_sorted_tokens(self._index)
        with open(file_name, "wb") as fp:
            pickle.dump((sorted_tokens, self._index), fp)
            self._partial_index_files.append(file_name)
        os.chdir(self._corpus_path)

    def load_partial_index(self, file_name):
        os.chdir("../partial")
        with open(file_name, "rb") as fp:
            partial_index = pickle.load(fp)
        os.chdir(self._corpus_path)
        return partial_index

    def dump_full_index(self, sorted_terms, full_index):
        os.chdir("..")
        with open(self._full_index_file, "w", errors="ignore") as fp:
            for term in sorted_terms:
                fp.write(term + ";")
                for posting in full_index[term]:
                    fp.write(str(posting.docid) + ";" + str(posting.tf) + ";" + str(posting.iw_score) + ";")
                fp.write("\n")
        os.chdir(self._corpus_path)

    def merge_partial_indices(self, tokens1, pi1, tokens2, pi2):
        """
        Merge two partial inverted index files into a single
        inverted index file

        tokens1: list of sorted tokens from partial index 1
        pi1: partial index 1 (dict of Posting lists)
        tokens2: list of sorted tokens from partial index 2
        pi2: partial index 2 (dict of Posting lists)
        """
        i = j = 0
        sorted_tokens = []
        merged_index = dict()

        while (i < len(pi1)) and (j < len(pi2)):
            if (tokens1[i] <= tokens2[j]):
                merged_index[tokens1[i]] = merged_index.get(tokens1[i],
                                                            []) + pi1[tokens1[i]]
                i += 1  # advance to next token in partial index 1
            else:
                merged_index[tokens2[j]] = merged_index.get(tokens2[j],
                                                            []) + pi2[tokens2[j]]
                j += 1  # advance to next token in partial index 2

        # add trailing posting lists to merged index
        while (i < len(pi1)):
            # add posting list for current token in partial index 1
            merged_index[tokens1[i]] = merged_index.get(tokens1[i],
                                                        []) + pi1[tokens1[i]]
            i += 1  # advance to next token in partial index 1

        while (j < len(pi2)):
            # add posting list for current token in partial index 2
            merged_index[tokens2[j]] = merged_index.get(tokens2[j],
                                                        []) + pi2[tokens2[j]]
            j += 1
        sorted_tokens = self.get_sorted_tokens(merged_index)
        return (sorted_tokens, merged_index)

    def build_alpha_index_dirs(self):
        try:
            os.mkdir(os.path.dirname(os.getcwd()) + "/index")  # construct the index dir
            os.chdir(os.path.dirname(os.getcwd()) + "/index")
            for subdir in self._index_subdirs:
                os.mkdir(subdir)  # construct subdirs in index dir
                os.chdir(subdir)  # change to current alphanumeric subdir
                # create file object for inverted index of current alphanumeric range
                fp = open(subdir + ".txt", "a")
                self._alpha_fps[subdir] = fp
                os.chdir("..")
            os.chdir(self._corpus_path)
        except FileExistsError:
            pass

    def build_alpha_index(self):
        self.build_alpha_index_dirs()
        os.chdir("..")
        i = 0
        n = len(self._index_subdirs) - 1
        alphanum = self._index_subdirs[i]
        alpha_fp = self._alpha_fps[alphanum]
        with open(self._full_index_file, "r") as fp:
            for line in fp:  # for each line in the full index file
                # first char in term does not match current alphanumeric range
                if line[0] != alphanum and i < n:
                    # advance to next alphanumeric range
                    i += 1
                    alphanum = self._index_subdirs[i]
                    alpha_fp = self._alpha_fps[alphanum]
                offset = alpha_fp.tell()  # get offset into alphanum range file
                alpha_fp.write(line)  # write current line to alphanum range file
                term = line.split(';', 1)[0]  # get current term
                self._index_offsets[term] = offset  # add offset for current term
        # close fps
        for fp in self._alpha_fps.values():
            fp.close()
        # dump index offsets
        self.dump_index_offsets()

    def build_partial_indices(self):
        # construct partial indices and dump them to disk
        simcheck = Sim()
        os.chdir(self._corpus_path)  # set directory to given data
        # construct dir to store partial index files
        os.mkdir(os.path.dirname(os.getcwd()) + "/partial")
        for folder in os.listdir(os.getcwd()):
            if os.path.isdir(folder):  # check for directories(domains)
                websitepath = os.path.abspath(os.path.join(os.getcwd(), folder))  # get absolute path of domains
                site = os.listdir(websitepath)  # get domain pages

                for page in site:
                    pagepath = os.path.abspath(os.path.join(websitepath, page))  # absolute path of page
                    page_id = len(self._doc_ids)
                    url, text_content, imp_txt = parse_file(pagepath)

                    tokens = tokenizer(text_content)  # get list of tokens from current page
                    imp_tokens = tokenizer(imp_txt)
                    if simcheck.sim_check(tokens) == False:
                        self._doc_ids.update({page_id: {"name": page,
                                                        "url": url,
                                                        "path": pagepath}})  # set doc_id for each page

                        for i, w in enumerate(tokens):  # use stemmer
                            tokens[i] = stemmer.stem(w)

                        imp_tokens_filter = []
                        for i, w in enumerate(imp_tokens):
                            if not w in stop_words:
                                imp_tokens_filter.append(stemmer.stem(w))

                        imp_tokens.clear()
                        added_words = set()
                        for token in tokens:  # for each token from the current page

                            if not (token in self._index):  # if token not in the index
                                self._index[token] = []  # create empty posting list for token

                            # if a word is already added, that means Posting object with current page_id has already created.
                            if not (token in added_words):
                                self._index[token].append(
                                    Posting(page_id, 1))  # append to posting list for current token
                                added_words.add(token)
                            else:  # the current page should be at the end of the list
                                self._index[token][-1].tf += 1

                            if token in imp_tokens_filter:  # if a token is important
                                self._index[token][-1].iw_score += 1

            if self.mem_thresh_exceeded():
                self.dump_partial_index()  # dump partial index to disk
                self._index = dict()  # free index from mem

        if self._index:  # if index contains any remaining posting lists
            self.dump_partial_index()  # dump remainder to disk
            self._index = dict()  # free index from mem

    def build_index(self):
        i = 1
        n = len(self._partial_index_files)
        index = self.load_partial_index(self._partial_index_files[0])
        while (i < n):
            # merge current index with next partial index file
            partial_index = self.load_partial_index(
                self._partial_index_files[i])
            index = self.merge_partial_indices(
                index[0], index[1],
                partial_index[0], partial_index[1])
            i += 1  # advance to next partial index file
        # after merging partial index files, dump full index to disk
        self.dump_full_index(index[0], index[1])
        # dump doc_ids dict
        self.dump_doc_ids()


if __name__ == "__main__":
    # corpus path - set to DEV directory path
    # e.g. corpus_path = "/home/johndoe/CS121_A3/DEV"
    corpus_path = ""

    simple_index = Index(corpus_path)

    # construct partial indices and dump them to disk
    # can read through entire index by loading partial
    # index files sequentially
    simple_index.build_partial_indices()
    print(len(simple_index._doc_ids))

    # construct full index by merging partial index files
    simple_index.build_index()

    # split full inverted index file into alphabet ranges
    simple_index.build_alpha_index()

