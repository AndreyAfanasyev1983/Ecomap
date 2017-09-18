import os
import re
from flask import Flask, jsonify, render_template, request, url_for, redirect
from flask_jsglue import JSGlue

from cs50 import SQL


# configure application
app = Flask(__name__)
JSGlue(app)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///ecomap.db")
# index.html
@app.route("/")
def index():
    """Render map."""
    if not os.environ.get("API_KEY"):
        raise RuntimeError("API_KEY not set")
    return render_template("index.html", key=os.environ.get("API_KEY"))

# ecomap.html
@app.route("/all")
def all():
    """Render map."""
    if not os.environ.get("API_KEY"):
        raise RuntimeError("API_KEY not set")
    return render_template("ecomap.html", key=os.environ.get("API_KEY"))

# send info in SQL database
@app.route("/send", methods=["POST"])
def send():
    _lat = request.form.get("lat")
    _lng = request.form.get("long")
    _name = request.form.get('name')
    _comment = request.form["comment"]
    _contact = request.form["contact"]
    db.execute("INSERT INTO geomarkers(Lat, Long, Name, comment, Contact) VALUES( :lat, :lng, :name, :comment, :contact)", lat = _lat, lng = _lng, name = _name, comment = _comment, contact = _contact)
    return redirect(url_for("all"))

# get info from sql and send json
@app.route("/markers", methods=["GET"])
def markers():
    markers = db.execute("SELECT * FROM geomarkers")
    return jsonify(markers);