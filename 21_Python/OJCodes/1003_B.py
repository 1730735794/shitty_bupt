na = int(input())
seta = set(map(int, input().split()))
nb = int(input())
setb = set(map(int, input().split()))
list1 = list(seta.intersection(setb))
list1.sort()
list2 = list(seta.union(setb))
list2.sort()
list3 = list(seta - setb)
list3.sort()
for x in list1:
    print(x, end=" ")
print("")
for x in list2:
    print(x, end=" ")
print("")
for x in list3:
    print(x, end=" ")
print("")
