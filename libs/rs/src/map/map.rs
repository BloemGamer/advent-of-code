use aoc_macros::add_show;
pub use aoc_macros::ToPos;
use std::{collections::{HashMap, HashSet, VecDeque}, io::Write};

#[macro_export]
macro_rules! map_from_file
{
    ($vec:expr) =>
    {{
        $vec.iter().map(|s| s.chars().collect::<Vec<char>>()).collect::<Vec<Vec<char>>>()
    }};
}
pub use map_from_file;

pub const WEIGHT_MAP_NONE: Option<&[&[i64]]> = None::<&[&[i64]]>;

pub const DIRECTIONS: [(isize, isize); 4] = [(1, 0), (0, 1), (-1, 0), (0, -1)];
pub const HORIZONTAL_DIRECTIONS: [(isize, isize); 2] = [DIRECTIONS[1], DIRECTIONS[3]];
pub const VERTICAL_DIRECTIONS: [(isize, isize); 2] = [DIRECTIONS[0], DIRECTIONS[2]];

pub trait ToPos 
{
    fn to_pos(&self) -> Pos;
}

#[allow(dead_code)]
#[derive(Debug, Default, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Pos
{
    pub y: usize,
    pub x: usize,
}


#[allow(dead_code)]
impl Pos
{
    pub fn new() -> Self
    {
        Default::default()
    }
}

impl ToPos for Pos
{
    fn to_pos(&self) -> Pos
    {
        *self
    }
}


/// finds the intersections (or turns) of a given maze, and also pushes [`start`] and [`end`] to the vec so they can be find by the algoritms
#[macro_export]
macro_rules! find_intersections
{
    ($map:ident, $start:expr, $end:expr => $intersection_fn:expr => $ID:ty, $neighbors:path, $on_intersection:expr) =>
    {{
        let mut intersections: Vec<$ID> = Vec::new();
        for (y, row) in $map.iter().enumerate()
        {
            for (x, &c) in row.iter().enumerate()
            {
                let mut id: $ID = Default::default();
                id.y = y;
                id.x = x;
                
                if c == '#' { continue; }
                if $intersection_fn(&$map, id)
                {
                    $on_intersection(&mut intersections, id);
                    //intersections.push(id);
                }
            }
        }
        for s in &$start
        {
            if !intersections.contains(s) { intersections.push(*s); }
        }
        for e in &$end
        {
            if !intersections.contains(e) { intersections.push(*e); }
        }
        intersections
    }};
    ($map:ident, $start:expr, $end:expr) =>
    {{
        $crate::map::find_intersections!($map, $start, $end
            => $crate::map::dijkstra::intersection
            => $crate::map::Pos,
            $crate::map::neighbors,
            |intersection: &mut Vec<$crate::map::Pos>, id: $crate::map::Pos| { intersection.push(id) })
    }};
    ($map:ident, $start:expr, $end:expr, show) =>
    {{
        $crate::map::find_intersections!($map, $start, $end
            => $crate::map::intersection
            => $crate::map::Pos,
            $crate::map::dijkstra::neighbors,
            |intersection: &mut Vec<$crate::map::Pos>, id: $crate::map::Pos| { intersection.push(id); $crate::map::update_cell(&$map, id.to_pos(), 37, 41); })
    }};
}
pub use find_intersections;

/// Checks if a given point in a maze has at least 3 ways, so an intersection
pub fn intersection<T>(map: &[T], pos: Pos) -> bool
where 
    T: AsRef<[char]>,
{
    let count = neighbors(pos)
    .iter()
    .filter(|&&n| 
        map.as_ref().get(n.y)
            .and_then(|r| r.as_ref().get(n.x))
            .map_or(false, |&ch| ch != '#')
    )
    .count();
    if count >= 3 { true } else { false }
}

