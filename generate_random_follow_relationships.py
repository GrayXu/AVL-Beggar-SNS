# -*- coding: utf-8 -*-

import random as r
import csv
uText = "10w.u"
fText = "10w.r"

name = []

with open(uText, 'r', encoding="utf-8") as csvFile:
    read = csv.reader(csvFile)
    for item in read:
        # print(item[0])
        name.append(item[0])


# now name has all names

saveFile = open(fText, "w", encoding="utf-8")
for i in range(len(name)):
    followedArray = [name[i]]  # itself
    j = 1
    while j <= 5:
        followed = r.choice(name)
        if followed not in followedArray:
            rowText = name[i] + "," + r.choice(name) + "\n"
            saveFile.write(rowText)
            j = j + 1
            print(str(i*5+j))


saveFile.close()
