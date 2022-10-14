n = int(input())
prime = []
for i in range(2, n + 1):
    isPrime = True
    for div in range(2, int(i / 2 + 1)):
        if i % div == 0:
            isPrime = False
            break
    if isPrime:
        prime.append(i)
print(prime[0], end="")
for i in range(1, len(prime)):
    print(" {}".format(prime[i]), end="")