/// Checks if a given point in a maze has a turn
pub fn turn<T, A>(map: &[T], pos: A) -> bool
where 
    T: AsRef<[char]>,
    A: ToPos,
{
    let pos = pos.to_pos();
    let count_h = HORIZONTAL_DIRECTIONS.map(|(dy, dx)| { Pos{y: ((pos.y as isize + dy) as usize), x: ((pos.x as isize) + dx) as usize }})
    .iter()
    .filter(|&&n| 
        map.as_ref().get(n.y)
            .and_then(|r| r.as_ref().get(n.x))
            .map_or(false, |&ch| ch != '#')
    )
    .count();

    let count_v = VERTICAL_DIRECTIONS.map(|(dy, dx)| { Pos{y: ((pos.y as isize + dy) as usize), x: ((pos.x as isize) + dx) as usize }})
    .iter()
    .filter(|&&n| 
        map.as_ref().get(n.y)
            .and_then(|r| r.as_ref().get(n.x))
            .map_or(false, |&ch| ch != '#')
    )
    .count();

    if count_v != 0 && count_h != 0 { true } else { false }
}

pub fn neighbors(pos: Pos) -> [Pos; 4]
{
    DIRECTIONS.map(|(dy, dx)| { Pos{y: ((pos.y as isize + dy) as usize), x: ((pos.x as isize) + dx) as usize }})
}


/// Finds the fasted length between two intersections using a standard breath first flood fill. 
/// Supports a weightmap, if None, just fill in something like [`None::<&[&[i64]]>`] or [`map::WEIGHTMAP_NONE`]
#[macro_export]
macro_rules! find_length_intersections
{
    ($map:ident, $weight_map:expr,
        $intersections:ident 
        => $neighbors:path =>
        $ID:ty,
        $one_way:expr,
		$show:ident) =>
    {{
        let mut lengths: Vec<(($ID, $ID), i64)> = Vec::new();
        for (i, &start) in $intersections.iter().enumerate()
        {
            let mut queue: std::collections::VecDeque<($ID, i64)> = std::collections::VecDeque::new();
            let mut visited: Vec<$ID> = Vec::new();
            queue.push_back((start, 0));
            visited.push(start);
			let mut visited_show: Vec<$crate::map::Pos> = Vec::new();
			if $show
			{
				$crate::map::update_cell(&$map, start.to_pos(), 37, 43);
			}
            'finish_queue: while let Some((id, dist)) = queue.pop_front()
            {


                if $intersections.contains(&id) && id != start
                {
                    if $intersections.iter().position(|&pos| pos == id).unwrap() > i
                    {
                        lengths.push(((start, id), dist));
                        lengths.push(((id, start), dist));
                    }
                    continue 'finish_queue;
                }
				if $show
				{
					if id != start
					{
						$crate::map::update_cell(&$map, id.to_pos(), 37, 45);
						visited_show.push(id.to_pos());
					}
				}
                let neighbors = $neighbors(id);
                for (dir_idx, &n) in neighbors.iter().enumerate()
                {
                    if n.y < $map.len() && n.x < $map[0].len() && $map[n.y][n.x] != '#' && !visited.contains(&n)
                    {
                        let can_move = if $one_way
                        {
                            let current_cell = $map[id.y][id.x];
                            match current_cell
                            {
                                '^' => dir_idx == 0, 
                                'v' => dir_idx == 1, 
                                '<' => dir_idx == 2, 
                                '>' => dir_idx == 3, 
                                _ => true, 
                            }
                        } else {
                            true
                        };
                        
                        if can_move
                        {
                            visited.push(n);
                            if let Some(wm) = &$weight_map
                            {
                                queue.push_back((n, dist + wm[n.y][n.x]));
                            } else {
                                queue.push_back((n, dist + 1));
                            }
                        }
                    }
                }
            }
			for &vs in &visited_show
			{
				$crate::map::update_cell(&$map, vs, 37, 40);
			}
		visited_show.clear();
        }
        lengths
    }};

    ($map:ident, $weight_map:expr,
        $intersections:ident 
        => $neighbors:path =>
        $ID:ty,
        $one_way:expr) =>
	{{
        $crate::map::find_length_intersections!(
			$map, $weight_map,
			$intersections
			=> $neighbors =>
			$ID, $one_way, false
		)
	}};
    ($map:ident, $weight_map:expr, $intersections:ident => $neighbors:path => $ID:ty) =>
    {{
        $crate::map::find_length_intersections!(
            $map, $weight_map, $intersections =>
            $neighbors =>
            $ID,
            false)
    }};

    ($map:ident, $intersections:ident) =>
    {{
        $crate::map::find_length_intersections!(
            $map, None::<&[&[i64]]>, $intersections
            => $crate::map::neighbors
            => $crate::map::Pos,
            false)
    }};
	($map:ident, $intersections:ident, show) =>
    {{
        $crate::map::find_length_intersections!(
            $map, None::<&[&[i64]]>, $intersections
            => $crate::map::neighbors
            => $crate::map::Pos,
            false, true)
    }};
}
pub use find_length_intersections;


