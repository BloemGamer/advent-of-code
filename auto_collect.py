import os, sys
import requests
import dotenv
import pathlib
import shutil

YEAR = int(sys.argv[1])
DAY = int(sys.argv[2])

os.makedirs(f"{YEAR}", exist_ok=True)
os.makedirs(f"{YEAR}{os.sep}txt", exist_ok=True)
pathlib.Path(f"{YEAR}/txt/{DAY:02}.test1.txt").touch(exist_ok=True)
pathlib.Path(f"{YEAR}/txt/{DAY:02}.test2.txt").touch(exist_ok=True)


# Your session token here (store securely!)
dotenv.load_dotenv()
SESSION = os.getenv("AOC_SESSION")
url = f"https://adventofcode.com/{YEAR}/day/{DAY}/input"
assert SESSION is not None, "Missing AOC_SESSION in .env"
cookies = {"session": SESSION}

response = requests.get(url, cookies=cookies)
if response.ok:
    with open(f"{YEAR}/txt/{DAY:02}.txt", "w+") as f:
        f.write(response.text)
    print(f"Input downloaded: {YEAR}/txt/{DAY:02}.txt")
else:
    print("Failed to download input:", response.status_code)

if(len(sys.argv) < 3):
    exit(0)

if pathlib.Path(f"{YEAR}{os.sep}{DAY}.{sys.argv[3]}").exists():
    exit(0)
elif pathlib.Path(f"standard/{sys.argv[3]}.{sys.argv[3]}").exists():
    shutil.copyfile("standard/py.py", f"{YEAR}{os.sep}{DAY}.{sys.argv[3]}")
else:
    pathlib.Path(f"{YEAR}{os.sep}{DAY}.{sys.argv[3]}").touch(exist_ok=True)
