str = list(map(str, list(map(int, input().split()))))
prefixZero = [True] * 2
ans = ""
for i in range(5):
    for j in range(2):
        if i < len(str[j]):
            ans += str[j][i]
print(ans)
