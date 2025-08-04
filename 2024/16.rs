use std::{collections::VecDeque, fs, io::{self, BufRead}};

#[derive(Debug)]
struct Solver
{
    paths: ShortestPaths,
    map: Vec<Vec<usize>>,
    start: (usize, usize),
    end: (usize, usize),
}

impl Solver
{
    fn new() -> Self
    {
        Self { paths: ShortestPaths::new(), start: (0, 0), end: (0, 0), map: Vec::new() }
    }
}

#[derive(Debug)]
struct ShortestPaths
{
    lenght: usize,
    paths: Vec<Vec<Vec<usize>>>,
}


impl ShortestPaths
{
    fn new() -> Self
    {
        Self { lenght: usize::MAX, paths: Vec::new() }
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
    match run_solver(&mut solver)
    {
        Ok(_) => println!("Part 1: {}", solver.paths.lenght),
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

fn part2(_file: &Vec<String>)
{

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

fn run_solver(solver: &mut Solver) -> Result<(), ()>
{
    let mut queue: VecDeque<((usize, usize), usize, (i64, i64))> = VecDeque::new();
    queue.push_back((((solver.start.0 + 0, solver.start.1 + 1)), 1, (0, 1)));
    queue.push_back((((solver.start.0 - 1, solver.start.1 + 0)), 1001, (-1, 0)));
    solver.map[solver.start.0][solver.start.1] = 2;
    solver.map[solver.start.0 + 0][solver.start.1 + 1] = 1;
    solver.map[solver.start.0 - 1][solver.start.1 + 0] = 1001;


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

    solver.paths.lenght = solver.map[solver.end.0][solver.end.1];

    Ok(())
}
