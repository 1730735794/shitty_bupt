from selenium import webdriver
from selenium.webdriver.common.action_chains import ActionChains
from selenium.webdriver.common.by import By
from selenium.webdriver.remote.webelement import WebElement
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions
from datetime import datetime, timedelta
import json


class Spider:
    def __init__(self):
        self.browser = webdriver.Chrome()
        self.country_infos = {}
        pass

    def get_country_urls(self):
        URL = "http://wx.wind.com.cn/WindSariWeb/sari/message.html?lan=cn"
        BUTTON_SELECTOR = "#root > div > div > div.layout-header > div.telecom > ul > li:nth-child(3) > div > a"
        TABLE_SELECTOR = "#root > div > div > div:nth-child(16) > div > div.w-card-body > span > div > div.w-table-wrapper > div > div > div > div > div > table > tbody"
        self.browser.get(URL)
        self.browser.find_element(By.CSS_SELECTOR, BUTTON_SELECTOR).click()   # 国外疫情
        WebDriverWait(self.browser, 60).until(
            expected_conditions.presence_of_element_located((By.CSS_SELECTOR, TABLE_SELECTOR + " > tr:nth-child(2)")))
        table: WebElement = self.browser.find_element(By.CSS_SELECTOR, TABLE_SELECTOR)
        print("reading urls")
        for i in range(100):
            line: WebElement = table.find_element(By.CSS_SELECTOR, "tr:nth-child({})".format(i + 1))
            country_name = line.find_element(By.CSS_SELECTOR, "td.w-table-cell.w-table-align-left.w-table-header-align-center > span").text
            url = line.find_element(By.CSS_SELECTOR, "td.w-table-cell.w-table-align-center.w-table-header-align-right.w-table-last-column > a").get_attribute("href")
            self.country_infos[country_name] = {}
            self.country_infos[country_name]["url"] = url
        print("urls reading completed")

    def get_vaccination_statics(self):
        URL = "http://wx.wind.com.cn/WindSariWeb/sari/message.html?lan=cn"
        BUTTON_SELECTOR = "#root > div > div > div.layout-header > div.telecom > ul > li:nth-child(2) > div > a"
        TABLE_SELECTOR = "#root > div > div > div.w-card.vaccin-detail.w-card-bordered.w-card-shadowed > div.w-card-body > div > div > div > div > div > div > table > tbody"
        self.browser.get(URL)
        self.browser.find_element(By.CSS_SELECTOR, BUTTON_SELECTOR).click()   # 疫苗接种
        WebDriverWait(self.browser, 60).until(
            expected_conditions.presence_of_element_located((By.CSS_SELECTOR, TABLE_SELECTOR + " > tr:nth-child(2)")))
        table: WebElement = self.browser.find_element(By.CSS_SELECTOR, TABLE_SELECTOR)
        print("reading vaccination statics")
        i = 1
        while True:
            try:
                line: WebElement = table.find_element(By.CSS_SELECTOR, "tr:nth-child({})".format(i))
            except:
                break
            country_name = line.find_element(By.CSS_SELECTOR, "td.w-table-cell.w-table-align-left.w-table-header-align-center > span").text
            vaccination_rate = line.find_element(By.CSS_SELECTOR, "td:nth-child(2) > span").text
            vaccination_dose_count = line.find_element(By.CSS_SELECTOR, "td:nth-child(4) > span").text
            if country_name in self.country_infos.keys():
                self.country_infos[country_name]["vaccination_rate"] = float(vaccination_rate)
                self.country_infos[country_name]["vaccination_dose_count"] = vaccination_dose_count
            i += 1
        print("vaccination statics reading completed")

    def get_country_statics(self, country_name, url):
        print("getting statics of {}".format(country_name))
        self.browser.get(url)
        MAP_TITLE_SELECTOR = "#root > div > div:nth-child(2) > div > div > div:nth-child(3) > div > div.w-card-head > div"
        NEW_INFECTED_SELECTOR = "#root > div > div:nth-child(2) > div > div > div:nth-child({}) > div > div.w-card-body > div:nth-child(2) > div"
        TOTAL_INFECTED_SELECTOR = "#root > div > div:nth-child(2) > div > div > div:nth-child({}) > div > div.w-card-body > div:nth-child(4) > div"
        TOTAL_DEATH_SELECTOR = "#root > div > div:nth-child(2) > div > div > div:nth-child({}) > div > div.w-card-body > div:nth-child(7) > div"
        if (self.browser.find_element(By.CSS_SELECTOR, MAP_TITLE_SELECTOR).text[:4] == "疫情地图"):    # 有地图! :(
            NEW_INFECTED_SELECTOR = NEW_INFECTED_SELECTOR.format(5)
            TOTAL_INFECTED_SELECTOR = TOTAL_INFECTED_SELECTOR.format(5)
            TOTAL_DEATH_SELECTOR = TOTAL_DEATH_SELECTOR.format(5)
            self.browser.execute_script('window.scrollBy(0, 800)')
        else:
            NEW_INFECTED_SELECTOR = NEW_INFECTED_SELECTOR.format(4)
            TOTAL_INFECTED_SELECTOR = TOTAL_INFECTED_SELECTOR.format(4)
            TOTAL_DEATH_SELECTOR = TOTAL_DEATH_SELECTOR.format(4)
            self.browser.execute_script('window.scrollBy(0, 400)')
        WebDriverWait(self.browser, 60).until(
            expected_conditions.presence_of_element_located((By.CSS_SELECTOR, NEW_INFECTED_SELECTOR)))
        new_infected_canvas: WebElement = self.browser.find_element(By.CSS_SELECTOR, NEW_INFECTED_SELECTOR)
        NEW_INFECTED_WIDTH = new_infected_canvas.size["width"]
        NEW_INFECTED_HEIGHT = new_infected_canvas.size["height"]
        self.country_infos[country_name]["new_infected"] = {}
        for x_offset in range(int(NEW_INFECTED_WIDTH / 2)):
            mouse_move: ActionChains = ActionChains(self.browser)
            mouse_move.move_to_element_with_offset(new_infected_canvas, NEW_INFECTED_WIDTH - x_offset, int(NEW_INFECTED_HEIGHT / 2))
            mouse_move.perform()
            if len(new_infected_canvas.text) == 0:
                continue
            INFO = new_infected_canvas.text.split('\n')
            new_infected_count = INFO[1][6:].replace(',', '')
            self.country_infos[country_name]["new_infected"][INFO[0]] = int(new_infected_count) if new_infected_count.isdigit() else 0
            if INFO[0] == (datetime.now() + timedelta(days=-15)).strftime("%m-%d"):
                break

        WebDriverWait(self.browser, 60).until(
            expected_conditions.presence_of_element_located((By.CSS_SELECTOR, TOTAL_INFECTED_SELECTOR)))
        total_infected_canvas: WebElement = self.browser.find_element(By.CSS_SELECTOR, TOTAL_INFECTED_SELECTOR)
        TOTAL_INFECTED_WIDTH = total_infected_canvas.size["width"]
        TOTAL_INFECTED_HEIGHT = total_infected_canvas.size["height"]
        for x_offset in range(int(TOTAL_INFECTED_WIDTH / 2)):
            mouse_move: ActionChains = ActionChains(self.browser)
            mouse_move.move_to_element_with_offset(total_infected_canvas, TOTAL_INFECTED_WIDTH - x_offset, int(TOTAL_INFECTED_HEIGHT / 2))
            mouse_move.perform()
            if len(total_infected_canvas.text) != 0:
                INFO = total_infected_canvas.text.split('\n')
                total_infected_count = INFO[1][6:].replace(',', '')
                self.country_infos[country_name]["total_infected"] = int(total_infected_count) if total_infected_count.isdigit() else 0
                break

        WebDriverWait(self.browser, 60).until(
            expected_conditions.presence_of_element_located((By.CSS_SELECTOR, TOTAL_DEATH_SELECTOR)))
        total_death_canvas: WebElement = self.browser.find_element(By.CSS_SELECTOR, TOTAL_DEATH_SELECTOR)
        TOTAL_DEATH_WIDTH = total_death_canvas.size["width"]
        TOTAL_DEATH_HEIGHT = total_death_canvas.size["height"]
        self.country_infos[country_name]["total_death"] = {}
        for x_offset in range(int(TOTAL_DEATH_WIDTH / 2)):
            mouse_move: ActionChains = ActionChains(self.browser)
            mouse_move.move_to_element_with_offset(total_death_canvas, TOTAL_DEATH_WIDTH - x_offset, int(TOTAL_DEATH_HEIGHT / 2))
            mouse_move.perform()
            if len(total_death_canvas.text) != 0:
                INFO = total_death_canvas.text.split('\n')
                total_death_count = INFO[1][6:].replace(',', '')
                self.country_infos[country_name]["total_death"] = int(total_death_count) if total_death_count.isdigit() else 0
                break

    def get_country_population(self):
        URL = "https://www.phb123.com/city/renkou/rk_{}.html"
        TABLE_SELECTOR = "body > div.wrap.mar1.clearfix > div.sp-l > div.mar1 > table > tbody"
        page_num = 1
        while True:
            self.browser.get(URL.format(page_num))
            table: WebElement = self.browser.find_element(By.CSS_SELECTOR, TABLE_SELECTOR)
            try:    # 无数据则跳出
                table.find_element(By.CSS_SELECTOR, "tr:nth-child(2)")
            except:
                break
            line_num = 2
            while True:
                try:
                    line: WebElement = table.find_element(By.CSS_SELECTOR, "tr:nth-child({})".format(line_num))
                except:
                    break
                country_name = line.find_element(By.CSS_SELECTOR, "td:nth-child(2) > a > p").text
                population =  line.find_element(By.CSS_SELECTOR, "td:nth-child(3)").text.replace(',', '')
                if country_name in self.country_infos.keys():
                    self.country_infos[country_name]["population"] = int(population)
                line_num += 1
            page_num += 1


    def write_json(self, path: str):
        print("writing to json file")
        with open(path, "w", encoding="utf8") as f:
            json.dump(self.country_infos, f, indent=4, ensure_ascii=False)

    def read_json(self, path: str):
        print("reading from json file")
        with open(path, "r", encoding="utf8") as f:
            self.country_infos = json.load(f)


if __name__ == "__main__":
    spider = Spider()
    spider.get_country_urls()
    spider.get_country_population()
    spider.get_vaccination_statics()
    for country_name, country_info in spider.country_infos.items():
        spider.get_country_statics(country_name, country_info["url"])
        spider.write_json("res.json")
