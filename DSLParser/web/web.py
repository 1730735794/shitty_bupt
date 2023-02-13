from flask import Flask, request
from flask.templating import render_template
import sys
import fcntl
import os
import time
import json


app = Flask(__name__)


@app.route("/")
def home():
    return render_template("index.html", msg='')

if __name__ == "__main__":
    app.run(debug=True)

    
