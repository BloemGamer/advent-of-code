use std::collections::{BinaryHeap};
use std::cmp::Reverse;



/// The whole dijkstra algoritm function that does everything, if you don't need customization
#[macro_export]
macro_rules! dijkstra_min_len
{
    ($map:ident, $start:expr, $end:expr) =>
    {{
        let intersections = $crate::map::find_intersections!($map, $start, $end);
        let lengths = $crate::map::find_length_intersections!($map, intersections);
        let min_length = $crate::map::dijkstra::find_min_steps(&lengths, &intersections, &$start, &$end);

        min_length
    }};
}
pub use dijkstra_min_len;



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

