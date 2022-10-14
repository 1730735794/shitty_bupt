from matplotlib import lines
import matplotlib.pyplot as plt
import pandas

plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False
plt.rcParams['toolbar'] = 'None'

data = pandas.read_csv("./BeijingPM20100101_20151231.csv")
data_by_year = data.groupby("year")


plt.figure(figsize=(10, 10))
plt.title("北京市2010~2015年PM2.5指数月平均数据")
for name, year_data in data_by_year:
    data_by_month = year_data.groupby("month")
    plt.xlabel("月份", fontsize=8)
    plt.ylabel("PM2.5指数", fontsize=8)
    plt.xticks(fontsize=8)
    plt.yticks(fontsize=8)
    n, bins, patches = plt.hist(data[data.columns[num]], int(max_score / 10), (0, max_score), edgecolor="black")
    for a, b in zip(bins, n):
        plt.text(a + 5, b, "%d"% b, ha="center", va="bottom")
plt.show()