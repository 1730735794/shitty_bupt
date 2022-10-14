from matplotlib import lines
import matplotlib.pyplot as plt
import pandas

plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False
plt.rcParams['toolbar'] = 'None'

iris = pandas.read_csv("./iris.csv")
iris_grouped = iris.groupby("Species")

fig = plt.figure(figsize=(10, 10))
plt.subplots_adjust(wspace=0.5, hspace=0.5)
for i in range(4):
    for j in range(4):
        plt.subplot(4, 4, i * 4 + j + 1)
        plt.title(iris.columns[i + 1] + " vs " + iris.columns[j + 1], fontsize=8)
        plt.xlabel(iris.columns[i + 1], fontsize=8)
        plt.ylabel(iris.columns[j + 1], fontsize=8)
        plt.xticks(fontsize=8)
        plt.yticks(fontsize=8)
        for name, data in iris_grouped:
            plt.scatter(data[data.columns[i + 1]], data[data.columns[j + 1]], 1, label=name)
lines, labels = fig.axes[-1].get_legend_handles_labels()
fig.legend(lines, labels)
plt.show()