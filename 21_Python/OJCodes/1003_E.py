s = input()
sset = set()
for x in s:
    sset.add(x)
slist = sorted(sset)
for x in slist:
    print(x, end="")
print("")
