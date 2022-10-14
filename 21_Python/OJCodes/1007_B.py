class queue:
    def __init__(self):
        self.queue = []

    def qout(self, num):
        ret = []
        num = min(num, len(self.queue))
        for _ in range(num):
            ret.append(self.queue[0])
            queue = self.queue.pop(0)
        return ret

    def qin(self, list=[]):
        for num in list:
            self.queue.append(num)

    def show(self):
        if len(self.queue) == 0:
            print("len = 0")
        else:
            print("len = {}, data = ".format(len(self.queue)), end="")
            print(str(self.queue).replace(
                '[', '').replace(']', '').replace(',', ''))


length = int(input())
nums = list(map(int, input().split()))
s1 = queue()
s2 = queue()
s1.qin(nums)
cmd = input().split()
nums = list(map(int, cmd[1:]))
cmd = cmd[0]
if cmd == "out":
    s2.qin(s1.qout(nums[0]))
elif cmd == "in":
    s1.qin(nums)
cmd = input().split()
nums = list(map(int, cmd[1:]))
cmd = cmd[0]
if cmd == "out":
    s2.qin(s1.qout(nums[0]))
elif cmd == "in":
    s1.qin(nums)
s1.show()
s2.show()
