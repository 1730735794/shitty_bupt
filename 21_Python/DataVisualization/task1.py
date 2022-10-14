import matplotlib.pyplot as plt

plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False
plt.rcParams['toolbar'] = 'None'

data = [58260, 69458, 100818, 113368, 126583, 133972, 141178]
labels = ["1953", "1964", "1982", "1990", "2000", "2010", "2020"]

plt.title("历次普查全国人口")
plt.xlabel("年份")
plt.ylabel("万人")
plt.bar(labels, data, label="人数")
for a,b in zip(labels, data):
    plt.text(a, b, b, ha="center", va="bottom")
plt.legend()
plt.show()