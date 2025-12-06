#[cfg(test)]
use aoc::{println};

fn main()
{
	let file: Vec<String> = aoc::read_file("2025", "06", aoc::WhichFile::Main);
	part1(&file);
	part2(&file);
}

fn part1(file: &Vec<String>)
{
	let mut answer: i64 = 0;

	let mut rows: Vec<Vec<i64>> = Vec::new();
	let operators: Vec<char> = file.last().unwrap().split_whitespace().into_iter().map(|x| x.chars().nth(0).unwrap()).collect();

	for f in &file[..file.len() - 1]
	{
		let row: Vec<i64> = f.split_whitespace().into_iter().map(|x| x.parse::<i64>().unwrap()).collect();
		rows.push(row);
	}
	for (i, op) in operators.iter().enumerate()
	{
		let mut tmp_answer: i64 = rows[0][i];
		match *op
		{
			'+' => rows[1..].iter().for_each(|x|tmp_answer += x[i]),
			'*' => rows[1..].iter().for_each(|x|tmp_answer *= x[i]),
			_ => panic!(),
		}
		answer += tmp_answer;
	}

	println!("Part 1: {}", answer);
}

fn part2(file: &Vec<String>)
{
	let mut answer: i64 = 0;

	let operators: Vec<char> = file.last().unwrap().split_whitespace().into_iter().map(|x| x.chars().nth(0).unwrap()).collect();

	let mut new_vec: Vec<Vec<u8>> = vec![vec![0; file.len()]; file[0].len()];

	for (y, l) in file[..file.len() - 1].iter().enumerate()
	{
		for (x, c) in l.bytes().enumerate()
		{
			new_vec[x][y] = c;
		}
	}

	let mut tmp_vec: Vec<Option<i64>> = Vec::new();
	for v in new_vec
	{
		let ts = String::from_utf8(v).unwrap();
		let s = ts[..ts.len() - 1].trim();
		// println!("|{}| -> |{}|",ts, s);
		match s.parse::<i64>()
		{
			Ok(num) => tmp_vec.push(Some(num)),
			Err(_) => tmp_vec.push(None),
		}
	}

	let mut rows: Vec<Vec<i64>> = Vec::new();
	let mut current_row: Vec<i64> = Vec::new();

	for item in tmp_vec
	{
		match item
		{
			Some(n) => current_row.push(n),
			None =>
			{
				if !current_row.is_empty()
				{
					rows.push(current_row);
					current_row = Vec::new();
				}
			}
		}
	}
	if !current_row.is_empty()
	{
		rows.push(current_row);
	}

	for (i, op) in operators.iter().enumerate()
	{
		let mut tmp_answer: i64 = rows[i][0];
		match *op
		{
			'+' => rows[i].iter().skip(1).for_each(|x| tmp_answer += x),
			'*' => rows[i].iter().skip(1).for_each(|x| tmp_answer *= x),
			_ => panic!(),
		}
		answer += tmp_answer;
	}

	println!("Part 2: {}", answer);
}

#[test]
fn test_part1_file1()
{
	let file: Vec<String> = aoc::read_file("2025", "06", aoc::WhichFile::Test(1));
	part1(&file);

	let result = {
		let mut gs = aoc::GLOBAL_STRING.lock().unwrap();
		std::mem::take(&mut *gs)
	};

	std::println!("{}",result);
	assert!(result.contains("Part 1: 4277556"))
}

#[test]
fn test_part2_file1()
{
	let file: Vec<String> = aoc::read_file("2025", "06", aoc::WhichFile::Test(1));
	part2(&file);

	let result = {
		let mut gs = aoc::GLOBAL_STRING.lock().unwrap();
		std::mem::take(&mut *gs)
	};
	std::println!("{}",result);

	assert!(result.contains("Part 2: 3263827"))
}
