n = int(input())
nlist = list(map(int, input().split()))
slist = [nlist[0]]
nlist = nlist[1:]
for i in range(0, len(nlist)):
    slist.append(nlist[0])
    slist.sort()
    nlist = nlist[1:]
    for x in slist:
        print(x, end=" ")
    for x in nlist:
        print(x, end=" ")
    print("")
