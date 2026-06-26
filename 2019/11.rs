#![allow(clippy::needless_return)]

use std::collections::HashMap;

#[cfg(test)]
use aoc::println;

enum Direction
{
	Up,
	Down,
	Left,
	Right,
}

impl Direction
{
	fn turn_right(self) -> Direction
	{
		match self {
			Direction::Up => Direction::Right,
			Direction::Right => Direction::Down,
			Direction::Down => Direction::Left,
			Direction::Left => Direction::Up,
		}
	}

	fn turn_left(self) -> Direction
	{
		match self {
			Direction::Up => Direction::Left,
			Direction::Left => Direction::Down,
			Direction::Down => Direction::Right,
			Direction::Right => Direction::Up,
		}
	}
}


struct IntCode
{
	code: Vec<i64>,
	places: [usize; 4],
	i: usize,
	base_offset: i64,
	input: i64,
}

impl IntCode
{
	fn new(code: impl Iterator<Item = i64>, input: i64) -> IntCode
	{
		let mut code: Vec<i64> = code.collect();
		code.resize(code.len() + 10_000, 0);
		return IntCode {
			code,
			places: [0; 4],
			i: 0,
			base_offset: 0,
			input
		};
	}
}

impl Iterator for IntCode
{
	type Item = i64;

	fn next(&mut self) -> Option<Self::Item> {
		while self.i < self.code.len()
		{
			match self.code[self.i] % 100
			{
				1 => { get_places(self.i, self.code.as_mut_slice(), &mut self.places, self.base_offset); ic_add(&mut self.i, self.code.as_mut_slice(), &self.places) },
				2 => { get_places(self.i, self.code.as_mut_slice(), &mut self.places, self.base_offset); ic_mult(&mut self.i, self.code.as_mut_slice(), &self.places) },
				3 => { get_places(self.i, self.code.as_mut_slice(), &mut self.places, self.base_offset); ic_input(&mut self.i, self.code.as_mut_slice(), &self.places, self.input) },
				4 => { get_places(self.i, self.code.as_mut_slice(), &mut self.places, self.base_offset); return Some(ic_output(&mut self.i, self.code.as_mut_slice(), &self.places)) },
				5 => { get_places(self.i, self.code.as_mut_slice(), &mut self.places, self.base_offset); ic_jump(&mut self.i, self.code.as_mut_slice(), &self.places, true) },
				6 => { get_places(self.i, self.code.as_mut_slice(), &mut self.places, self.base_offset); ic_jump(&mut self.i, self.code.as_mut_slice(), &self.places, false) },
				7 => { get_places(self.i, self.code.as_mut_slice(), &mut self.places, self.base_offset); ic_cmp(&mut self.i, self.code.as_mut_slice(), &self.places, |a, b| a < b) },
				8 => { get_places(self.i, self.code.as_mut_slice(), &mut self.places, self.base_offset); ic_cmp(&mut self.i, self.code.as_mut_slice(), &self.places, |a, b| a == b) },
				9 => { get_places(self.i, self.code.as_mut_slice(), &mut self.places, self.base_offset); ic_adj_offset(&mut self.i, self.code.as_mut_slice(), &self.places, &mut self.base_offset) },

				99 => break,
				_ => unreachable!()
			}
		}
		return None;
	}
}

fn main()
{
	let file: Vec<String> = aoc::read_file("2019", "11", aoc::WhichFile::Main);
	part1(&file);
	part2(&file);
}

fn part1(file: &[String])
{
	let mut location: (i64, i64) = (0, 0);
	let mut panels: HashMap<(i64, i64), i64> = HashMap::new();
	let mut ic: IntCode = IntCode::new(file[0].split(',').map(|x| x.parse::<i64>().unwrap()), 0);
	let mut direction: Direction = Direction::Up;

	while let Some(colour) = ic.next()
	{
		let Some(dir) = ic.next() else { panic!() };
		panels.insert(location, colour);
		match dir {
			0 => direction = direction.turn_left(),
			1 => direction = direction.turn_right(),
			_ => unreachable!(),
		}
		location = match direction {
			Direction::Up    => (location.0 - 1, location.1),
			Direction::Down  => (location.0 + 1, location.1),
			Direction::Left  => (location.0, location.1 - 1),
			Direction::Right => (location.0, location.1 + 1),
		};
		ic.input = *panels.get(&location).unwrap_or(&0);
	}

	println!("Part 1: {}", panels.len());
}

