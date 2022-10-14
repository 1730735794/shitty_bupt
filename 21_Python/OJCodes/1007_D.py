class Vector3:
    def __init__(self, nums):
        self.coord = nums

    def mul(self, num):
        for i in range(3):
            self.coord[i] *= num

    def div(self, num):
        for i in range(3):
            self.coord[i] /= num

    def get_length(self):
        return (self.coord[0] ** 2 + self.coord[1] ** 2 + self.coord[2] ** 2) ** 0.5

    def add(self, x):
        for i in range(3):
            self.coord[i] += x.coord[i]

    def sub(self, x):
        for i in range(3):
            self.coord[i] -= x.coord[i]


nums = list(map(int, input().split()))
a = Vector3(nums)
nums = list(map(int, input().split()))
b = Vector3(nums)
cmd = input()
if cmd == "add":
    a.add(b)
    print("{} {} {}".format(*a.coord))
elif cmd == "sub":
    a.sub(b)
    print("{} {} {}".format(*a.coord))
elif cmd == "mul":
    num = int(input())
    a.mul(num)
    print("{} {} {}".format(*a.coord))
elif cmd == "div":
    num = int(input())
    a.div(num)
    print("{:.2f} {:.2f} {:.2f}".format(*a.coord))
elif cmd == "get_length":
    print("{:.2f}".format(a.get_length()))
