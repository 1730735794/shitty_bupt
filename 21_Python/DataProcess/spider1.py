from bs4.element import ResultSet
import requests
import bs4
import csv
import threading
import re

# config here

PAGE_NUM = 23
TERM_IN_PAGE_NUM = 10


BASE_URL = "https://bj.fang.lianjia.com/loupan/"
CSV_HEADERS = ["名称", "地理位置1", "地理位置2", "地理位置3", "房型", "面积", "均价", "总价"]

result_list = []


def one_page_pipeline(url: str):
    url_text = requests.get(url).text
    soup = bs4.BeautifulSoup(url_text, features="lxml")
    info_list = soup.select("body > div.resblock-list-container.clearfix > ul.resblock-list-wrapper")[0]
    for line_num in range(1, TERM_IN_PAGE_NUM + 1):
        try:
            info = info_list.select("li:nth-child({}) > div".format(line_num))[0]
        except:
            return
        name = info.select("div.resblock-name > a")[0].text
        pos1 = info.select("div.resblock-location > span:nth-child(1)")[0].text
        pos2 = info.select("div.resblock-location > span:nth-child(3)")[0].text
        pos3 = info.select("div.resblock-location > a")[0].text
        type = info.select("a > span:nth-child(1)")
        type = type[0].text if len(type) else ""
        area = re.search(r"\d+", info.select("div.resblock-area > span")[0].text)
        area = area[0] if area != None else ""
        main_price = info.select("div.resblock-price > div.main-price > span.number")[0].text
        price_unit = info.select("div.resblock-price > div > span.desc")[0].text
        if price_unit.find('套') != -1:
            total_price = re.search(r"\d+", main_price)[0]
            unit_price = str(int(float(total_price) * 10000 / int(area))) if len(area) else ""
        else:
            unit_price = main_price
            total_price = str(int(area) * int(unit_price) / 10000) if len(area) else ""
        result = [name, pos1, pos2, pos3, type, area, unit_price, total_price]
        result_list.append(result)


threads = []


for PAGE in range(1, PAGE_NUM + 1):
    URL = BASE_URL + "pg{}/".format(PAGE)
    thread = threading.Thread(target=one_page_pipeline, args=[URL])
    threads.append(thread)
    thread.start()

for thread in threads:
    thread.join()

result_list.sort()

with open("res1.csv", "w", newline="", encoding="utf8") as f:
    f_csv = csv.writer(f)
    f_csv.writerow(CSV_HEADERS)
    f_csv.writerows(result_list)
