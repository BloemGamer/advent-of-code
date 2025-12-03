pub fn setup(year: &str, day: &str)
{
	let file: Vec<String> = aoc::read_file(year, day, aoc::WhichFile::Main);
	let _ = part1(&file);
	let _ = part2(&file);
}

fn part1(file: &Vec<String>)
{
	let mut answer: i64 = 0;

	for line in file
	{
		let mut highes_pos: usize = usize::MAX;
		let mut highes_num: u8 = 0;
		let mut second_num: u8 = 0;
		for (p,c) in line[0..line.len() - 1].bytes().enumerate()
		{
			if c > highes_num
			{
				highes_pos = p;
				highes_num = c;
			}
		}
		for c in line[highes_pos + 1..line.len()].bytes()
		{
			if c > second_num
			{
				second_num = c;
			}
		}
		answer += 10 * (highes_num as i64 - b'0' as i64) + (second_num as i64 - b'0' as i64);
	}

	println!("Part 1: {}", answer);
}

fn part2(file: &Vec<String>)
{
	let mut answer: i64 = 0;
	for line in file
	{
		let mut line_answer: i64 = 0;
		let mut highes_pos: usize = usize::MAX;
		let mut highes_num: u8 = 0;
		for (p,c) in line[0..line.len() - 11].bytes().enumerate()
		{
			if c > highes_num
			{
				highes_pos = p;
				highes_num = c;
			}
		}
		line_answer += highes_num as i64 - b'0' as i64;
		for i in (0..11).rev()
		{
			let old_highest_pos: usize = highes_pos + 1;
			let range = highes_pos + 1..line.len() - i;
			let mut second_num: u8 = 0;
			for (p,c) in line[range].bytes().enumerate()
			{
				if c > second_num
				{
					highes_pos = p;
					second_num = c;
				}
			}
			highes_pos += old_highest_pos;
			line_answer = (line_answer * 10) + (second_num as i64 - b'0' as i64);
		}
		answer += line_answer;
	// println!("line_answer: {}", line_answer);
	}

	println!("Part 2: {}", answer);
}

