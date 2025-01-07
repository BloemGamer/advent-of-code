import io, os
import time
from collections import Counter, defaultdict, deque
from adventofcode import *

def main() -> None:
	global file
	file = fix_file(__file__, "M")
	answer: int = 0; answer2: int = 0
	for f in file:
		pair = fill_pair(f)
		if(pair[0][0] >= pair[1][0] and pair[0][1] <= pair[1][1]):
			answer += 1
		elif(pair[0][0] <= pair[1][0] and pair[0][1] >= pair[1][1]):
			answer += 1
		if(pair[0][1] >= pair[1][0] and pair[0][0] <= pair[1][1]):
			answer2 += 1
		elif(pair[0][1] <= pair[1][0] and pair[0][0] >= pair[1][1]):
			answer2 += 1
	print("Part 1:", answer)
	print("Part 2:", answer2)

def fill_pair(input: str):
	dif_pairs = input.split(',')
	return([(int(d.split('-')[0]),int(d.split('-')[1])) for d in dif_pairs])


main()