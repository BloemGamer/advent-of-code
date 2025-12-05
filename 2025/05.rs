use std::{
	cmp::Ord,
	ops::RangeInclusive
};


pub fn setup(year: &str, day: &str)
{
	let file: Vec<String> = aoc::read_file(year, day, aoc::WhichFile::Main);
	let _ = part1(&file);
	let _ = part2(&file);
}

fn part1(file: &Vec<String>)
{
	let mut answer: i64 = 0;

	let mut highest_id: i64 = 0;
	let mut ranges: Vec<RangeInclusive<i64>> = Vec::new();
	let mut end_ranges = 0;

	for (i,f) in file.iter().enumerate()
	{
		if f.len() == 0
		{
			end_ranges = i;
			break;
		}
		let s: Vec<&str> = f.split("-").collect();
		ranges.push(s[0].parse().unwrap()..=s[1].parse().unwrap());
		highest_id = i64::max(highest_id, s[1].parse().unwrap())
	}

	ranges.sort_by(|a, b| i64::cmp(a.start(), b.start()));

	let mut allowed: Vec<RangeInclusive<i64>> = vec![ranges[0].clone()];

	for r in &ranges[1..]
	{
		let last: &mut std::ops::RangeInclusive<i64> = allowed.last_mut().unwrap();
		if *r.start() <= *last.end()
		{
			let new_end = i64::max(*last.end(),*r.end());
			*last = *last.start()..=new_end;
			continue;
		}
		allowed.push(r.clone());
	}

	'floop: for f in &file[end_ranges+1..]
	{
		for r in &allowed
		{
			if r.contains(&f.parse::<i64>().unwrap())
			{
				answer += 1;
				continue 'floop;
			}
		}
	}


	println!("Part 1: {}", answer);
}

fn part2(file: &Vec<String>)
{
	let mut answer: i64 = 0;

	let mut highest_id: i64 = 0;
	let mut ranges: Vec<RangeInclusive<i64>> = Vec::new();

	for f in file
	{
		if f.len() == 0
		{
			break;
		}
		let s: Vec<&str> = f.split("-").collect();
		ranges.push(s[0].parse().unwrap()..=s[1].parse().unwrap());
		highest_id = i64::max(highest_id, s[1].parse().unwrap())
	}

	ranges.sort_by(|a, b| i64::cmp(a.start(), b.start()));

	let mut allowed: Vec<RangeInclusive<i64>> = vec![ranges[0].clone()];


	for r in &ranges[1..]
	{
		let last: &mut std::ops::RangeInclusive<i64> = allowed.last_mut().unwrap();
		if *r.start() <= *last.end()
		{
			let new_end = i64::max(*last.end(),*r.end());
			*last = *last.start()..=new_end;
			continue;
		}
		allowed.push(r.clone());
	}

	for a in &allowed
	{
		answer += a.end() - a.start() + 1;
	}



	println!("Part 2: {}", answer);
}

