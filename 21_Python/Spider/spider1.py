import requests
import bs4
import csv
import threading

# config here
REGION_NAMES = {"东城": "dongcheng", "西城": "xicheng",
                "海淀": "haidian", "朝阳": "chaoyang"}
PAGE_NUM = 5
TERM_IN_PAGE_NUM = 30


BASE_URL = "https://bj.lianjia.com/ershoufang/"
CSV_HEADERS = ["区域", "楼盘名称", "总价", "面积", "单价"]

result_list = []


def one_page_pipeline(region_name: str, url: str):
    URL_TEXT = requests.get(url).text
    print("target url = \"{}\"".format(url))
    soup = bs4.BeautifulSoup(URL_TEXT, features="lxml")
    INFO_LIST = soup.select("#content > div.leftContent > ul")[0]
    for CHILD in range(1, TERM_IN_PAGE_NUM + 1):
        INFO = INFO_LIST.select(
            "li:nth-child({}) > div.info.clear".format(CHILD))[0]
        NAME = INFO.select("div.title > a")[0].text
        TOTAL_PRICE = INFO.select("div.priceInfo > div.totalPrice.totalPrice2 > span")[
            0].text + "万"
        AREA = INFO.select("div.address > div")[0].text.split('|')[1][1:-1]
        UNIT_PRICE = INFO.select(
            "div.priceInfo > div.unitPrice > span")[0].text
        RESULT = [region_name, NAME, TOTAL_PRICE, AREA, UNIT_PRICE]
        result_list.append(RESULT)


threads = []

for REGION_NAME, REGION_PINYIN in REGION_NAMES.items():
    for PAGE in range(1, PAGE_NUM + 1):
        URL = BASE_URL + REGION_PINYIN + "/pg{}/".format(PAGE)
        thread = threading.Thread(
            target=one_page_pipeline, args=[REGION_NAME, URL])
        threads.append(thread)
        thread.start()

for thread in threads:
    thread.join()

result_list.sort()

with open("res1.csv", "w", newline="", encoding="utf8") as f:
    f_csv = csv.writer(f)
    f_csv.writerow(CSV_HEADERS)
    f_csv.writerows(result_list)
