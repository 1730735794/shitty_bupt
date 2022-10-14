import matplotlib.pyplot as plt

plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False
plt.rcParams['toolbar'] = 'None'

data = [51686.39, 10325.60, 7927.91, 5407.22, 5304.16, 980.11, 801.55, 770.00, 16041.03]
labels = ["生活日用", "文教娱乐", "服饰美容", "交通出行", "饮食", "运动健康", "通讯物流", "住房缴费", "其他消费"]

plt.figure(figsize=(10, 10))
plt.title("2018年支付宝年支出")
plt.pie(data, [0.05] * len(data), labels, autopct="%1.2f%%", startangle=90, pctdistance=1)
plt.show()