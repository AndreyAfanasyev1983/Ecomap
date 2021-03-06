from flask import Flask, redirect, render_template, request, url_for

import helpers
from analyzer import Analyzer
import sys
import os

def percentage(part, total):
    return part/total * 100 

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name", "")
    if not screen_name:
        return redirect(url_for("index"))
        
    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")
    analyzer = Analyzer(positives, negatives)

    # get screen_name's tweets
    positive, negative, neutral = 0, 0, 0
    
    tweets = helpers.get_user_timeline(screen_name, 100)
    for tweet in tweets:
        score = analyzer.analyze(tweet)
        color = ""
        if score > 0.0:
            positive += 1
        elif score < 0.0:
            negative += 1
        else:
            neutral += 1

    # TODO
    

    # generate chart
    chart = helpers.chart(percentage(positive, len(tweets)), percentage(negative, len(tweets)), percentage(neutral, len(tweets)))

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)
