import io, os
import time
import itertools
from collections import Counter, defaultdict, deque
from adventofcode import *

def main() -> None:
	numbers = [int(f) for f in fix_file(__file__, "M") if f != ""]
	print(sum([(b - a) >= 0 for a, b in itertools.pairwise(numbers)])) # part 1 
	print(sum([(b - a) > 0 for a, b in itertools.pairwise([sum((numbers[i:i + 3]))for i in range(0, len(numbers) - 2)])])) # part 2


main()