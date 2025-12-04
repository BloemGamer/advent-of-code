const DIRECTIONS: [(isize, isize);8] = [(-1,-1),(-1,0),(-1,1),(0,-1),(0,1),(1,-1),(1,0),(1,1)];

pub fn setup(year: &str, day: &str)
{
	let file: Vec<String> = aoc::read_file(year, day, aoc::WhichFile::Main);
	let _ = part1(&file);
	let _ = part2(&file);
}

fn part1(file: &Vec<String>)
{
	let mut answer: i64 = 0;

	for (y, l) in file.iter().enumerate()
	{
		for (x, c) in l.bytes().enumerate()
		{
			let mut amount_rolls: i64 = 0;
			if c != b'@'
			{
				continue;
			}
			for (dy, dx) in DIRECTIONS
			{
				if let Some(r) = file.get((dy + y as isize) as usize)
				{
					if let Some(ret) = r.as_bytes().get((dx + x as isize) as usize)
					{
						if *ret == b'@'
						{
							amount_rolls += 1;
						}
					}
				}
			}
			if amount_rolls < 4
			{
				answer += 1;
			}
		}
	}

	println!("Part 1: {}", answer);
}

fn part2(file: &Vec<String>)
{
	let mut f = aoc::map_from_file!(file);
	let mut answer: i64 = 0;

	let mut changed: bool = true;
	while changed
	{
		changed = false;
		for (y, l) in f.clone().iter().enumerate()
		{
			for (x, c) in l.iter().enumerate()
			{
				let mut amount_rolls: i64 = 0;
				if *c != '@'
				{
					continue;
				}
				for (dy, dx) in DIRECTIONS
				{
					if let Some(r) = f.get((dy + y as isize) as usize)
					{
						if let Some(ret) = r.get((dx + x as isize) as usize)
						{
							if *ret == '@'
							{
								amount_rolls += 1;
							}
						}
					}
				}
				if amount_rolls < 4
				{
					f[y][x] = '.';
					answer += 1;
					changed = true;
				}
			}
		}
	}
	println!("Part 2: {}", answer);
}

