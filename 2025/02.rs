pub fn setup(year: &str, day: &str)
{
	let file: Vec<String> = aoc::read_file(year, day, aoc::WhichFile::Main);
	let _ = part1(&file);
	let _ = part2(&file);
}

fn part1(file: &Vec<String>)
{
	let mut answer: i64 = 0;

	let tmp: Vec<&str> = file[0].split(",").collect();
	let nums: Vec<Vec<&str>> = tmp.iter().map(|f|f.split("-").collect::<Vec<&str>>()).collect();

	for num in nums
	{
		let n1: i64 = num[0].parse().unwrap();
		let n2: i64 = num[1].parse().unwrap();
		for n in n1..=n2
		{
			let str: String = n.to_string();
			if str.len() % 2 != 0
			{
				continue;
			}
			let len: usize = str.len() / 2;
			let s1: &str = &str[..len];
			let s2: &str = &str[len..];
			if s1 == s2
			{
				answer += n;
			}
		}
	}

	println!("Part 1: {}", answer);
}

fn part2(file: &Vec<String>)
{
	let mut answer: i64 = 0;

	let tmp: Vec<&str> = file[0].split(",").collect();
	let nums: Vec<Vec<&str>> = tmp.iter().map(|f|f.split("-").collect::<Vec<&str>>()).collect();

	let re = fancy_regex::Regex::new(r"^(\d+)\1+$").unwrap();
	for num in nums
	{
		let n1: i64 = num[0].parse().unwrap();
		let n2: i64 = num[1].parse().unwrap();
		for n in n1..=n2
		{
			let str: String = n.to_string();
			if re.is_match(&str).unwrap()
			{
				answer += n;
				continue;
			}
		}
	}

	println!("Part 2: {}", answer);
}

