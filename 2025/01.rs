pub fn setup(year: &str, day: &str)
{
	let file = aoc::read_file(year, day, aoc::WhichFile::Test(2));
	let _ = part1(&file);
	let _ = part2(&file);
}

fn part1(file: &Vec<String>)
{
	let mut answer: i64 = 0;
	let mut place: i64 = 50;
	for l in file
	{
		let c: u8 = l.as_bytes()[0];
		let num: i64 = l[1..].parse().unwrap();
		if c == b'L'
		{
			place -= num;
		}
		else
		{
			place += num;
		}
		place %= 100;
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
		let c: u8 = l.as_bytes()[0];
		let mut num: i64 = l[1..].parse().unwrap();
		if c == b'L'
		{
			answer += num / 100;
			num = num % 100;
			if num < place || place == 0
			{
				place -= num;
			}
			else
			{
				place -= num;
				answer += 1;
			}
			if place < 0
			{
				place += 100;
			}
		}
		else
		{
			answer += num / 100;
			num = num % 100;
			place += num;
			if place > 99
			{
				answer += 1;
				place -= 100;
			}
		}
	}

	println!("Part 2: {answer}");
}

