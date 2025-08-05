use std::{cell::RefCell, collections::VecDeque, fs, io::{self, BufRead}};

// 499, to high
// 485, to low

#[derive(Debug, Clone)]
struct Solver
{
    length: usize,
    map: Vec<Vec<usize>>,
    start: (usize, usize),
    end: (usize, usize),
}

impl Solver
{
    fn new() -> Self
    {
        Self { length: 0, start: (0, 0), end: (0, 0), map: Vec::new() }
    }
}

pub fn setup(year: &str, day: &str)
{
    let path: String = format!("{}/txt/{}.txt", year, day);
    println!("{}", path);
    let file: fs::File = fs::File::open(path).expect("Can't open file");
    let reader: io::BufReader<fs::File> = io::BufReader::new(file);

    let mut file_v: Vec<String> = Vec::new();
    for line_result in reader.lines()
    {
        let line: String = line_result.unwrap();
        file_v.push(line);
    }

    let _ = part1(&file_v);
    let _ = part2(&file_v);
}

fn part1(file: &Vec<String>)
{
    let mut solver = setup_solver(&file);
    match 
    {
        let mut queue: VecDeque<((usize, usize), usize, (i64, i64))> = VecDeque::new();
        queue.push_back((((solver.start.0 + 0, solver.start.1 + 1)), 1, (0, 1)));
        queue.push_back((((solver.start.0 - 1, solver.start.1 + 0)), 1001, (-1, 0)));
        solver.map[solver.start.0][solver.start.1] = 2;
        solver.map[solver.start.0 + 0][solver.start.1 + 1] = 1;
        solver.map[solver.start.0 - 1][solver.start.1 + 0] = 1001;
        solve(&mut solver, queue)
    }
    {
        Ok(_) => println!("Part 1: {}", solver.length),
        Err(_) =>
        {
            for row in &solver.map
            {
                for &val in row
                {
                    let ch = match val
                    {
                        0 => '.',
                        usize::MAX => '#',
                        _ => '/',
                    };
                    print!("{}", ch);
                }
                println!();
            }
        },
    }
}

fn part2(file: &Vec<String>)
{
    let solver_base = setup_solver(&file);
    let mut solver1 = 
    {
        let mut solver = solver_base.clone();
        let mut queue: VecDeque<((usize, usize), usize, (i64, i64))> = VecDeque::new();
        queue.push_back((((solver.start.0 + 0, solver.start.1 + 1)), 1, (0, 1)));
        queue.push_back((((solver.start.0 - 1, solver.start.1 + 0)), 1001, (-1, 0)));
        solver.map[solver.start.0][solver.start.1] = 2;
        solver.map[solver.start.0 + 0][solver.start.1 + 1] = 1;
        solver.map[solver.start.0 - 1][solver.start.1 + 0] = 1001;
        solve(&mut solver, queue).unwrap();
        solver.map[solver.start.0][solver.start.1] = 0;
        solver
    };
    let mut queue: VecDeque<((usize, usize), usize, (i64, i64))> = VecDeque::new();
    //queue.push_back((((solver.start.0 + 0, solver.start.1 + 1)), 1, (0, 1)));
    queue.push_back((((solver1.start.0 - 1, solver1.start.1 + 0)), 1001, (-1, 0)));

    let new_map = find_all_paths(&mut solver1);
    
    let mut count: u64 = 0;
    for row in &new_map
    {
        for &val in row
        {
            let _ch = match val
            {
                0 => '.',
                usize::MAX => '#',
                _ => {count += 1; 'O'},
            };
            //print!("{}", _ch);
        }
        //println!();
    }

    println!("Part 2: {count}");
}


fn setup_solver(map_given: &Vec<String>) -> Solver
{
    let mut solver = Solver::new();
    //let mut visited: Vec<(i64, i64)> = Vec::new();
    let mut map: Vec<Vec<usize>> = Vec::new();
    for (i, row) in map_given.iter().enumerate()
    {
        map.push(Vec::new());
        for (j, cell)  in row.chars().enumerate()
        {
            match cell
            {
                '#' => map[i].push(usize::MAX),
                'S' => { map[i].push(0); solver.start = (i, j)},
                'E' => { map[i].push(0); solver.end = (i, j)},
                '.' => map[i].push(0),

                _ => unreachable!(),
            }
        }
    }

    solver.map = map;
    solver
}

