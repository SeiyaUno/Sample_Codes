import index
import math
import os
import pickle
from operator import itemgetter
import time

MIN_WORD_NUM = 2  # use this for limiting number of potential docs
QUERY_TH = 3

# parameters for score calculation
IMPORTANT_WORDS_EFFECT = 0.05
SIMILARITY_EFFECT = 1


class Page_Score_Elms:
    # want to use this class for keeping various scores
    def __init__(self, ww):
        self.tfidf = []
        self.words_weight = ww  # add some score if a word is important


def get_alpha_fps():
    subdirs = [str(i) for i in range(10)]  # subdirs 0-9
    subdirs += [chr(i) for i in range(ord('a'), ord('z') + 1)]  # subdirs a-z
    subdirs.append("other")
    os.chdir(os.getcwd() + "/index")
    alpha_fps = dict()
    for subdir in subdirs:
        os.chdir(subdir)  # change to current alphanum subdir
        fp = open(subdir + ".txt", "r")  # open corresponding inverted index txt file
        alpha_fps[subdir] = fp  # store file ptr as value to subdir key
        os.chdir("..")
    os.chdir("..")
    return alpha_fps


def close_alpha_fps(alpha_fps):
    for fp in alpha_fps.values():
        fp.close()


def get_posting_list(index_offsets, alpha_fps, term):
    posting_list = []
    first = term[0]  # get first char of the term
    # if first char is alnum, get matching alphanumeric range file handle
    fp = alpha_fps[first] if first.isalnum() else alpha_fps["other"]
    # fp.seek(0)
    # for line in fp:
    #    li = line.split(';')
    #    if (li[0] == term):
    # get (docid, tf, iw_score) tuples from line
    #        posting_list = [(int(li[i]), int(li[i + 1]), int(li[i + 2])) for i in range(1, len(li) - 2, 3)]
    #        break
    # return posting_list

    if term in index_offsets:
        offset = index_offsets[term]
        if offset is not None:
            fp.seek(offset)
            li = fp.readline().rstrip()
            li = li.split(';')
            posting_list = [(int(li[i]), int(li[i + 1]), int(li[i + 2])) for i in range(1, len(li) - 2, 3)]
    return posting_list


def find_commonID(list1, list2):
    added = set()
    return_list = []

    for elm in list1:
        added.add(elm)

    for elm in list2:
        if elm in added:
            return_list.append(elm)

    return return_list


def get_tfidf(tf, total_doc, doc_f):
    return (1 + math.log10(tf)) * math.log10(total_doc / doc_f)

def get_lf(tf):
    return 1 + math.log10(tf)

def get_cosine_sim(vct1, vct2):
    if len(vct1) != len(vct2):
        print(f'vct1:{len(vct1)}, vct2:{len(vct2)}')
        print("def get_cosine_sim : vector length do not match")

    numerator = 0
    d1 = 0
    d2 = 0
    for i in range(0, len(vct1)):
        numerator += (vct1[i] * vct2[i])
        d1 += vct1[i] ** 2
        d2 += vct2[i] ** 2

    return numerator / (math.sqrt(d1) * math.sqrt(d2))


# this function receives a list of strings and a string to count
def get_term_freq(words, target):
    count = 0
    for w in words:
        if w == target:
            count += 1

    return count


# this function receives a list of posting object
def getDocids(posting_list):
    result = []

    for elm in posting_list:
        result.append(elm[0])

    return result


def search_engine(index_offsets, alpha_fps, doc_ids, user_query):
    total_docs = len(doc_ids)
    word_list = index.tokenizer(user_query)

    for i, w in enumerate(word_list):  # use stemmer
        word_list[i] = index.stemmer.stem(w)

    potential_pages = {}

    # get pages that contains at least one words from a query
    for w in word_list:
        posting_list = get_posting_list(index_offsets, alpha_fps, w)
        if posting_list:
            potential_pages.update({w: posting_list})  # simple_index._index[w] is a list of posting object

    if len(potential_pages) != 0:
        # if we find some matches, compute scores
        query_vct = []
        page_vct_dict = {}  # {docid : [list of score. should be parallel to query vct for computing cosine]}

        # construct page_vct_dict
        if len(potential_pages) > QUERY_TH:  # if the length of query is longer than 3 (number may change in the future)
            doc_count = {}  # {docid : count of how many times we see this docid in potential pages}

            for key in potential_pages:
                for id in potential_pages[key]:
                    if not id[0] in page_vct_dict:
                        page_vct_dict.update({id[0]: Page_Score_Elms(id[2])})

                    if not id[0] in doc_count:
                        doc_count.update({id[0]: 1})
                    else:
                        doc_count[id[0]] += 1

            for docid in doc_count:
                # remove docids that do not meet minimum requirement
                if doc_count[docid] <= MIN_WORD_NUM:
                    page_vct_dict.pop(docid)

        else:
            for key in potential_pages:
                for id in potential_pages[key]:
                    if not id[0] in page_vct_dict:
                        page_vct_dict.update({id[0]: Page_Score_Elms(id[2])})

        # compute tfidf term at a time
        # currently using lnc.ltc
        for i, w in enumerate(potential_pages):
            query_vct.append(get_tfidf(get_term_freq(word_list, w),
                                       total_docs + 1,  # + 1 for counting query as a document
                                       len(potential_pages[w]) + 1))

            for posting in potential_pages[w]:
                if len(potential_pages) <= QUERY_TH or (
                        len(potential_pages) > QUERY_TH and posting[0] in page_vct_dict):
                    page_vct_dict[posting[0]].tfidf.append(get_lf(posting[1]))

            # for those ids that did not get update, add 0 at the end of the list
            for key in page_vct_dict:
                if len(page_vct_dict[key].tfidf) < i + 1:
                    page_vct_dict[key].tfidf.append(0)

        # at this point, we should have all tfidf computed for query and potential documents.
        # query and potential documents should have list with the same length

        # compute cosine similarity
        score_dict = {}  # {doc id : score}

        # for key in page_vct_dict: # for debug
        #     print(page_vct_dict[key].tfidf)

        for key in page_vct_dict:
            score_dict.update({key: get_cosine_sim(page_vct_dict[key].tfidf, query_vct) * SIMILARITY_EFFECT})
            score_dict[key] += (math.sqrt(page_vct_dict[key].words_weight) * IMPORTANT_WORDS_EFFECT)
        # sort by cosine similarity
        # sorted function returns a list, not a dictionary
        score_dict = sorted(score_dict.items(), key=itemgetter(1), reverse=True)

    finalResult = []  # this will become a list of urls
    if len(potential_pages) != 0:
        for i in score_dict:
            # print(index.doc_ids[i[0]]['url'])
            finalResult.append(doc_ids[i[0]]['url'])

        # this is only for debugging
        for i, u in enumerate(score_dict):
            #print(u)
            if i == 20:
                break

    return finalResult


if __name__ == "__main__":
    # get file ptrs to alphanumeric range index files
    alpha_fps = get_alpha_fps()

    # get doc ids dict
    with open("doc-ids.p", "rb") as fp:
        doc_ids = pickle.load(fp)

    # get index offsets dict
    with open("index-offsets.p", "rb") as fp:
        index_offsets = pickle.load(fp)

    while True:
        print("Type something")
        user_in = input()

        if user_in == "quitprog":
            break

        start = time.time()
        result = search_engine(index_offsets, alpha_fps, doc_ids, user_in)
        end = time.time()
        for i in range(20):
            if i == len(result):
                break
            print(result[i])
        print(f'number of urls: {len(result)}')
        print(f'search time {end - start} seconds')
    close_alpha_fps(alpha_fps)