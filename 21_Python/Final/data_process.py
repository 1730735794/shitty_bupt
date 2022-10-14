import json
import matplotlib.pyplot as plt

plt.rcParams['font.sans-serif'] = ['SimHei'] # 步骤一（替换sans-serif字体）
plt.rcParams['axes.unicode_minus'] = False   # 步骤二（解决坐标轴负数的负号显示问题）
plt.rcParams['toolbar'] = 'None'

class GraphMaker:
    def __init__(self):
        pass

    def get_new_infected_graph(self, country_name: str, country_statics: dict):
        plt.figure(figsize=(8, 6))
        plt.title(country_name + "近15天新增病例")
        x_axis_labels = list(country_statics["new_infected"].keys())
        data = list(country_statics["new_infected"].values())
        x_axis_labels.reverse()
        data.reverse()
        plt.xticks(range(len(x_axis_labels)), x_axis_labels)
        plt.plot(range(len(x_axis_labels)), data)
        plt.show()

if __name__ == "__main__":
    with open("res.json", "r", encoding="utf8") as f:
            data = json.load(f)
    gm = GraphMaker()
    gm.get_new_infected_graph("美国", data["美国"])