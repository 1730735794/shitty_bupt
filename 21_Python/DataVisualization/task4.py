from matplotlib import lines
import matplotlib.pyplot as plt
import pandas

plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False
plt.rcParams['toolbar'] = 'None'

data = pandas.read_excel("./八年级期末考试成绩表.xlsx")
plt.figure(figsize=(10, 10))
plt.subplots_adjust(wspace=0.5, hspace=0.5)
for i in range(2):
    for j in range(3):
        num = i * 3 + j + 1
        max_score = 100
        if num >= 6:
            max_score = 120
        plt.subplot(2, 3, num)
        plt.title(data.columns[num], fontsize=10)
        plt.xlim(0, max_score)
        plt.xlabel("成绩", fontsize=8)
        plt.ylabel("人数", fontsize=8)
        plt.xticks(range(0, max_score + 1, 10), fontsize=8)
        plt.yticks(fontsize=8)
        n, bins, patches = plt.hist(data[data.columns[num]], int(max_score / 10), (0, max_score), edgecolor="black")
        for a, b in zip(bins, n):
            plt.text(a + 5, b, "%d"% b, ha="center", va="bottom")
plt.show()