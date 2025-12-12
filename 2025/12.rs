#[cfg(test)]
use aoc::{println};

// 431 too low

fn main()
{
	let file: Vec<String> = aoc::read_file("2025", "12", aoc::WhichFile::Main);
	part1(&file);
}

fn part1(file: &Vec<String>)
{
	let mut split = file.split(|f| f.is_empty());

	let regions: Vec<(Vec<i64>, Vec<i64>)> = split.next_back().unwrap().iter().map(|f|
		{
			let mut tmp = f.split_whitespace();
			let	area = tmp.next().unwrap();
			(area[..area.len() - 1].split('x').map(|a| a.parse::<i64>().unwrap()).collect(), tmp.map(|i| i.parse::<i64>().unwrap()).collect::<Vec<i64>>())
		}
	).collect();

	let shapes: Vec<Vec<Vec<i64>>> = split.map(|f|
		{
			f.iter().skip(1).map(|x|
				{
					x.bytes().map(|b|
						{
							match b
							{
								b'.' => 0,
								b'#' => 1,
								_ => panic!(),
							}
						}
					).collect()
				}
			).collect()
		}
	).collect();

	let shape_sizes: Vec<i64> = shapes.iter().map(|s| s.iter().flatten().sum()).collect();

	let answer: i64 = regions.iter().map(|(a, r)|
		{
			let area_size = a[0] * a[1];
			let needed_size = r.iter().enumerate().map(|(i, &x)|
				{
					shape_sizes[i] * x
				}
			).sum();
			if area_size > needed_size { 1 } else { 0 }
		}
	).sum();

	println!("Part 1: {}", answer);
}