fn part2(file: &[String])
{
	let mut location: (i64, i64) = (0, 0);
	let mut panels: HashMap<(i64, i64), i64> = HashMap::new();
	let mut ic: IntCode = IntCode::new(file[0].split(',').map(|x| x.parse::<i64>().unwrap()), 1);
	let mut direction: Direction = Direction::Up;

	while let Some(colour) = ic.next()
	{
		let Some(dir) = ic.next() else { panic!() };
		panels.insert(location, colour);
		match dir {
			0 => direction = direction.turn_left(),
			1 => direction = direction.turn_right(),
			_ => unreachable!(),
		}
		location = match direction {
			Direction::Up    => (location.0 - 1, location.1),
			Direction::Down  => (location.0 + 1, location.1),
			Direction::Left  => (location.0, location.1 - 1),
			Direction::Right => (location.0, location.1 + 1),
		};
		ic.input = *panels.get(&location).unwrap_or(&0);
	}

	let mut max_y: i64 = i64::MIN;
	let mut min_y: i64 = i64::MAX;
	let mut max_x: i64 = i64::MIN;
	let mut min_x: i64 = i64::MAX;

	for (y, x) in panels.keys()
	{
		max_y = max_y.max(*y);
		min_y = min_y.min(*y);
		max_x = max_x.max(*x);
		min_x = min_x.min(*x);
	}
	let mut map: Vec<Vec<char>> = vec![vec!['.'; (max_x - min_x + 1) as usize]; (max_y - min_y + 1) as usize];
	for ((y, x), v) in panels
	{
		if v == 1
		{
			map[(y + min_y) as usize][(x + min_x) as usize] = '#';
		}
	}

	for row in map {
		for cell in row {
			print!("{}", cell);
		}
		println!();
	}

}


fn get_places(i: usize, intcode: &mut [i64], places: &mut [usize; 4], base_offset: i64)
{
	places[0] = intcode[i] as usize;

	let mut par: i64 = intcode[i] / 100;
	places[1] = if par % 10 == 0 { intcode[i + 1] as usize } else if par % 10 == 1 { i + 1 } else { (intcode[i + 1] + base_offset) as usize };

	par /= 10;
	places[2] = if par % 10 == 0 { intcode[i + 2] as usize } else if par % 10 == 1 { i + 2 } else { (intcode[i + 2] + base_offset) as usize };

	par /= 10;
	places[3] = if par % 10 == 0 { intcode[i + 3] as usize } else if par % 10 == 1 { i + 3 } else { (intcode[i + 3] + base_offset) as usize };
}

fn ic_add(i: &mut usize, intcode: &mut [i64], places: &[usize; 4])
{
	intcode[places[3]] = intcode[places[1]] + intcode[places[2]];
	*i += 4;
}

fn ic_mult(i: &mut usize, intcode: &mut [i64], places: &[usize; 4])
{
	intcode[places[3]] = intcode[places[1]] * intcode[places[2]];
	*i += 4;
}

fn ic_input(i: &mut usize, intcode: &mut [i64], places: &[usize; 4], input: i64)
{
	intcode[places[1]] = input;
	*i += 2;
}


fn ic_output(i: &mut usize, intcode: &mut [i64], places: &[usize; 4]) -> i64
{
	// print!("{} ", intcode[places[1]]);
	*i += 2;
	return intcode[places[1]];
}


fn ic_jump(i: &mut usize, intcode: &mut [i64], places: &[usize; 4], jump_if: bool)
{
	if (intcode[places[1]] != 0) == jump_if
	{
		*i = intcode[places[2]] as usize;
	}
	else
	{
		*i += 3;
	}
}

fn ic_cmp<F>(i: &mut usize, intcode: &mut [i64], places: &[usize; 4], cmp: F)
where F: Fn(i64, i64) -> bool,
{
	if cmp(intcode[places[1]], intcode[places[2]])
	{
		intcode[places[3]] = 1;
	}
	else
	{
		intcode[places[3]] = 0;
	}
	*i += 4;
}

fn ic_adj_offset(i: &mut usize, intcode: &mut [i64], places: &[usize; 4], base_offset: &mut i64)
{
	*base_offset += intcode[places[1]];
	*i += 2;
}
