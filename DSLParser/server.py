from flask import Flask, request
from flask.templating import render_template
from subprocess import PIPE, Popen
import sys
import fcntl
import os
import time
import json


app = Flask(__name__)
pipe = Popen


@app.route("/")
def home():
    global pipe
    pipe = Popen(["python3", "./DSLParser.py", sys.argv[1]],
                 encoding="utf8", stdin=PIPE, stdout=PIPE)
    fcntl.fcntl(pipe.stdout.fileno(), fcntl.F_SETFL, os.O_NONBLOCK)
    time.sleep(0.1)
    text = pipe.stdout.read()
    pipe.stdout.flush()
    text = text.replace('\n', "<br/>")
    return render_template("index.html", msg=text)


@app.route("/reply", methods=["POST"])
def get_reply():
    user_input: str = request.get_json()["msg"]
    print(user_input)
    pipe.stdin.write(user_input + '\n')
    pipe.stdin.flush()
    time.sleep(0.1)
    j = {}
    j["msg"] = pipe.stdout.read().replace('\n', "<br/>").split('<br/><br/>')
    return json.dumps(j)


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("please input the path of the script!")
        exit(1)
    app.run(debug=True)
