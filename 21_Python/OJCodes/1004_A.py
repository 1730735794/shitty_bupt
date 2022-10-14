str = input()
ans = ""
for ch in str:
    if ch.isupper():
        ans += ch.lower()
    elif ch.islower():
        ans += ch.upper()
    else:
        ans += ch
print(ans)