// this one prob needs it made, so it can do oneways too and like that, I don't want to do that
// right now tbh
/// This one does not work if you have the intersections function, not map at least all the intersections
#[add_show]
pub fn all_fastest_paths<ID, T>(backtrace: &[(ID, Vec<ID>)], end: &[ID], map: &[T]) -> Vec<Vec<bool>>
where 
    ID: ToPos + Eq + Copy,
	T: AsRef<[char]>,
{
    let mut map_speed: Vec<Vec<bool>> = vec![vec![false; map[0].as_ref().len()]; map.len()];
    let mut visited: Vec<ID> = Vec::new();

    let mut queue: VecDeque<&(ID, Vec<ID>)> = VecDeque::new();

    let starts: Vec<&(ID, Vec<ID>)> = backtrace.iter()
        .filter(|(id, _)| end.iter().any(|e| id == e))
        .collect();

    for start in starts
    {
        queue.push_back(start);
    }

    while let Some((id, from)) = queue.pop_front()
    {
		if visited.contains(&id)
		{
			continue;
		}
		visited.push(*id);
        for f in from
        {
			let path = shortest_path(map, id.to_pos(), f.to_pos()).expect("TF");
			//if path.is_empty() { panic!(); }
			for p in path
			{
				map_speed[p.y][p.x] = true;
				insert_here!(crate::map::update_cell(map, p, 37, 45););
			}
			let next = backtrace.iter().find(|(x, _)| *x == *f).expect("how than");
			queue.push_back(next);
        }
    }

    return map_speed
}


// this one prob needs it made, so it can do oneways too and like that, I don't want to do that
// right now tbh
/// This one does not work if you have the intersections function, not map at least all the intersections
#[add_show]
pub fn fastest_path<ID, T>(backtrace: &[(ID, Vec<ID>)], end: &[ID], map: &[T]) -> Vec<Vec<bool>>
where 
    ID: ToPos + Eq + Copy,
	T: AsRef<[char]>,
{
    let mut map_speed: Vec<Vec<bool>> = vec![vec![false; map[0].as_ref().len()]; map.len()];
    let mut visited: Vec<ID> = Vec::new();

    let mut queue: VecDeque<&(ID, Vec<ID>)> = VecDeque::new();

    let starts: Vec<&(ID, Vec<ID>)> = backtrace.iter()
        .filter(|(id, _)| end.iter().any(|e| id == e))
        .collect();

    for start in starts
    {
        queue.push_back(start);
    }

    while let Some((id, from)) = queue.pop_front()
    {
		if visited.contains(&id)
		{
			continue;
		}
		visited.push(*id);
		if from.is_empty()
		{
			break;
		}
		let f = from[0];
		let path = shortest_path(map, id.to_pos(), f.to_pos()).expect("TF");
		//if path.is_empty() { panic!(); }
		for p in path
		{
			map_speed[p.y][p.x] = true;
			insert_here!(crate::map::update_cell(map, p, 37, 45););
		}
		let next = backtrace.iter().find(|(x, _)| *x == f).expect("how than");
		queue.push_back(next);
    }

    return map_speed
}


