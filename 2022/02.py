import io, os
import time
from collections import Counter, defaultdict, deque


def main():
	global file; file = readfile("M")
	part1()
	part2()

def part1():
	ssum = 0
	for f in file:
		game = f.split(" ")
		ssum += who_whon(game) + what(game)
	print("Part 1:", ssum)

def part2():
	ssum = 0
	for f in file:
		game = f.split(" ")
		ssum += who_whon2(game) + what2(game)
	print("Part 2:", ssum)

		
def who_whon(game):
	if(ord(game[0][0]) + ord('X') == ord(game[1][0]) + ord('A')):
		return 3
	if(game[0] == "A" and game[1] == "Z"):
		return 0
	if(game[0] == "B" and game[1] == "X"):
		return 0
	if(game[0] == "C" and game[1] == "Y"):
		return 0
	return 6

def what(game):
	match game[1]:
		case "X":
			return 1
		case "Y":
			return 2
		case "Z":
			return 3
	
def who_whon2(game):
	match game[1]:
		case "X":
			return 0
		case "Y":
			return 3
		case "Z":
			return 6

def what2(game):	
	match who_whon2(game):
		case 0:
			return (ord(game[0][0]) - ord('A') - 1) % 3 + 1
		case 3:
			return ord(game[0][0]) - ord('A') + 1
		case 6:
			return (ord(game[0][0]) - ord('A') + 1) % 3 + 1
		


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