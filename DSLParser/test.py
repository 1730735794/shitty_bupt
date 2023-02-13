import requests
import json

def test_func(num1, num2):
    output=[]
    res=requests.post("http://127.0.0.1:8000/reply",json={'msg':str(num1)})
    jsonobj = json.loads(res.text)
    # 返回值格式解析
    output.extend(str(jsonobj['msg'][0]).split('<br/>')[:-1])
    res=requests.post("http://127.0.0.1:8000/reply",json={'msg':str(num2)})
    jsonobj = json.loads(res.text)
    output.extend(str(jsonobj['msg'][0]).split('<br/>')[:-1])
    # test1
    if output[0] != str(num1):
        return False
    # test2
    if int(str(num1)) >= 1 and int(str(num1)) <= 3:
        if output[1] != str(int(str(num1)) + 1):
            return False
    elif int(str(num1)) >= 4 and int(str(num1)) <= 5:
        if output[1] != str(int(str(num1)) - 1):
            return False
    elif output[1] != str(num1):
        return False
    # test3
    if str(num1) == str(num2) and output[2] != "success":
        return False
    if str(num1) != str(num2) and output[2] != "fail":
        return False
    # passed
    return True


test_num = 0
pass_num = 0
for num1 in range(10):
    for num2 in range(10):
        test_num += 1
        if test_func(num1, num2) == True:
            pass_num += 1
        print("\r({}/{}) cases tested".format(test_num, 100), end="")
print("\n{}/{} testcases passed".format(pass_num, test_num))
