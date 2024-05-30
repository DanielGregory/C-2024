import random

outputList = []
for i in range(129):
	outputList.append(i)
	
random.shuffle(outputList)

output = open("test3.csv","w")
for i in outputList:
	output.write(str(i)+"\n")