fn solve(solver: &mut Solver, mut queue: VecDeque<((usize, usize), usize, (i64, i64))>) -> Result<(), ()>
{
    while !queue.is_empty()
    {
        let ((y, x), length, (diry, dirx)) = queue.pop_back().ok_or(())?;
        if x == 0 || y == 0 {panic!()}

        let next1 = solver.map[(y as i64 + diry) as usize][(x as i64 + dirx) as usize];
        if (next1 > length + 1 || next1 == 0) && next1 != usize::MAX
        {
            solver.map[(y as i64 + diry) as usize][(x as i64 + dirx) as usize] = length + 1;
            queue.push_back(((((y as i64 + diry) as usize, (x as i64 + dirx) as usize)), length + 1, (diry, dirx)));
        }

        let next2 = solver.map[(y as i64 + dirx) as usize][(x as i64 + diry) as usize];
        if (next2 > length + 1001 || next2 == 0) && next2 != usize::MAX
        {
            solver.map[(y as i64 + dirx) as usize][(x as i64 + diry) as usize] = length + 1001;
            queue.push_back(((((y as i64 + dirx) as usize, (x as i64 + diry) as usize)), length + 1001, (dirx, diry)));
        }

        let next3 = solver.map[(y as i64 - dirx) as usize][(x as i64 - diry) as usize];
        if (next3 > length + 1001 || next3 == 0) && next3 != usize::MAX
        {
            solver.map[(y as i64 - dirx) as usize][(x as i64 - diry) as usize] = length + 1001;
            queue.push_back(((((y as i64 - dirx) as usize, (x as i64 - diry) as usize)), length + 1001, (-dirx, -diry)));
        }
    }

    solver.length = solver.map[solver.end.0][solver.end.1];

    Ok(())
}



fn find_all_paths(solver: &mut Solver) -> Vec<Vec<usize>>
{
    let map: std::rc::Rc<RefCell<Vec<Vec<usize>>>> = std::rc::Rc::new(RefCell::new(vec![vec![0; solver.map.len()]; solver.map[1].len()]));
    {
        let mut map_tmp = map.borrow_mut();
        map_tmp[solver.end.0][solver.end.1] = solver.length;
    }
    
    step_straight(&map, &solver.map, solver.end.0, solver.end.1, 1, 0, solver.length);
    step_straight(&map, &solver.map, solver.end.0, solver.end.1, 0, -1, solver.length);


    let mut map_ret = map.take();
    map_ret[solver.start.0][solver.start.1] = 1;
    map_ret
}

fn step_straight(map_rc: &std::rc::Rc<RefCell<Vec<Vec<usize>>>>, read_map: &Vec<Vec<usize>>, y: usize, x: usize, diry: i64, dirx: i64, length: usize)
{
    if length == 0 {return}
    let next = read_map[(y as i64 + diry) as usize][(x as i64 + dirx) as usize];
    if next == length - 1
    {
        {
            let mut map = map_rc.borrow_mut();
            map[(y as i64 + diry) as usize][(x as i64 + dirx) as usize] = length - 1;
        }
        step_straight(map_rc, read_map, (y as i64 + diry) as usize, (x as i64 + dirx) as usize, diry, dirx, length - 1);
    }

    else if next == length - 1001
    {
        {
            let mut map = map_rc.borrow_mut();
            map[(y as i64 + diry) as usize][(x as i64 + dirx) as usize] = length - 1001;
        }
        step_not_straight(map_rc, read_map, (y as i64 + diry) as usize, (x as i64 + dirx) as usize, diry, dirx, length - 1001);
    }
}


fn step_not_straight(map_rc: &std::rc::Rc<RefCell<Vec<Vec<usize>>>>, read_map: &Vec<Vec<usize>>, y: usize, x: usize, diry: i64, dirx: i64, length: usize)
{
    if length == 0
    {
        let mut map = map_rc.borrow_mut();
        map[y as usize][x as usize] = 1;
        return;
    }
    let next1 = read_map[(y as i64 + diry) as usize][(x as i64 + dirx) as usize];
    if next1 == length + 999
    {
        {
            let mut map = map_rc.borrow_mut();
            map[(y as i64 + diry) as usize][(x as i64 + dirx) as usize] = length - 1;
        }
        step_straight(map_rc, read_map, (y as i64 + diry) as usize, (x as i64 + dirx) as usize, diry, dirx, length + 999);
    }
    let next2 = read_map[(y as i64 + dirx) as usize][(x as i64 + diry) as usize];
    if next2 == length - 1
    {
        {
            let mut map = map_rc.borrow_mut();
            map[(y as i64 + dirx) as usize][(x as i64 + diry) as usize] = length - 1;
        }
        step_straight(map_rc, read_map, (y as i64 + dirx) as usize, (x as i64 + diry) as usize, dirx, diry, length - 1);
    }
    
    let next3 = read_map[(y as i64 - dirx) as usize][(x as i64 - diry) as usize];
    if next3 == length - 1
    {
        {
            let mut map = map_rc.borrow_mut();
            map[(y as i64 - dirx) as usize][(x as i64 - diry) as usize] = length - 1;
        }
        step_straight(map_rc, read_map, (y as i64 - dirx) as usize, (x as i64 - diry) as usize, -dirx, -diry, length - 1);
    }
}
