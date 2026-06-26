use std::cmp::Ordering;

#[derive(PartialEq, Eq, Debug)]
struct Dir
{
	y: i64,
	x: i64,
}

fn main()
{
	setup("2019", "10");
}

fn setup(year: &str, day: &str)
{
	let file = aoc::read_file(year, day, aoc::WhichFile::Main);
	let pos: aoc::map::Pos = part1(&file);
	part2(&file, pos);
}

fn part1(file: &Vec<String>) -> aoc::map::Pos
{
	let map: Vec<Vec<char>> = aoc::map_from_file!(file);
	let mut count_map: Vec<Vec<i64>> = vec![vec![0; map[0].len()]; map.len()];

	for (my, m) in map.iter().enumerate()
	{
		for (mx, &c) in m.iter().enumerate()
		{
			if c == '.' { continue; }

			for py in -(map.len() as i64)..(map.len() as i64)
			{
				'line_loop: for px in -(map[0].len() as i64)..(map[0].len() as i64)
				{
					if gcd(px.abs(), py.abs()) != 1 { continue; }

					let mut ry: i64 = my as i64 + py;
					let mut rx: i64 = mx as i64 + px;


					while ry >= 0 && ry < map.len() as i64 && 
							rx >= 0 && rx < map[0].len() as i64
							&& !(py == 0 && px == 0)
					{
						if map[ry as usize][rx as usize] == '#'
						{
							count_map[my as usize][mx as usize] += 1;
							continue 'line_loop;
						}

						ry += py;
						rx += px;
					}
				}
			}
		}
	}

	let mut max_count: i64 = 0;
	let mut pos: aoc::map::Pos= Default::default();
	for (y,m) in count_map.iter().enumerate()
	{
		for (x, &count) in m.iter().enumerate()
		{
			// print!("{:5}", count);
			if count > max_count
			{
				pos = aoc::map::Pos { y, x };
				max_count = count;
			}

		}
		// println!();
	}

	println!("Part 1: {}", max_count);
	return pos;
}

fn part2(file: &Vec<String>, pos: aoc::map::Pos)
{
	let map: Vec<Vec<char>> = aoc::map_from_file!(file);
	let (sx, sy) = (pos.x as i64, pos.y as i64);

	let mut asteroids: Vec<(i64, i64)> = map
		.iter()
		.enumerate()
		.flat_map(|(y, row)| {
			row.iter()
				.enumerate()
				.filter_map(move |(x, &c)| {
					if c == '#' && !(x as i64 == sx && y as i64 == sy) {
						Some((x as i64, y as i64))
					} else {
						None
					}
				})
		})
	.collect();

	asteroids.sort_by(|&(ax, ay), &(bx, by)| {
		let angle_a = angle_from_north(ax - sx, ay - sy);
		let angle_b = angle_from_north(bx - sx, by - sy);
		angle_a.partial_cmp(&angle_b).unwrap_or(Ordering::Equal)
			.then_with(|| {
				let da = (ax - sx).abs() + (ay - sy).abs();
				let db = (bx - sx).abs() + (by - sy).abs();
				da.cmp(&db)
			})
	});

	use std::collections::HashMap;
	let mut groups: Vec<(i64, i64)> = Vec::new(); // direction keys in order
	let mut by_dir: HashMap<(i64, i64), Vec<(i64, i64)>> = HashMap::new();

	for &(ax, ay) in &asteroids {
		let dx = ax - sx;
		let dy = ay - sy;
		let g = gcd(dx.abs(), dy.abs());
		let key = (dx / g, dy / g);
		let entry = by_dir.entry(key).or_insert_with(|| {
			groups.push(key);
			Vec::new()
		});
		entry.push((ax, ay));
	}

	let mut vaporized = 0;
	loop {
		let mut any = false;
		for dir in &groups {
			if let Some(queue) = by_dir.get_mut(dir) {
				if !queue.is_empty() {
					let (x, y) = queue.remove(0);
					vaporized += 1;
					if vaporized == 200 {
						println!("Part 2: {}", x * 100 + y);
						return;
					}
					any = true;
				}
			}
		}
		if !any { break; }
	}
}

fn angle_from_north(dx: i64, dy: i64) -> f64 {
	let angle = (dx as f64).atan2(-dy as f64);
	let a = if angle < 0.0 { angle + 2.0 * std::f64::consts::PI } else { angle };
	a
}


fn gcd(a: i64, b: i64) -> i64
{
	if b == 0 { a } else { gcd(b, a % b) }
}
