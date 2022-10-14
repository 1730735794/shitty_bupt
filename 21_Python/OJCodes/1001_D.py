n = int(input())
score = list(map(int, input().split()))
sum = 0
count = 0
for x in score:
    sum += x
    if x >= 60:
        count += 1
print("average = {:.1f}".format(sum / n))
print("count = {}".format(count))
