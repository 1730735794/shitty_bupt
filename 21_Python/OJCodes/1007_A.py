class stack:
    def __init__(self):
        self.stack = []

    def pop(self, num):
        ret = []
        num = min(num, len(self.stack))
        for _ in range(num):
            ret.append(self.stack.pop())
        return ret

    def push(self, list=[]):
        for num in list:
            self.stack.append(num)

    def show(self):
        if len(self.stack) == 0:
            print("len = 0")
        else:
            print("len = {}, data = ".format(len(self.stack)), end="")
            print(str(self.stack).replace(
                '[', '').replace(']', '').replace(',', ''))


length = int(input())
nums = list(map(int, input().split()))
s1 = stack()
s2 = stack()
s1.push(nums)
cmd = input().split()
nums = list(map(int, cmd[1:]))
cmd = cmd[0]
if cmd == "pop":
    s2.push(s1.pop(nums[0]))
elif cmd == "push":
    s1.push(nums)
cmd = input().split()
nums = list(map(int, cmd[1:]))
cmd = cmd[0]
if cmd == "pop":
    s2.push(s1.pop(nums[0]))
elif cmd == "push":
    s1.push(nums)
s1.show()
s2.show()
