import io, os
import time
import itertools
from collections import Counter, defaultdict, deque
from adventofcode import *

def main() -> None:
	global file
	numbers = list()
	file = fix_file(__file__, "M")
	numbers = [int(f) for f in file if f != ""]
	print(sum([(b - a) >= 0 for a, b in itertools.pairwise(numbers)])) # part 1 
	s = [(numbers[i] + numbers[i + 1] + numbers[i + 2])for i in range(0, len(numbers) - 2)]
	print(sum([(b - a) > 0 for a, b in itertools.pairwise(s)])) # part 2


main()