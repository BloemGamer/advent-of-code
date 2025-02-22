import io, os

def fix_file(file, what: str = "M") -> list[str]:
	day= file.split(os.sep)[-1].replace('.py', '')
	dir = os.path.dirname(os.path.realpath(file)) + os.sep
	filename = dir + "txt" + os.sep + day + ".txt"
	filename1 = filename.replace('.', '.test1.')
	filename2 = filename.replace('.', '.test2.')
	with open(filename, "a+") as data:
		pass
	with open(filename1, "a+") as data:
		pass
	with open(filename2, "a+") as data:
		pass

	match what:
		case "M":
			tmp = filename
		case "T1":
			tmp = filename1
		case "T2":
			tmp = filename2

	with io.open(tmp, "r", encoding="utf-8") as data:
		print(tmp)
		data_array_str = data.read().split("\n")
	return data_array_str