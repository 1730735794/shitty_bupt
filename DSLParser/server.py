import sys
from flask import Flask, request
import time
import json
from flask_cors import CORS
import time
from threading import  Lock
import threading
from killThread import stop_thread

app = Flask(__name__)

#防止http请求呗拦截
CORS(app, supports_credentials=True)

# response_text的锁
response_lock = Lock()
response_text = ""
input_lock = Lock()
input_text = ""


class Node:
    def __init__(self, text: str = ""):
        self.indent = -4
        for i in range(len(text)):
            if text[i] != ' ':
                self.indent = i
                break
        text = text[self.indent:]
        self.parent: Node = None
        self.brother: Node = None
        self.child: Node = None
        self.keyword = text.split(' ')[0]
        self.value = text[text.find(' ') + 1:]
        

class Parser:
    def __init__(self, file_path: str):
        with open(file_path, "r", encoding="utf8") as f:
            total_text = f.read()
        f.close()
        # 将脚本文本按行分割，并去掉空行和注释行
        self.text = list(filter(lambda line: not line.isspace() and len(line) and line[0] != '#',
                                total_text.split('\n')))
        # 构建语法树
        self.parse_tree = Node("end")
        self.parse_tree.indent = -4
        node_stack = [self.parse_tree]
        for line in self.text:
            new_node = Node(line)
            # 根据缩进判断语句在语法树中的层级
            # 当新的一行缩进为0，则弹出所有节点
            while new_node.indent != node_stack[-1].indent + 4:
                node_stack.pop()
            if node_stack[-1].child != None:
                last_child = node_stack[-1].child
                while last_child.brother != None:
                    last_child = last_child.brother
                last_child.brother = new_node
            else:
                node_stack[-1].child = new_node
            new_node.parent = node_stack[-1]
            node_stack.append(new_node)

    def run(self):
        global response_text
        global input_text
        symbols = {}
        # 获取变量的值或字面值
        get_value = lambda v: v[1:-1] if v[0] == "\"" and v[-1] == "\"" else symbols[v]
        current_node = self.parse_tree.child
        while(True):
            if current_node.keyword == "input":
                # 从管道中读取用户输入,达到阻塞的目的
                while input_text == "":
                    time.sleep(1)
                with input_lock:
                    symbols[current_node.value] = input_text
                    input_text=""
            elif current_node.keyword == "retry":
                current_node = self.parse_tree.child
                print("retry")
                continue
            elif current_node.keyword == "end":
                break
            elif current_node.keyword == "print":
                # 将脚本输出放入管道
                with response_lock: 
                    response_text += get_value(current_node.value)
            elif current_node.keyword == "println":
                # 将脚本输出放入管道
                with response_lock: 
                    response_text += get_value(current_node.value) + '\n'
            elif current_node.keyword == "switch":
                var = get_value(current_node.value)
                flag = False
                current_child = current_node.child
                # 匹配对应分支，跳转执行
                while current_child != None and current_child.keyword != None:
                    if current_child.value == "other" or var == get_value(current_child.value):
                        flag = True
                        current_node = current_child.child
                        break
                    current_child = current_child.brother
                if flag:
                    continue
            elif current_node.keyword == "case":
                current_node = current_node.parent
            else:
                print("Syntax error")
                return
            # 下一条语句
            if current_node.brother != None:
                current_node = current_node.brother
            else:
                current_node = current_node.parent


# 为了便于创建线程，对脚本指示器和web服务器做一次封装
def runParser(filename):
    Parser(filename).run()

def runServer():
    app.run(port=8000)

@app.route("/reply", methods=["POST"])
def get_reply():
    # 获取用户输入并将用户输入放入管道
    global input_text
    user_input: str = request.get_json()["msg"]
    with input_lock:
        input_text = user_input
    time.sleep(1)
    # 获取所有脚本输出
    global response_text
    json_text = {}
    with response_lock:
        json_text["msg"] = response_text.replace('\n', "<br/>").split('<br/><br/>')
        response_text=""
    return json.dumps(json_text)

# 在页面启动时打招呼
@app.route("/hi", methods=["POST"])
def send_mess():
    time.sleep(1)
    global response_text
    json_text = {}
    with response_lock:
        json_text["msg"] = response_text.replace('\n', "<br/>").split('<br/><br/>')
        response_text=""
    return json.dumps(json_text)

parser_thread = threading.Thread(target=runParser, args=(sys.argv[1],))

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("please input the path of the script!")
        exit(1)
    app_thread = threading.Thread(target=runServer)
    app_thread.start()
    parser_thread.start()
    # 建立两个线程，分别执行脚本解释器和web服务器
    app_thread.join()


