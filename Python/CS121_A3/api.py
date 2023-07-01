import pickle

from flask import Flask, jsonify, request
from flask_cors import CORS
from engine import search_engine, get_alpha_fps
from index import Posting
from index import Index
import time


app = Flask(__name__)
app.config.update(DEBUG=True)
CORS(app)
alpha_fps = get_alpha_fps()
with open("doc-ids.p", "rb") as fp:
    doc_ids = pickle.load(fp)
with open("index-offsets.p", "rb") as fp:
    index_offsets = pickle.load(fp)

@app.route("/search", methods=['POST'])
def doSearch():
    query = request.form['query']
    start = time.time()
    res=search_engine(index_offsets, alpha_fps, doc_ids, query)
    end = time.time()
    time1 = end - start
    result = jsonify(time1, res)
    return result


app.run()