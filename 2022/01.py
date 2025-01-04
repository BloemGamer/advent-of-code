import io, os
import time
from collections import Counter, defaultdict, deque

# file = defaultdict(None, {'file', 'amountlines', 'lengthlines'})

def main():
	# readfile("T1")
	global file; file = readfile("M")
	part1()
	part2()

def part1():
	j = 0
	maxssum = 0
	while(True):
		ssum = 0
		while(len(file[j])):
			ssum += int(file[j])
			j += 1
			if(j >= len(file)):
				break
		maxssum = max(maxssum, ssum)
		j += 1
		if(j >= len(file)):
			break
	print(maxssum)

def part2():
	pass



def readfile(what = "M"):
	day= __file__.split(os.sep)[-1].replace('.py', '')

	dir = os.path.dirname(os.path.realpath(__file__)) + os.sep

	tmp = dir + "txt" + os.sep + day + ".txt"
	if(what == "M"):
		with io.open(tmp, "r", encoding="utf-8") as data:
			running_test_data = False

	if(what == "T1"):
		tmp = tmp.replace('.', '.test1.')
		with io.open(tmp, "r", encoding="utf-8") as data:
			running_test_data = True

	if(what == "T2"):
		with io.open(tmp.replace('.','.test2.'), "r", encoding="utf-8") as data:
			running_test_data = True
		
	with io.open(tmp, "r", encoding="utf-8") as data:
		print(tmp)
		data_array_str = data.read().split("\n")
	return data_array_str


main()