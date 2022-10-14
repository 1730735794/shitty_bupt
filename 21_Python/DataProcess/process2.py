import pandas

pandas.set_option("display.unicode.east_asian_width", True)
data = pandas.read_csv("BeijingPM20100101_20151231.csv")

data_2015 = data[data["year"] == 2015]
data_2015.to_csv("2015.csv")
for index, row in data_2015.iteritems():
    if row.count() != len(data_2015):
        print("column {} has {} null values".format(index, len(data_2015) - row.count()))
data_2015 = data_2015.interpolate()
data_2015 = data_2015.fillna({"cbwd": "unknown"})
data_2015.to_csv("2015_no_null.csv")