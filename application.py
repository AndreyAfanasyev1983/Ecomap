from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = "/tmp/sessions"
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    return render_template('portfolio.html', portfolio=db.execute("SELECT * FROM shares WHERE user_id = :userid", userid = session["user_id"]), cash = db.execute("SELECT cash FROM users WHERE id = :userid;", userid = session["user_id"]), name = db.execute("SELECT symbol,name,share_price FROM history"))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide symbol")
        elif not request.form.get("shares"):
            return apology("must provide shares")
        
        shares_count = request.form.get("shares")
        if not shares_count.isdigit():
            return apology("must provide valid number")
        
        symbol_name = request.form.get("symbol")
        shares_count = int(shares_count)
        symbol_info = lookup(symbol_name)
        if symbol_info is None:
            return apology("Failed to get symbol info")
            
        rows = db.execute("SELECT cash FROM users WHERE id = :userid;", userid = session["user_id"])
        if len(rows) != 1:
            return apology("Internal Error")
        
        available_cash = float(rows[0]["cash"])
        if available_cash < shares_count * symbol_info["price"]:
            return apology("Not enough money")
        
        available_cash -= shares_count * symbol_info["price"]
        db.execute("UPDATE users SET cash = :cash WHERE id = :userid;", cash = round(available_cash, 2), userid = session["user_id"]);
        
        rows = db.execute("SELECT shares_count FROM shares WHERE user_id = :userid AND symbol = :symbol;", userid = session["user_id"], symbol= symbol_info["symbol"])
        if len(rows) == 1:
            total_shares = shares_count + int(rows[0]["shares_count"])
            db.execute("UPDATE shares SET shares_count = :count WHERE user_id = :userid AND symbol = :symbol;", count = total_shares, userid = session["user_id"], symbol= symbol_info["symbol"])
        else:
            db.execute("INSERT INTO shares (user_id, symbol, shares_count) VALUES (:userid, :symbol, :count);", userid = session["user_id"], symbol= symbol_info["symbol"], count = shares_count)
            
        #Insert data in History table
        db.execute("INSERT INTO history (user_id, symbol, name, share_count, share_price) VALUES (:userid, :symbol, :name, :share_count, :share_price);", userid = session["user_id"], symbol= symbol_name, name = symbol_info["name"], share_count = shares_count, share_price = symbol_info["price"])
        #return apology("ALL OK " + str(available_cash))
        return render_template('portfolio.html', portfolio=db.execute("SELECT * FROM shares WHERE user_id = :userid", userid = session["user_id"]), cash = db.execute("SELECT cash FROM users WHERE id = :userid;", userid = session["user_id"]), name = db.execute("SELECT symbol,name,share_price FROM history"))
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    return render_template('history.html', history=db.execute("SELECT * FROM history WHERE user_id = :userid", userid = session["user_id"]))
    
    
    #transaction_history = db.execute("SELECT * FROM history WHERE user_id = :userid", userid = session["user_id"])
    #return apology(str(transaction_history))
    #return render_template("history.html")

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide symbol")
        symbol_info = lookup(request.form.get("symbol"))
        if symbol_info is None:
            return apology("Failed to get symbol info")
            
        return render_template("quoted.html", symbol_info = symbol_info)
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("INSERT INTO users(username, hash) VALUES(:username, :hash)", username=request.form.get("username"), hash = pwd_context.encrypt(request.form.get("password")))
        if rows is None:
            return apology("User already exists")
            
        session["user_id"] = rows
        return redirect(url_for("index"))
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide symbol")
        elif not request.form.get("shares"):
            return apology("must provide shares")
        
        shares_count = request.form.get("shares")
        if not shares_count.isdigit():
            return apology("must provide valid number")
        
        symbol_name = request.form.get("symbol")
        symbol_name = symbol_name.upper()
        shares_count = int(shares_count)
        symbol_info = lookup(symbol_name)
        
        avaliable_cash = db.execute("SELECT cash FROM users WHERE id = :userid", userid = session["user_id"])
        avaliable_shares_count = db.execute("SELECT shares_count FROM shares WHERE user_id = :userid AND symbol = :symbol;", userid = session["user_id"], symbol= symbol_name)
        
        if len(avaliable_shares_count) != 1:
            return apology("Symbol not owned")
        else :
            avaliable = int(avaliable_shares_count[0]["shares_count"])
            if avaliable < shares_count:
                return apology("Too many shares")
            else:
                avaliable_shares_count = avaliable - shares_count;
                db.execute("UPDATE shares SET shares_count = :count WHERE user_id = :userid AND symbol = :symbol;", count = avaliable_shares_count, userid = session["user_id"], symbol= symbol_info["symbol"])
                
                available_cash = float(avaliable_cash[0]["cash"])
                new_cash = available_cash + shares_count * symbol_info["price"]
                db.execute("UPDATE users SET cash = :cash WHERE id = :userid", cash = round(new_cash, 2), userid = session["user_id"])
                #Delete row after selling all shares
                db.execute("DELETE FROM shares WHERE shares_count = '0';")
                #Insert data in History table
                db.execute("INSERT INTO history (user_id, symbol, name, share_count, share_price) VALUES (:userid, :symbol, :name, :share_count, :share_price);", userid = session["user_id"], symbol= symbol_name, name = symbol_info["name"], share_count = -shares_count, share_price = symbol_info["price"])
                
                #return apology("All right, show table here")
                return render_template('portfolio.html', portfolio=db.execute("SELECT * FROM shares WHERE user_id = :userid", userid = session["user_id"]), cash = db.execute("SELECT cash FROM users WHERE id = :userid;", userid = session["user_id"]), name = db.execute("SELECT symbol,name,share_price FROM history"))
    else:
        return render_template("sell.html")