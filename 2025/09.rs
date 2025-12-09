use aoc::map::Pos;
#[cfg(test)]
use aoc::{println};
use itertools::Itertools;


fn main()
{
	let file: Vec<String> = aoc::read_file("2025", "09", aoc::WhichFile::Main);
	part1(&file);
	part2(&file);
}

fn part1(file: &Vec<String>)
{
	let mut answer: i64 = 0;

	let coords: Vec<Pos> = file.iter().map(|f|
		{
			let mut tmp = f.split(",");
			Pos {x: tmp.next().unwrap().parse().unwrap(), y: tmp.next().unwrap().parse().unwrap()}
		}
	).collect();

	coords.iter().for_each(|c1|
		{
			coords.iter().for_each(|c2|
				{
					answer = i64::max(answer, (1 + i64::abs(c1.x as i64 - c2.x as i64)) * (1 + i64::abs(c1.y as i64 - c2.y as i64)));
				}
			);
		}
	);

	println!("Part 1: {}", answer);
}

fn part2(file: &Vec<String>)
{
	let mut answer: i64 = 0;

	let coords: Vec<Pos> = file.iter().map(|f|
		{
			let mut tmp = f.split(",");
			Pos {x: tmp.next().unwrap().parse().unwrap(), y: tmp.next().unwrap().parse().unwrap()}
		}
	).collect();

	let mut borders: Vec<(Pos, Pos)> = coords.windows(2).map(|c|
		{
			(c[0], c[1])
		}
	).collect_vec();
	borders.push((*coords.last().unwrap(), coords[0]));

	coords.iter().for_each(|c1|
		{
			coords.iter().for_each(|c2|
				{
					let allowed: Option<()> = borders.iter().find_map(|c|
						{
							if intersects_rectangle((*c1, *c2), *c) {
								Some(())
							} else {
								None
							}
						}
					);
					if allowed.is_none()
					{
						answer = i64::max(answer, (1 + i64::abs(c1.x as i64 - c2.x as i64)) * (1 + i64::abs(c1.y as i64 - c2.y as i64)));
					}
				}
			);
		}
	);

	println!("Part 2: {}", answer);
}



fn normalize((p1, p2): (Pos, Pos)) -> (Pos, Pos)
{
	let min_x = p1.x.min(p2.x);
	let max_x = p1.x.max(p2.x);
	let min_y = p1.y.min(p2.y);
	let max_y = p1.y.max(p2.y);
	(
		Pos { x: min_x, y: min_y },
		Pos { x: max_x, y: max_y },
	)
}

fn intersects_rectangle((pa1, pa2): (Pos, Pos), (pb1, pb2): (Pos, Pos)) -> bool
{
	let (a1, a2) = normalize((pa1, pa2));
	let (b1, b2) = normalize((pb1, pb2));

	debug_assert!(b1.y == b2.y || b1.x == b2.x);

	if b1.y == b2.y
	{
		let y = b1.y;
		if y <= a1.y || y >= a2.y
		{
			return false;
		}
		if b2.x <= a1.x || b1.x >= a2.x
		{
			return false;
		}
		return true;
	}

	if b1.x == b2.x
	{
		let x = b1.x;
		if x <= a1.x || x >= a2.x
		{
			return false;
		}
		if b2.y <= a1.y || b1.y >= a2.y
		{
			return false;
		}
		return true;
	}

	false
}

#[test]
fn test_part1_file1()
{
	let file: Vec<String> = aoc::read_file("2025", "09", aoc::WhichFile::Test(1));
	part1(&file);

	let result = {
		let mut gs = aoc::GLOBAL_STRING.lock().unwrap();
		std::mem::take(&mut *gs)
	};

	assert!(result.contains("Part 1: 50"))
}

#[test]
fn test_part2_file1()
{
	let file: Vec<String> = aoc::read_file("2025", "09", aoc::WhichFile::Test(1));
	part2(&file);

	let result = {
		let mut gs = aoc::GLOBAL_STRING.lock().unwrap();
		std::mem::take(&mut *gs)
	};

	std::println!("{}", result);
	assert!(result.contains("Part 2: 24"))
}

// #[test]
// fn test_part1_file2()
// {
// 	let file: Vec<String> = aoc::read_file("2025", "09", aoc::WhichFile::Test(2));
// 	part1(&file);
//
// 	let result = {
// 		let mut gs = aoc::GLOBAL_STRING.lock().unwrap();
// 		std::mem::take(&mut *gs)
// 	};
//
// 	assert!(result.contains("Part 1: "))
// }
//
// #[test]
// fn test_part2_file2()
// {
// 	let file: Vec<String> = aoc::read_file("2025", "09", aoc::WhichFile::Test(2));
// 	part2(&file);
//
// 	let result = {
// 		let mut gs = aoc::GLOBAL_STRING.lock().unwrap();
// 		std::mem::take(&mut *gs)
// 	};
//
// 	assert!(result.contains("Part 2: "))
// }
