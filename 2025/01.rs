// 5795 too low

pub fn setup(year: &str, day: &str)
{
	let file = aoc::read_file(year, day, aoc::WhichFile::Main);
	let _ = part1(&file);
	let _ = part2(&file);
}

fn part1(file: &Vec<String>)
{
	let mut answer: i64 = 0;
	let mut place: i64 = 50;
	for l in file
	{
		let mut tmp = l.clone();
		let num: i64 = tmp.split_off(1).parse().unwrap();
		if tmp.chars().collect::<Vec<char>>()[0] == 'L'
		{
			place -= num;
		}
		else
		{
			place += num;
		}
		while place > 99
		{
			place -= 100;
		}
		while place < 0
		{
			place += 100;
		}
		if place == 0
		{
			answer += 1;
		}
	}

	println!("Part 1: {answer}");

}

fn part2(file: &Vec<String>)
{
	let mut answer: i64 = 0;
	let mut place: i64 = 50;
	for l in file
	{
		let mut tmp = l.clone();
		let num: i64 = tmp.split_off(1).parse().unwrap();
		if tmp.chars().collect::<Vec<char>>()[0] == 'L'
		{
			// place -= num;
			for _ in 0..num
			{
				place -= 1;
				if place % 100 == 0
				{
					answer += 1;
				}
			}
		}
		else
		{
			for _ in 0..num
			{
				place += 1;
				if place % 100 == 0
				{
					answer += 1;
				}
			}
		}
		if num == 0{
			if place % 100 == 0
			{
				answer += 1;
			}
		}
		while place > 99
		{
			place -= 100;
		}
		while place < 0
		{
			place += 100;
		}
	}

	println!("Part 2: {answer}");
}

