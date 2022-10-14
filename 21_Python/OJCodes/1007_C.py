line1 = input()
line2 = input()
line3 = input()
with open("tmp", "w") as f:
    f.write(line1)
if line2 != 'r' and line2 != 'x' and line2 != 'x+':
    with open("tmp", line2) as f:
        f.write(line3)
with open("tmp", "r") as f:
    str = f.read()
    print(str)
