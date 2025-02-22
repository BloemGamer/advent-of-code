import io, os
import time
import itertools
from collections import Counter, defaultdict, deque
from adventofcode import *


def main() -> None:
	global file
	numbers = list()
	answer = 0
	file = fix_file(__file__, "T1")
	for f in file:
		numbers.append(int(f))
	diff = [b - a for a, b in itertools.pairwise(numbers)]
	for d in diff:
		if(d >= 0):
			answer += 1
	print(answer)
main()