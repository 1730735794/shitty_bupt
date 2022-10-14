import os
import platform

TMP_INPUT_FILE = "./in.txt"
TMP_OUTPUT_FILE = "./out.txt"


def test_pipeline(s1, s2):
    with open(TMP_INPUT_FILE, "w") as f:
        f.write(str(s1) + "\n")
        f.write(str(s2) + "\n")
    os.system(
        "python3 ./DSLParser.py ./test.script < {} > {}".format(TMP_INPUT_FILE, TMP_OUTPUT_FILE))
    with open(TMP_OUTPUT_FILE, "r") as f:
        res = f.read().split('\n')
    # delete tmpfiles
    os.remove(TMP_INPUT_FILE)
    os.remove(TMP_OUTPUT_FILE)
    # test1
    if res[0] != str(s1):
        return False
    # test2
    if int(str(s1)) >= 1 and int(str(s1)) <= 5:
        if res[1] != str(int(str(s1)) * 2):
            return False
    elif res[1] != "other":
        return False
    # test3
    if str(s1) == str(s2) and res[2] != "same":
        return False
    if str(s1) != str(s2) and res[2] != "not same":
        return False
    # passed
    return True


total_test_count = 0
total_passed_count = 0
for s1 in range(10):
    for s2 in range(10):
        total_test_count += 1
        if test_pipeline(s1, s2) == True:
            total_passed_count += 1
        print("\r({}/{}) cases tested".format(total_test_count, 100), end="")
print("\n{}/{} testcases passed".format(total_passed_count, total_test_count))
