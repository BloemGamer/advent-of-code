import os, sys
import requests
import dotenv
import pathlib
import shutil
import datetime

today = datetime.date.today()

def get_aoc_data(DAY = today.day, YEAR = today.year, FILE_TYPE = None):
    # check if the given day is valid, and if no day is given, if today is valid
    if(YEAR > today.year):
        print(f"this year is not ready: {YEAR} > {today.year}")
        exit(1)
    if(today.year == YEAR):
        if(today.month != 12):
            print("Not december")
            exit(1)
        if(DAY > today.day):
            print(f"this day is not ready: {DAY} > {today.day}")
            exit(1)
    if(DAY > 25):
        print("After day 25")
        exit(1)

    # make directories
    os.makedirs(f"{YEAR}", exist_ok=True)
    os.makedirs(f"{YEAR}{os.sep}txt", exist_ok=True)
    pathlib.Path(f"{YEAR}/txt/{DAY:02}.test1.txt").touch(exist_ok=True)
    pathlib.Path(f"{YEAR}/txt/{DAY:02}.test2.txt").touch(exist_ok=True)

    # load the cookie/session thing
    dotenv.load_dotenv()
    SESSION = os.getenv("AOC_SESSION")
    assert SESSION is not None, "Missing AOC_SESSION in .env"
    url = f"https://adventofcode.com/{YEAR}/day/{DAY}/input"
    cookies = {"session": SESSION}

    # ask for the input from the server
    response = requests.get(url, cookies=cookies)
    if response.ok:
        with open(f"{YEAR}/txt/{DAY:02}.txt", "w+") as f:
            f.write(response.text)
        print(f"Input downloaded: {YEAR}/txt/{DAY:02}.txt")
    else:
        print("Failed to download input:", response.status_code, response.text)

    # copy the standard file, if that is asked, and exists, else, just make the file
    if(FILE_TYPE != None):
        exit(0)
    if pathlib.Path(f"{YEAR}{os.sep}{DAY}.{FILE_TYPE}").exists():
        exit(0)
    elif pathlib.Path(f"standard/{FILE_TYPE}.{FILE_TYPE}").exists():
        shutil.copyfile(f"standard/{FILE_TYPE}.{FILE_TYPE}", f"{YEAR}{os.sep}{DAY}.{FILE_TYPE}")
    else:
        pathlib.Path(f"{YEAR}{os.sep}{DAY}.{FILE_TYPE}").touch(exist_ok=True)


if __name__ == "__main__":
    if(len(sys.argv) == 1):
        get_aoc_data();
        exit(0)
    day = today.day
    year = today.year
    file_type = None
    i = 1
    if(sys.argv[1][0] == '-'):
        while i < len(sys.argv):
            if(sys.argv[i][0] != '-'):
                print("not a good argument, expected an argument with '-' at the start")
                exit(1)
            j = i
            for a in sys.argv[j][1:]:
                i += 1
                match a:
                    case 'y':
                        year = int(sys.argv[i])
                    case 'd':
                        day = int(sys.argv[i])
                    case 'f':
                        file_type = sys.argv[i]
                    case _:
                        print("this is not a good argument")
                        exit(1)
            i += 1

    else:
        if(len(sys.argv) >= 3):
            file_type = sys.argv[3]
        if(len(sys.argv) > 3):
            year = int(sys.argv[1])
            day = int(sys.argv[2])
        elif len(sys.argv) == 2:
            file_type = sys.argv[1]

    print("day = ", day)
    print("year = ", year)
    print("file_type = ", file_type)


    get_aoc_data(day, year, file_type)
