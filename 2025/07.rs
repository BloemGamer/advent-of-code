#[cfg(test)]
use aoc::{println};

use std::collections::HashMap;
use std::cell::RefCell;

thread_local! {
	static CACHE: RefCell<HashMap<(usize, usize), i64>> = RefCell::new(HashMap::new());
}

fn main()
{
	let file: Vec<String> = aoc::read_file("2025", "07", aoc::WhichFile::Main);
	part1(&file);
	part2(&file);
}

fn part1(file: &Vec<String>)
{
	let mut map = file.clone();
	let mut answer: i64 = 0;

	map[0] = map[0].replace("S", "|");

	for y in 1..map.len()
	{
		for (x, c) in map[y].clone().bytes().enumerate()
		{
			if c == b'^'
			{
				if map[y - 1].bytes().nth(x).unwrap() == b'|'
				{
					if let Some(ch) = unsafe { map[y].as_bytes_mut().get_mut(x + 1) }
					{
						*ch = b'|';
					}
					if let Some(ch) = unsafe { map[y].as_bytes_mut().get_mut(x - 1) }
					{
						*ch = b'|';
					}
					answer += 1;
				}
			}
			else
			{
				if map[y - 1].bytes().nth(x).unwrap() == b'|'
				{
					if let Some(ch) = unsafe { map[y].as_bytes_mut().get_mut(x) }
					{
						*ch = b'|';
					}
				}
			}
		}
	}


	println!("Part 1: {}", answer);
}

fn part2(file: &Vec<String>)
{
	let x = file[0].find("S").unwrap();
	let answer: i64 = calc_timeline(file, 0, x);

	println!("Part 2: {}", answer);
}

fn calc_timeline(map: &Vec<String>, y: usize, x: usize) -> i64
{
	if y == map.len()
	{
		return 1;
	}

	let cached = CACHE.with(|cache|
	{
		cache.borrow().get(&(y, x)).copied()
	});

	if let Some(ans) = cached
	{
		return ans;
	}

	if map[y].bytes().nth(x) == Some(b'^')
	{
		let ans = calc_timeline(map, y + 1, x + 1) + calc_timeline(map, y + 1, x - 1);

		CACHE.with(|cache|
			{
				cache.borrow_mut().insert((y, x), ans);
			}
		);

		return ans
	}
	else
	{
		return calc_timeline(map, y + 1, x)
	}
}

#[test]
fn test_part1_file1()
{
	let file: Vec<String> = aoc::read_file("2025", "07", aoc::WhichFile::Test(1));
	part1(&file);

	let result = {
		let mut gs = aoc::GLOBAL_STRING.lock().unwrap();
		std::mem::take(&mut *gs)
	};

	assert!(result.contains("Part 1: 21"))
}

// #[test]
// fn test_part1_file2()
// {
// 	let file: Vec<String> = aoc::read_file("2025", "07", aoc::WhichFile::Test(2));
// 	part1(&file);
//
// 	let result = {
// 		let mut gs = aoc::GLOBAL_STRING.lock().unwrap();
// 		std::mem::take(&mut *gs)
// 	};
//
// 	assert!(result.contains("Part 1: "))
// }

#[test]
fn test_part2_file1()
{
	let file: Vec<String> = aoc::read_file("2025", "07", aoc::WhichFile::Test(1));
	part2(&file);

	let result = {
		let mut gs = aoc::GLOBAL_STRING.lock().unwrap();
		std::mem::take(&mut *gs)
	};

	assert!(result.contains("Part 2: 40"))
}
// #[test]
// fn test_part2_file2()
// {
// 	let file: Vec<String> = aoc::read_file("2025", "07", aoc::WhichFile::Test(2));
// 	part2(&file);
//
// 	let result = {
// 		let mut gs = aoc::GLOBAL_STRING.lock().unwrap();
// 		std::mem::take(&mut *gs)
// 	};
//
// 	assert!(result.contains("Part 2: "))
// }
