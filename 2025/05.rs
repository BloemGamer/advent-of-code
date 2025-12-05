use std::{
	cmp::Ord,
	ops::RangeInclusive,
	time::Instant,
};
use aoc;



pub fn setup(year: &str, day: &str)
{
	let time_file = Instant::now();
	let mut answer_p1: i64 = 0;
	let mut answer_p2: i64 = 0;
	let file: Vec<String> = aoc::read_file(year, day, aoc::WhichFile::Test(2));
	let time_used_file = time_file.elapsed();

	let time_ranges = Instant::now();
	let (start_id, ranges) = get_ranges(&file);
	let time_used_ranges= time_ranges.elapsed();


	let time_p1 = Instant::now();
	let mut nums: Vec<i64> = Vec::new();
	for f in &file[start_id..]
	{
		nums.push(f.parse::<i64>().unwrap());
	}
	nums.sort();
	let mut i = 0;
	'floop: for num in &nums
	{
		while num > ranges[i].end()
		{
			i += 1;
			if i >= ranges.len()
			{
				break 'floop;
			}
		}
		if ranges[i].contains(num)
		{
			answer_p1 += 1;
			continue 'floop;
		}
	}
	let time_used_p1 = time_p1.elapsed();

	let time_p2 = Instant::now();
	for a in &ranges
	{
		answer_p2 += a.end() - a.start() + 1;
	}
	let time_used_p2 = time_p2.elapsed();
	let time_used_total = time_file.elapsed();

	println!("File:   {:5}ms", time_used_file.as_millis());
	println!("Ranges: {:5}ms", time_used_ranges.as_millis());
	println!("Part 1: {:5}ms", time_used_p1.as_millis());
	println!("Part 2: {:5}ms", time_used_p2.as_millis());
	println!("Total:  {:5}ms", time_used_total.as_millis());

	println!();

	println!("Part 1: {}", answer_p1);
	println!("Part 2: {}", answer_p2);
}

fn get_ranges(file: &Vec<String>) -> (usize, Vec<RangeInclusive<i64>>)
{
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
	return (end_ranges + 1, allowed);
}
