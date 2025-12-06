use std::{
	cmp::Ord,
	time::Instant,
	fs,
};


fn main()
{
	let time_file = Instant::now();
	let mut answer_p1: i64 = 0;
	let mut answer_p2: i64 = 0;

	let text = fs::read_to_string("2025/txt/05.test2.txt").unwrap();
	let file: Vec<&str> = text.lines().collect();

	let time_used_file = time_file.elapsed();

	let time_ranges = Instant::now();
	let (start_id, ranges) = get_ranges(&file);
	let time_used_ranges= time_ranges.elapsed();


	let time_p1 = Instant::now();
	let mut nums: Vec<i64> = Vec::new();
	for f in &file[start_id..file.len() - 1]
	{
		nums.push(f.trim_end_matches('\n').parse().unwrap());
	}
	nums.sort_unstable();
	let mut i = 0;
	'floop: for &num in &nums
	{
		while num > ranges[i].1
		{
			i += 1;
			if i >= ranges.len()
			{
				break 'floop;
			}
		}
		if ranges[i].0 <= num
		{
			answer_p1 += 1;
			continue 'floop;
		}
	}
	let time_used_p1 = time_p1.elapsed();

	let time_p2 = Instant::now();
	ranges.iter().for_each(|&(b, e)| answer_p2 += e - b + 1 );
	let time_used_p2 = time_p2.elapsed();

	let time_used_total = time_file.elapsed();

	println!("File:   {:7}us", time_used_file.as_micros());
	println!("Ranges: {:7}us", time_used_ranges.as_micros());
	println!("Part 1: {:7}us", time_used_p1.as_micros());
	println!("Part 2: {:7}us", time_used_p2.as_micros());
	println!("Total:  {:7}us", time_used_total.as_micros());

	println!();

	println!("Part 1: {}", answer_p1);
	println!("Part 2: {}", answer_p2);
}

#[inline(always)]
fn get_ranges(file: &Vec<&str>) -> (usize, Vec<(i64, i64)>)
{
	let mut ranges: Vec<(i64, i64)> = Vec::new();
	let mut end_ranges = 0;

	for (i,f) in file.iter().enumerate()
	{
		if f.len() == 0
		{
			end_ranges = i;
			break;
		}

		let s: (&str, &str) = f.split_once("-").unwrap();
		ranges.push((s.0.parse().unwrap(),s.1.parse().unwrap()));
	}

	ranges.sort_unstable_by_key(|a| a.0);

	let mut allowed: Vec<(i64, i64)> = vec![(ranges[0].0, ranges[0].1)];

	for r in &ranges[1..]
	{
		let last: &mut (i64, i64) = allowed.last_mut().unwrap();
		if r.0 <= last.1
		{
			last.1 = i64::max(last.1,r.1);
			continue;
		}
		allowed.push((r.0, r.1));
	}
	return (end_ranges + 1, allowed);
}
