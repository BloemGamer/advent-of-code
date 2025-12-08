#[cfg(test)]
use aoc::{println};

#[derive(Copy, Clone, PartialEq, Eq, PartialOrd, Ord,Debug)]
struct Coordinate
{
	x: i64,
	y: i64,
	z: i64,
}

fn main()
{
	let file: Vec<String> = aoc::read_file("2025", "08", aoc::WhichFile::Main);
	part1(&file, 1000);
	part2(&file);
}

fn part1(file: &Vec<String>, max_connections: usize)
{
	let mut connections: usize = 0;
	let coords: Vec<Coordinate> = file.iter().map(|f|
		{
			let mut tmp = f.split(",");
			Coordinate {x: tmp.next().unwrap().parse().unwrap(), y: tmp.next().unwrap().parse().unwrap(), z: tmp.next().unwrap().parse().unwrap()}
		}
	).collect();

	let mut distances: Vec<(i64, Coordinate, Coordinate)> = coords
		.iter()
		.flat_map(|c| {
			coords.iter().map(move |c2| {
				let dist = (c.x - c2.x).pow(2) + (c.y - c2.y).pow(2) + (c.z - c2.z).pow(2);
				(dist, *c, *c2)
			})
		}) .collect();

	distances.sort_unstable_by(|a, b| i64::cmp(&a.0, &b.0));

	let mut circuits: Vec<Vec<Coordinate>> = Vec::new();

	'dloop: for d in distances
	{
		if connections >= max_connections
		{
			break;
		}
		if d.1 >= d.2
		{
			continue;
		}
		connections += 1;

		let mut inx_c1: Option<usize> = None;
		let mut inx_c2: Option<usize> = None;
		for (i, c) in circuits.iter().enumerate()
		{
			if c.contains(&d.1) && c.contains(&d.2)
			{
				continue 'dloop;
			}
			if c.contains(&d.1)
			{
				inx_c1 = Some(i);
			}
			if c.contains(&d.2)
			{
				inx_c2 = Some(i);
			}
		}
		match (inx_c1, inx_c2)
		{
			(Some(i1), Some(i2)) =>
			{
				let (a, b) = if i1 > i2 { (i1, i2) } else { (i2, i1) };
				let mut removed = circuits.remove(a);
				circuits[b].append(&mut removed);
			},
			(Some(i1), None) =>
			{
				circuits[i1].push(d.2);
			},
			(None, Some(i2)) =>
			{
				circuits[i2].push(d.1);
			},
			_ =>
			{
				circuits.push(vec![d.1, d.2]);
			}
		}
		// println!("{:?}\n", circuits);

	}

	circuits.sort_unstable_by(|a, b| usize::cmp(&b.len(), &a.len()));

	let default = vec![Coordinate { x: 0, y: 0, z: 0 }];

	let (l1, l2, l3) = (
		circuits.get(0).unwrap_or(&default),
		circuits.get(1).unwrap_or(&default),
		circuits.get(2).unwrap_or(&default),
	);

	let answer = l1.len() * l2.len() * l3.len();

	println!("Part 1: {}", answer);
}

fn part2(file: &Vec<String>)
{
	let mut answer: i64 = 0;
	let coords: Vec<Coordinate> = file.iter().map(|f|
		{
			let mut tmp = f.split(",");
			Coordinate {x: tmp.next().unwrap().parse().unwrap(), y: tmp.next().unwrap().parse().unwrap(), z: tmp.next().unwrap().parse().unwrap()}
		}
	).collect();

	let mut distances: Vec<(i64, Coordinate, Coordinate)> = coords
		.iter()
		.flat_map(|c| {
			coords.iter().map(move |c2| {
				let dist = (c.x - c2.x).pow(2) + (c.y - c2.y).pow(2) + (c.z - c2.z).pow(2);
				(dist, *c, *c2)
			})
		}) .collect();

	distances.sort_unstable_by(|a, b| i64::cmp(&a.0, &b.0));

	let mut circuits: Vec<Vec<Coordinate>> = Vec::new();

	'dloop: for d in distances
	{
		if d.1 >= d.2
		{
			continue;
		}
		if let Some(l) = circuits.get(0)
		{
			if l.len() == file.len()
			{
				break;
			}
		}
		answer = d.1.x * d.2.x;


		let mut inx_c1: Option<usize> = None;
		let mut inx_c2: Option<usize> = None;
		for (i, c) in circuits.iter().enumerate()
		{
			if c.contains(&d.1) && c.contains(&d.2)
			{
				continue 'dloop;
			}
			if c.contains(&d.1)
			{
				inx_c1 = Some(i);
			}
			if c.contains(&d.2)
			{
				inx_c2 = Some(i);
			}
		}
		match (inx_c1, inx_c2)
		{
			(Some(i1), Some(i2)) =>
			{
				let (a, b) = if i1 > i2 { (i1, i2) } else { (i2, i1) };
				let mut removed = circuits.remove(a);
				circuits[b].append(&mut removed);
			},
			(Some(i1), None) =>
			{
				circuits[i1].push(d.2);
			},
			(None, Some(i2)) =>
			{
				circuits[i2].push(d.1);
			},
			_ =>
			{
				circuits.push(vec![d.1, d.2]);
			}
		}

	}
	println!("Part 2: {}", answer);
}

#[test]
fn test_part1_file1()
{
	let file: Vec<String> = aoc::read_file("2025", "08", aoc::WhichFile::Test(1));
	part1(&file, 10);

	let result = {
		let mut gs = aoc::GLOBAL_STRING.lock().unwrap();
		std::mem::take(&mut *gs)
	};

	std::println!("{}", result);
	assert!(result.contains("Part 1: 40"))
}

#[test]
fn test_part2_file1()
{
	let file: Vec<String> = aoc::read_file("2025", "08", aoc::WhichFile::Test(1));
	part2(&file);

	let result = {
		let mut gs = aoc::GLOBAL_STRING.lock().unwrap();
		std::mem::take(&mut *gs)
	};

	std::println!("{}", result);
	assert!(result.contains("Part 2: 25272"))
}

// #[test]
// fn test_part1_file2()
// {
// 	let file: Vec<String> = aoc::read_file("2025", "08", aoc::WhichFile::Test(2));
// 	part1(&file, 10);
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
// 	let file: Vec<String> = aoc::read_file("2025", "08", aoc::WhichFile::Test(2));
// 	part2(&file);
//
// 	let result = {
// 		let mut gs = aoc::GLOBAL_STRING.lock().unwrap();
// 		std::mem::take(&mut *gs)
// 	};
//
// 	assert!(result.contains("Part 2: "))
// }
