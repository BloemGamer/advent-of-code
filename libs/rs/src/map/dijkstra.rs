use std::collections::{BinaryHeap, VecDeque, HashMap, HashSet};
use std::cmp::Reverse;
use crate::map::{
    Pos, ToPos, DIRECTIONS, VERTICAL_DIRECTIONS, HORIZONTAL_DIRECTIONS
};



/// The whole dijkstra algoritm function that does everything, if you don't need customization
#[macro_export]
macro_rules! dijkstra_min_len
{
    ($map:ident, $start:expr, $end:expr) =>
    {{
        let intersections = $crate::map::dijkstra::find_intersections!($map, $start, $end);
        let lengths = $crate::map::dijkstra::find_length_intersections!($map, intersections);
        let min_length = $crate::map::dijkstra::find_min_steps(&lengths, &intersections, &$start, &$end);

        min_length
    }};
}
pub use dijkstra_min_len;


/// finds the intersections (or turns) of a given maze, and also pushes [`start`] and [`end`] to the vec so they can be find by the algoritms
#[macro_export]
macro_rules! find_intersections
{
    ($map:ident, $start:expr, $end:expr => $intersection_fn:expr => $ID:ty, $neighbors:path, $on_intersection:expr ) =>
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
        $crate::map::dijkstra::find_intersections!($map, $start, $end
            => $crate::map::dijkstra::intersection
            => $crate::map::Pos,
            $crate::map::dijkstra::neighbors,
            |intersection: &mut Vec<$crate::map::Pos>, id: $crate::map::Pos| { intersection.push(id) })
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
        $one_way:expr) =>
    {{
        let mut lengths: Vec<(($ID, $ID), i64)> = Vec::new();
        for (i, &start) in $intersections.iter().enumerate()
        {
            let mut queue: std::collections::VecDeque<($ID, i64)> = std::collections::VecDeque::new();
            let mut visited: Vec<$ID> = Vec::new();
            queue.push_back((start, 0));
            visited.push(start);
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
        }
        lengths
    }};

    ($map:ident, $weight_map:expr, $intersections:ident => $neighbors:path => $ID:ty) =>
    {{
        $crate::map::dijkstra::find_length_intersections!(
            $map, $weight_map, $intersections =>
            $neighbors =>
            $ID,
            false)
    }};

    ($map:ident, $intersections:ident) =>
    {{
        $crate::map::dijkstra::find_length_intersections!(
            $map, None::<&[&[i64]]>, $intersections
            => $crate::map::dijkstra::neighbors
            => $crate::map::Pos,
            false)
    }};
}
pub use find_length_intersections;

