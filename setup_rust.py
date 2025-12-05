#!/usr/bin/env python3

import re
import sys
import pathlib

def setup_rust(year: int, day: int) -> None:
	TEMPLATE_FILE: str = "template/template.rs"
	rust_file: str = f"{year}/{day:02d}.rs/"
	with open("Cargo.toml", "r") as cargo_toml:
		file = cargo_toml.read()
	reg: str = '\npath = \".*\"\n'
	match = re.match(reg, file)
	print(match)
	new_cargo_toml: str = re.sub(reg, f'\npath = \"{year}/{day:02d}.rs\"\n', file)
	print(new_cargo_toml)
	with open("Cargo.toml", "w") as cargo_toml:
		cargo_toml.write(new_cargo_toml)

	if pathlib.Path(rust_file).exists():
		return
	if not pathlib.Path(TEMPLATE_FILE).exists():
		return
	with open(f"{year}/{day:02d}.rs", "r") as file:
		new_rust_file: str = file.read()

	new_rust_file.replace("{day}", f"{day:02}")
	new_rust_file.replace("{year}", f"{year}")

	with open(rust_file, "w+") as file:
		file.write(new_rust_file)

if __name__ == "__main__":
	setup_rust(int(sys.argv[1]), int(sys.argv[2]))
