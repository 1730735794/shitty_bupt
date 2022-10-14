n = int(input())
classes = list(map(int, input().split()))
classes.sort()
ans = 0
for i in range(int((n + 1) / 2)):
    ans += int((classes[i] + 1) / 2)
print(ans)
