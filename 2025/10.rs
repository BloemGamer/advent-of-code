#[cfg(test)]
use aoc::{println};
use itertools::Itertools;
use z3::{ast::Int, Optimize, SatResult};

fn main()
{
	let file: Vec<String> = aoc::read_file("2025", "10", aoc::WhichFile::Main);
	part1(&file);
	part2(&file);
}

fn part1(file: &Vec<String>)
{
	let mut answer: i64 = 0;
	let input: Vec<(&str, Vec<Vec<i64>>, &str)> = file.iter().map(|f|
		{
			let mut tmp = f.split_whitespace();
			let first = tmp.next().unwrap().trim_matches('[').trim_matches(']');
			let buttons: Vec<Vec<i64>> = tmp.clone().map_while(|t|
				{
					if t.bytes().nth(0).unwrap() == b'{'
					{
						return None;
					}
					let nums: Vec<i64> = t.trim_matches('(').trim_matches(')').split(',').map(|n|
						{
							n.parse::<i64>().unwrap()
						}
					).collect();

					return Some(nums)
				}
			).collect();
			let joltage = tmp.last().unwrap();
			return (first, buttons, joltage)
		}
	).collect();

	input.iter().for_each(|(needed, buttons, _)|
		{
			let mut min_presses = i64::MAX;
			for v in buttons.iter().powerset()
			{
				let mut ans: Vec<u8> = needed.bytes().collect();
				ans.iter_mut().for_each(|a|
					{
						*a = match a
						{
							b'#' => 1,
							b'.' => 0,
							_ => unreachable!(),
						}
					}
				);
				v.iter().for_each(|b|
					{
						b.iter().for_each(|n|
							{
								ans[*n as usize] = ans[*n as usize] ^ 0b01;
							}
						);
					}
				);
				if !ans.iter().any(|&x| x != 0)
				{
					min_presses = i64::min(min_presses, v.len() as i64)
				}
			}
			answer += min_presses;
		}
	);

	println!("Part 1: {}", answer);
}

fn part2(file: &Vec<String>)
{
	let mut answer: i64 = 0;

	let input: Vec<(&str, Vec<Vec<i64>>, Vec<i64>)> = file.iter().map(|f|
		{
			let mut tmp = f.split_whitespace();
			let first = tmp.next().unwrap().trim_matches('[').trim_matches(']');
			let buttons: Vec<Vec<i64>> = tmp.clone().map_while(|t|
				{
					if t.bytes().nth(0).unwrap() == b'{'
					{
						return None;
					}
					let nums: Vec<i64> = t.trim_matches('(').trim_matches(')').split(',').map(|n|
						{
							n.parse::<i64>().unwrap()
						}
					).collect();

					return Some(nums)
				}
			).collect();
			let joltage: Vec<i64> = tmp.last().unwrap().trim_matches('{').trim_matches('}').split(',').map(|n| n.parse::<i64>().unwrap()).collect();
			return (first, buttons, joltage)
		}
	).collect();

	input.iter().for_each(|(_, buttons, needed)|
		{
			answer += min_needed_v3(needed, buttons);
		}
	);

	println!("Part 2: {}", answer);
}



fn min_needed_v3(needed: &[i64], buttons: &[Vec<i64>]) -> i64
{
	let opt = Optimize::new();

	let button_vars: Vec<Int> = (0..buttons.len())
		.map(|i| Int::new_const(format!("b{}", i)))
		.collect();

	button_vars.iter().for_each(|b| opt.assert(&b.ge(&Int::from_i64(0))));

	needed.iter().enumerate().for_each(|(pos, &target)|
		{
			let affecting: Vec<&Int> = buttons
				.iter()
				.enumerate()
				.filter(|(_, btn)| btn.contains(&(pos as i64)))
				.map(|(i, _)| &button_vars[i])
				.collect();

				let sum = Int::add(&affecting);
				opt.assert(&sum.eq(&Int::from_i64(target)));
		}
	);

	let total = Int::add(&button_vars.iter().collect::<Vec<_>>());
	opt.minimize(&total);

	return match opt.check(&[])
	{
		SatResult::Sat =>
		{
			let model = opt.get_model().unwrap();
			model.eval(&total, true).unwrap().as_i64().unwrap()
		}
		_ => panic!("No solution found"),
	}
}

#[test]
fn test_part1_file1()
{
	let file: Vec<String> = aoc::read_file("2025", "10", aoc::WhichFile::Test(1));
	part1(&file);

	let result = {
		let mut gs = aoc::GLOBAL_STRING.lock().unwrap();
		std::mem::take(&mut *gs)
	};

	std::println!("{}", result);
	assert!(result.contains("Part 1: 7"))
}

#[test]
fn test_part2_file1()
{
	let file: Vec<String> = aoc::read_file("2025", "10", aoc::WhichFile::Test(1));
	part2(&file);

	let result = {
		let mut gs = aoc::GLOBAL_STRING.lock().unwrap();
		std::mem::take(&mut *gs)
	};

	std::println!("{}", result);
	assert!(result.contains("Part 2: 33"))
}

// #[test]
// fn test_part1_file2()
// {
// 	let file: Vec<String> = aoc::read_file("2025", "10", aoc::WhichFile::Test(2));
// 	part1(&file);
//
// 	let result = {
// 		let mut gs = aoc::GLOBAL_STRING.lock().unwrap();
// 		std::mem::take(&mut *gs)
// 	};
//
//	std::println!("{}", result);
// 	assert!(result.contains("Part 1: "))
// }
//
// #[test]
// fn test_part2_file2()
// {
// 	let file: Vec<String> = aoc::read_file("2025", "10", aoc::WhichFile::Test(2));
// 	part2(&file);
//
// 	let result = {
// 		let mut gs = aoc::GLOBAL_STRING.lock().unwrap();
// 		std::mem::take(&mut *gs)
// 	};
//
// 	std::println!("{}", result);
// 	assert!(result.contains("Part 2: 12345"))
// }
