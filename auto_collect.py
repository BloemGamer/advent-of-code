import os, sys
import requests
import dotenv
import pathlib
import shutil
import datetime

today = datetime.date.today()

# check if the given day is valid, and if no day is given, if today is valid
if(len(sys.argv) <= 2):
    if(today.month != 12):
        print("Not december")
        exit(1)
    if(today.day > 25):
        print("After day 25")
        exit(1)
    YEAR = today.year
    DAY = today.day
else:
    YEAR = int(sys.argv[1])
    DAY = int(sys.argv[2])
    if(YEAR > today.year):
        print(f"this year is not ready: {YEAR} > {today.year}")
        exit(1)
    if(today.year == YEAR and DAY > today.day):
        print(f"this day is not ready: {DAY} > {today.day}")
        exit(1)
    if(today.day > 25):
        print("After day 25")
        exit(1)

# make directories
os.makedirs(f"{YEAR}", exist_ok=True)
os.makedirs(f"{YEAR}{os.sep}txt", exist_ok=True)
pathlib.Path(f"{YEAR}/txt/{DAY:02}.test1.txt").touch(exist_ok=True)
pathlib.Path(f"{YEAR}/txt/{DAY:02}.test2.txt").touch(exist_ok=True)

# fix the cookie/session thing
dotenv.load_dotenv()
SESSION = os.getenv("AOC_SESSION")
url = f"https://adventofcode.com/{YEAR}/day/{DAY}/input"
assert SESSION is not None, "Missing AOC_SESSION in .env"
cookies = {"session": SESSION}

# ask for the input from the server
response = requests.get(url, cookies=cookies)
if response.ok:
    with open(f"{YEAR}/txt/{DAY:02}.txt", "w+") as f:
        f.write(response.text)
    print(f"Input downloaded: {YEAR}/txt/{DAY:02}.txt")
else:
    print("Failed to download input:", response.status_code)

# copy the standard file, if that is asked, and exists, else, just make the file
if(len(sys.argv) <= 3 and len(sys.argv) != 2):
    exit(0)
if pathlib.Path(f"{YEAR}{os.sep}{DAY}.{sys.argv[3]}").exists():
    exit(0)
elif pathlib.Path(f"standard/{sys.argv[3]}.{sys.argv[3]}").exists():
    shutil.copyfile("standard/py.py", f"{YEAR}{os.sep}{DAY}.{sys.argv[3]}")
else:
    pathlib.Path(f"{YEAR}{os.sep}{DAY}.{sys.argv[3]}").touch(exist_ok=True)
