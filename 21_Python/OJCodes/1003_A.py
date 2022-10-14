dict = {}
n = int(input())
for i in range(n):
    kv = input().split()
    dict[kv[1]] = kv[0]
while True:
    woof = input()
    if woof == "dog":
        break
    meow = dict.get(woof)
    if meow == None:
        print("dog")
    else:
        print(meow)
