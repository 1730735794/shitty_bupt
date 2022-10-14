import sys


class GrammarNode:
    def __init__(self, text: str = ""):
        self.indent = -4
        for i in range(len(text)):
            if text[i] != ' ':
                self.indent = i
                break
        text = text[self.indent:]
        self.command = text.split(' ')[0]
        self.value = text[text.find(' ') + 1:]
        self.child: GrammarNode = None
        self.parent: GrammarNode = None
        self.brother: GrammarNode = None


class Parser:
    def __init__(self, file_path: str):
        with open(file_path, "r", encoding="utf8") as f:
            total_text = f.read()
        self.text = list(filter(lambda line: not line.isspace() and len(line) and line[0] != '#',
                                total_text.split('\n')))
        # 构建语法树
        self.parse_tree = GrammarNode("end")
        self.parse_tree.indent = -4
        node_stack = [self.parse_tree]
        for line in self.text:
            new_node = GrammarNode(line)
            # 根据缩进判断语句在语法树中的层级
            while new_node.indent != node_stack[-1].indent + 4:
                node_stack.pop()
            if node_stack[-1].child == None:
                node_stack[-1].child = new_node
            else:
                last_child = node_stack[-1].child
                while last_child.brother != None:
                    last_child = last_child.brother
                last_child.brother = new_node
            new_node.parent = node_stack[-1]
            node_stack.append(new_node)

    def execute(self):
        symbols = {}
        # 获取变量的值或字面值
        get_value = lambda v: v[1:-
                                1] if v[0] == "\"" and v[-1] == "\"" else symbols[v]
        current_node = self.parse_tree.child
        while(True):
            if current_node.command == "end":
                break
            elif current_node.command == "retry":
                current_node = self.parse_tree.child
                continue
            elif current_node.command == "print":
                print(get_value(current_node.value), end='')
            elif current_node.command == "println":
                print(get_value(current_node.value))
            elif current_node.command == "input":
                symbols[current_node.value] = input()
            elif current_node.command == "switch":
                var = get_value(current_node.value)
                is_pattern_matched = False
                current_child = current_node.child
                # 匹配对应分支，跳转执行
                while current_child != None and current_child.command != None:
                    if current_child.value == "other" or var == get_value(current_child.value):
                        current_node = current_child.child
                        is_pattern_matched = True
                        break
                    current_child = current_child.brother
                if is_pattern_matched:
                    continue
            elif current_node.command == "case":
                current_node = current_node.parent
            else:
                print("Syntax error")
                return
            # 下一条语句
            if current_node.brother != None:
                current_node = current_node.brother
            else:
                current_node = current_node.parent
        print()


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("please input the path of the script!")
        exit(1)
    parser = Parser(sys.argv[1])
    parser.execute()
