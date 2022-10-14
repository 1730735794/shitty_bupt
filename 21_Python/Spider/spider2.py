import requests
import bs4
import json

HEADERS = {
    "accept": "application/json, text/plain, */*",
    "accept-encoding": "gzip, deflate, br",
    "accept-language": "zh",
    "app-name": "xtzx",
    "cache-control": "no-cache",
    "django-language": "zh",
    "pragma": "no-cache",
    "referer": "https://www.xuetangx.com/university/all",
    "sec-ch-ua": "\"Google Chrome\";v=\"95\", \"Chromium\";v=\"95\", \";Not A Brand\";v=\"99\"",
    "sec-ch-ua-mobile": "?0",
    "sec-ch-ua-platform": "\"Linux\"",
    "sec-fetch-dest": "empty",
    "sec-fetch-mode": "cors",
    "sec-fetch-site": "same-origin",
    "terminal-type": "web",
    "user-agent": "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/95.0.4638.54 Safari/537.36",
    "x-client": "web",
    "xtbz": "xt"
}

result_list = []
page_num = 1
while (True):
    webpage = requests.get(
        "https://www.xuetangx.com/api/v1/lms/get_org_list/?page={}&page_size=100&appid=10000&query=&order=3".format(page_num), headers=HEADERS)
    INFO_JSON = json.loads(webpage.text)
    if len(INFO_JSON["data"]["org_list"]) == 0:
        break
    for INFO in INFO_JSON["data"]["org_list"]:
        result = {}
        result["name"] = INFO["name"]
        result["num"] = INFO["product_num"]
        result_list.append(result)
    page_num += 1
with open("res2.json", "w",encoding="utf8") as f:
    json.dump(result_list, f, ensure_ascii=False, indent=4)
