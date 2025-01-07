import io, os
import time
from collections import Counter, defaultdict, deque
from adventofcode import *

def main() -> None:
	global file
	file = fix_file(__file__, "M")
	answer: int = 0
	for f in file:
		answer += priority(in_both_compartments(f))
	print("Part 1:", answer)

	answer = 0
	for i in range(0, len(file), 3):
		answer += priority(on_three_elves(file[i], file[i + 1], file[i + 2]))
	print("Part 2:", answer)

def in_both_compartments(bag: str) -> str:
	compartment1: set[str] = {}; compartment2: set[str] = {}
	compartment1 = set(); compartment2 = set()
	size_bag: int = len(bag)
	
	for i, x in enumerate(bag):
		if(i < size_bag / 2):
			compartment1.add(x)
		else:
			compartment2.add(x)
	tmp = list(compartment1 & compartment2)
	return tmp[0]

def priority(item: str) -> int:
	if(item == item.lower()):
		return ord(item) - ord('a') + 1
	elif(item == item.upper()):
		return ord(item) - ord('A') + 27
	else:
		assert(False)

def on_three_elves(bag1: str, bag2: str, bag3: str) -> str:
	tmp: list[str] = list(set(bag1) & set(bag2) & set(bag3))
	return tmp[0]

main()