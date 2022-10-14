nm = list(map(int, input().split()))
str1 = input()
ans = ""
for ch in str1:
    if ch != '[' and ch != ']':
        ans += ch
nums = list(map(int, ans.split(',')))
str1 = input()
ans = ""
for ch in str1:
    if ch != '[' and ch != ']':
        ans += ch
nums1 = list(map(int, ans.split(',')))
for i in range(len(nums)):
    nums[i] += nums1[i]
ans = []
for i in range(nm[0]):
    ans.append([])
    for j in range(nm[1]):
        ans[len(ans)-1].append(nums[i * nm[1] + j])
strans = str(ans)
ans = ""
for ch in strans:
    if ch != ' ':
        ans += ch
print(ans)
