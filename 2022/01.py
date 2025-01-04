import io, os
import time
from collections import Counter, defaultdict, deque

def main():
	global file; file = readfile("T1")
	j = 0
	maxssum = []
	while(True):
		ssum = 0
		while(len(file[j])):
			ssum += int(file[j])
			j += 1
			if(j >= len(file)):
				break
		maxssum.append(ssum)
		j += 1
		if(j >= len(file)):
			break
	maxssum.sort(reverse = True)
	print("Part 1:", maxssum[0])
	print("Part 2:", maxssum[0] + maxssum[1] + maxssum[2])

def readfile(what = "M"):
	day= __file__.split(os.sep)[-1].replace('.py', '')
	dir = os.path.dirname(os.path.realpath(__file__)) + os.sep
	tmp = dir + "txt" + os.sep + day + ".txt"
	
	if(what == "T1"):
		tmp = tmp.replace('.', '.test1.')
		
	if(what == "T2"):
		tmp = tmp.replace('.', '.test2.')
		
	with io.open(tmp, "r", encoding="utf-8") as data:
		print(tmp)
		data_array_str = data.read().split("\n")
	return data_array_str


main()