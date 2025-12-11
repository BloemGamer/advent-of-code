use std::{cell::RefCell, collections::HashMap};


#[cfg(test)]
use aoc::{println};

thread_local! {
	static CACHE: RefCell<HashMap<(String, [bool; 2]), Option<i64>>> = RefCell::new(HashMap::new());
}

fn main()
{
	let file: Vec<String> = aoc::read_file("2025", "11", aoc::WhichFile::Main);
	part1(&file);
	part2(&file);
}

fn part1(file: &Vec<String>)
{
	let paths: HashMap<&str, Vec<&str>> = file.iter().map(|f|
		{
			let mut tmp = f.split(" ");
			let first = &tmp.next().unwrap()[0..3];
			(first, tmp.collect())
		}
	).collect();

	let answer = paths_count(&paths, "you");

	println!("Part 1: {}", answer);
}

fn part2(file: &Vec<String>)
{
	let paths: HashMap<&str, Vec<&str>> = file.iter().map(|f|
		{
			let mut tmp = f.split(" ");
			let first = &tmp.next().unwrap()[0..3];
			(first, tmp.collect())
		}
	).collect();

	let answer = paths_count_v2(&paths, "svr", [false, false]);

	println!("Part 2: {}", answer);
}

fn paths_count(edges: &HashMap<&str, Vec<&str>>, current: &str) -> i64
{
	if current == "out"
	{
		return 1;
	}

	return edges.get(current).unwrap().iter().map(|p| paths_count(edges, p)).sum()
}

fn paths_count_v2(edges: &HashMap<&str, Vec<&str>>, current: &str, mut visited: [bool; 2]) -> i64
{
	let cached = CACHE.with(|cache|
	{
		cache.borrow().get(&(current.to_string(), visited)).copied()
	});
	if let Some(ret) = cached
	{
		return if let Some(r) = ret { r } else { 0 }
	}

	if current == "out"
	{
		return if visited[0] && visited[1] { 1 } else { 0 };
	}

	if current == "fft"
	{
		visited[0] = true;
	}
	if current == "dac"
	{
		visited[1] = true;
	}

	let ret: i64 = edges.get(current).unwrap().iter().map(|p| paths_count_v2(edges, p, visited)).sum();
	CACHE.with(|cache|
	{
		cache.borrow_mut().insert((current.to_string(), visited), Some(ret))
	});
	return  ret
}

#[test]
fn test_part1_file1()
{
	let file: Vec<String> = aoc::read_file("2025", "11", aoc::WhichFile::Test(1));
	part1(&file);

	let result = {
		let mut gs = aoc::GLOBAL_STRING.lock().unwrap();
		std::mem::take(&mut *gs)
	};

	std::println!("{}", result);
	assert!(result.contains("Part 1: 5"))
}

#[test]
fn test_part2_file2()
{
	let file: Vec<String> = aoc::read_file("2025", "11", aoc::WhichFile::Test(2));
	part2(&file);

	let result = {
		let mut gs = aoc::GLOBAL_STRING.lock().unwrap();
		std::mem::take(&mut *gs)
	};

	std::println!("{}", result);
	assert!(result.contains("Part 2: 2"))
}