fn shortest_path<T: AsRef<[char]>>(grid: &[T], start: Pos, end: Pos) -> Option<Vec<Pos>>
{
    let rows = grid.len();
    let cols = grid[0].as_ref().len();

    let mut queue = VecDeque::new();
    let mut visited = HashSet::new();
    let mut parent: HashMap<Pos, Pos> = HashMap::new();

    queue.push_back(start);
    visited.insert(start);
    if start == end { return Some(vec![start]) }

    let directions = [(-1i32, 0), (1, 0), (0, -1), (0, 1)]; // 4-way connectivity

    while let Some(current) = queue.pop_front()
	{
        if current == end
		{
            // reconstruct path
            let mut path = vec![current];
            let mut cur = current;
            while let Some(&p) = parent.get(&cur)
			{
                cur = p;
                path.push(cur);
            }
            path.reverse();
            return Some(path);
        }

        for &(dy, dx) in &directions
		{
            let ny = current.y as i32 + dy;
            let nx = current.x as i32 + dx;

            if ny >= 0 && ny < rows as i32 && nx >= 0 && nx < cols as i32
			{
                let next = Pos { y: ny as usize, x: nx as usize };
                if grid[next.y].as_ref()[next.x] != '#' && !visited.contains(&next)
				{
                    visited.insert(next);
                    parent.insert(next, current);
                    queue.push_back(next);
                }
            }
        }
    }

    None
}


#[repr(C)]
struct Winsize
{
    ws_row: u16,
    ws_col: u16,
    ws_xpixel: u16,
    ws_ypixel: u16,
}

const TIOCGWINSZ: u64 = 0x5413; // from <asm-generic/ioctls.h>
const STDOUT_FILENO: i32 = 1;

unsafe extern "C"
{
	fn ioctl(fd: i32, request: u64, ...) -> i32;
}

fn ternimal_size() -> (u16, u16)
{
	use std::mem;
	unsafe
	{
		let mut ws: Winsize = mem::zeroed();
		if ioctl(STDOUT_FILENO, TIOCGWINSZ, &mut ws) == -1
		{
			panic!("something went wrong with getting the terminal size");
		} else {
			return (ws.ws_row, ws.ws_col)
		}
	}
}


pub fn first_print_map<T: AsRef<[char]>>(map: &[T])
{
	let mut stdout = std::io::stdout();
	println!();
	while ternimal_size() < (map.len() as u16, map[0].as_ref().len() as u16)
	{
		print!("\rMake the terminal bigger");
		stdout.flush().unwrap();
		std::thread::sleep(std::time::Duration::from_millis(250));
	}

	print!("\x1b[2J"); // Clear entire screen
	print!("\x1b[H");  // Move cursor to top-left


	for (y,l) in map.iter().enumerate()
	{
		for (x, &c) in l.as_ref().iter().enumerate()
		{
			print!("\x1b[{};{}H\x1b[37;40m{}", y + 1, x + 1, c);
		}
	}

	print!("\x1b[0m");
	stdout.flush().unwrap();
	println!();

	//update_cell(map, Pos { y: 3, x: 5 }, 37, 41);
}

pub fn update_cell<T: AsRef<[char]>>(map: &[T], pos: Pos, fg: u8, bg: u8) 
{
    let mut stdout = std::io::stdout();

	print!("\x1b[s"); // save cursor position

    // Move cursor to row+1, col+1 (ANSI is 1-indexed), set fg/bg, print char, reset
    print!("\x1b[{};{}H\x1b[{};{}m{}\x1b[0m", pos.y + 1, pos.x + 1, fg, bg, map[pos.y].as_ref()[pos.x]);

	print!("\x1b[u"); //return to saved cursor position
    stdout.flush().unwrap();
	//std::thread::sleep(std::time::Duration::from_millis(20));
}
