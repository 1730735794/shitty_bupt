nm = list(map(int, input().split()))
n = nm[0]
m = nm[0]
lrulist = [0] * n
count = 0
orilist = list(map(int, input().split()))
for cur in orilist:
    if cur in lrulist:
        lrulist.remove(cur)
        lrulist.insert(0, cur)
    else:
        count += 1
        lrulist.insert(0, cur)
        lrulist = lrulist[0:n]
print(count)
lrulist.sort()
print(lrulist[0], end="")
for i in range(1, len(lrulist)):
    print(" {}".format(lrulist[i]), end="")
print("")