// The main dijkstra algoritm without backtrace enabled
pub fn find_min_steps<ID: Ord + Copy>(lengths: &Vec<((ID, ID), i64)>, intersections: &Vec<ID>, start: &[ID], end: &[ID]) -> i64
{
    let mut min_length: i64 = i64::MAX;
    let mut queue: BinaryHeap<Reverse<(i64, ID)>> = BinaryHeap::new();
    let mut completed: Vec<ID> = Vec::new();
    let mut points: Vec<(i64, ID)> = intersections.iter().map(|&inter| 
        {
            if start.contains(&inter) { (0, inter) }
            else { (i64::MAX, inter) }
        })
        .collect();
    for &s in start
    {
        queue.push(Reverse((0i64, s)));
    }

    'dijkstra: while let Some(Reverse((length, id))) = queue.pop()
    {
        if let Some(&(current_dist, _)) = points.iter().find(|(_, pos)| *pos == id)
        {
            if length > current_dist
            {
                continue 'dijkstra;
            }
        }
        debug_assert!(!completed.contains(&id));
        if end.contains(&id) { min_length = length; break 'dijkstra }
        completed.push(id);
        
        let next: Vec<(i64, ID)> = lengths
            .iter()
            .filter_map(|&((a, b), l)|
            {
                if a == id { Some((l, b)) }
                else { None }
            })
            .collect();

        for (l, n) in next
        {
            if let Some((lc, pos)) = points.iter_mut().find(|&&mut(_, pos)| pos == n)
            {
                if *lc > length + l
                {
                    *lc = length + l;
                    queue.push(Reverse((*lc, *pos)));
                }
            }
        }
    }

    return min_length
}

/// The main dijkstra algoritm, with backtrace enabled
pub fn find_fastest_path<ID: Ord + Copy + Default>(lengths: &Vec<((ID, ID), i64)>, intersections: &Vec<ID>, start: &[ID], end: &[ID]) -> (i64, Vec<(i64, ID, Vec<ID>)>)
{
    let mut min_length: i64 = i64::MAX;
    let mut queue: BinaryHeap<Reverse<(i64, ID)>> = BinaryHeap::new();
    let mut completed: Vec<ID> = Vec::new();
    let mut points: Vec<(i64, ID, Vec<ID>)> = intersections.iter().map(|&inter| 
        {
            if start.contains(&inter) { (0, inter, Vec::new()) }
            else { (i64::MAX, inter, Vec::new()) }
        })
        .collect();
    for &s in start
    {
        queue.push(Reverse((0i64, s)));
    }

    'dijkstra: while let Some(Reverse((length, id))) = queue.pop()
    {
        if let Some(&(current_dist, _, _)) = points.iter().find(|(_, pos, _)| *pos == id)
        {
            if length > current_dist
            {
                continue 'dijkstra;
            }
        }
        debug_assert!(!completed.contains(&id));
        if end.contains(&id) { min_length = length.min(min_length) }
        if min_length < length { break 'dijkstra }
        completed.push(id);
        
        let next: Vec<(i64, ID)> = lengths
            .iter()
            .filter_map(|&((a, b), l)|
            {
                if a == id { Some((l, b)) }
                else { None }
            })
            .collect();

        for (l, n) in next
        {
            if let Some((lc, pos, paths)) = points.iter_mut().find(|&&mut(_, pos, _)| pos == n)
            {
                if *lc > length + l
                {
                    *lc = length + l;

                    paths.clear();
                    paths.push(id);

                    queue.push(Reverse((*lc, *pos)));
                }
                if *lc == length + l
                {
                    paths.push(id);
                }
            }
        }
    }

    return (min_length, points)
}

// this one prob needs it made, so it can do oneways too and like that, I don't want to do that
// right now tbh
/// This one does not work if you have the intersections function, not map at least all the intersections
pub fn all_fastest_paths<ID>(backtrace: &[(ID, Vec<ID>)], end: &[ID], map: &[Vec<char>]) -> Vec<Vec<bool>>
where 
    ID: ToPos + Eq + Copy
{
    let mut map_speed: Vec<Vec<bool>> = vec![vec![false; map[0].len()]; map.len()];
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
        for f in from
        {
            if !visited.contains(&f)
            {
                let path = shortest_path(map, id.to_pos(), f.to_pos()).expect("TF");
                for p in path
                {
                    map_speed[p.y][p.x] = true;
                }
                if !visited.contains(&id) { visited.push(*id); }
                if !visited.contains(f) { visited.push(*f); }
                let next = backtrace.iter().find(|(x, _)| *x == *f).expect("how than");
                queue.push_back(next);
            }
        }
    }

    return map_speed
}


fn shortest_path(grid: &[Vec<char>], start: Pos, end: Pos) -> Option<Vec<Pos>>
{
    let rows = grid.len();
    let cols = grid[0].len();

    let mut queue = VecDeque::new();
    let mut visited = HashSet::new();
    let mut parent: HashMap<Pos, Pos> = HashMap::new();

    queue.push_back(start);
    visited.insert(start);
    if start == end { return Some(vec![start]) }
    //println!("start cell: {}", grid[start.y][start.x]);
    //println!("end cell: {}", grid[end.y][end.x]);

    let directions = [(-1i32, 0), (1, 0), (0, -1), (0, 1)]; // 4-way connectivity

    while let Some(current) = queue.pop_front() {
        if current == end {
            // reconstruct path
            let mut path = vec![current];
            let mut cur = current;
            while let Some(&p) = parent.get(&cur) {
                cur = p;
                path.push(cur);
            }
            path.reverse();
            return Some(path);
        }

        for &(dy, dx) in &directions {
            let ny = current.y as i32 + dy;
            let nx = current.x as i32 + dx;

            if ny >= 0 && ny < rows as i32 && nx >= 0 && nx < cols as i32 {
                let next = Pos { y: ny as usize, x: nx as usize };
                // Check for wall using '#'
                if grid[next.y][next.x] != '#' && !visited.contains(&next) {
                    visited.insert(next);
                    parent.insert(next, current);
                    queue.push_back(next);
                }
            }
        }
    }

    None
}
