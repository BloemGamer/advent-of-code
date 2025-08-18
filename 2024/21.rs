use std::{fs, io::{self, BufRead}};

const POSITION_A: (i8, i8) = (0, 2);
const POSITION_R: (i8, i8) = (1, 2);
const POSITION_L: (i8, i8) = (1, 0);
const POSITION_U: (i8, i8) = (0, 1);
const POSITION_D: (i8, i8) = (1, 1);

thread_local!
{
    static CACHE: std::cell::RefCell<std::collections::HashMap<(Vec<(i8, i8)>, i8), usize>> = std::cell::RefCell::new(std::collections::HashMap::new());
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
    let mut sum: i64 = 0;
    for input in file
    {
        let mut remaining: [(i8, i8); 4] = [(0, 0); 4];
        for (i, c) in input.chars().enumerate()
        {
            remaining[i] = match c
            {
                'A' => (0, 2),
                '0' => (0, 1),
                '1' => (-1, 0),
                '2' => (-1, 1),
                '3' => (-1, 2),
                '4' => (-2, 0),
                '5' => (-2, 1),
                '6' => (-2, 2),
                '7' => (-3, 0),
                '8' => (-3, 1),
                '9' => (-3, 2),
                _ => unreachable!(),
            }
        }
        let amount_steps: i64 = calc_steps_numpad(&remaining, &Vec::new(), 2, 0, POSITION_A) as i64;
        let complexity: i64 = input.chars().take(3).collect::<String>().parse().unwrap();
        sum += amount_steps * complexity;
        println!("{}, {}", amount_steps, complexity);
    }
    println!("Part 1: {}\n", sum);
}

fn part2(file: &Vec<String>)
{
    CACHE.with_borrow_mut(|cache|
        {
            cache.clear();
        }
    );


    let mut sum: i64 = 0;
    for input in file
    {
        let mut remaining: [(i8, i8); 4] = [(0, 0); 4];
        for (i, c) in input.chars().enumerate()
        {
            remaining[i] = match c
            {
                'A' => (0, 2),
                '0' => (0, 1),
                '1' => (-1, 0),
                '2' => (-1, 1),
                '3' => (-1, 2),
                '4' => (-2, 0),
                '5' => (-2, 1),
                '6' => (-2, 2),
                '7' => (-3, 0),
                '8' => (-3, 1),
                '9' => (-3, 2),
                _ => unreachable!(),
            }
        }
        let amount_steps: i64 = calc_steps_numpad(&remaining, &Vec::new(), 25, 0, POSITION_A) as i64;
        let complexity: i64 = input.chars().take(3).collect::<String>().parse().unwrap();
        sum += amount_steps * complexity;
        println!("{}, {}", amount_steps, complexity);
    }
    println!("Part 2: {}", sum);
}

fn calc_steps(remaining: &[(i8, i8)], so_far: &[(i8, i8)], max_robots: i8, robot: i8, (y, x): (i8, i8)) -> usize
{
    if (y, x) == (0, 0) { return usize::MAX }
    if remaining.len() == 0
    {
        return if robot == max_robots { so_far.len() }
        else
        {
            let parts = so_far.split(|pos| { *pos == POSITION_A } );
            let mut sum: usize = 0;
            let len = parts.clone().count();
            for (i, p) in parts.enumerate()
            {
                let mut part = p.to_vec();
                if i != len - 1 { part.push(POSITION_A); }

                sum += if let Some(value) = CACHE.with_borrow(|cache| 
                    { cache.get(&(part.clone(), max_robots - robot)).cloned() }
                )
                { value }
                else 
                { 
                    let value = calc_steps(&part, &Vec::new(), max_robots, robot + 1, POSITION_A);

                    CACHE.with_borrow_mut(|cache|
                        {
                            cache.insert((part, max_robots - robot), value)
                        }
                    );

                    value
                }
            }
            sum
        };
    }

    if (y, x) == remaining[0]
    {
        let mut new_remaining = remaining.to_vec();
        new_remaining.remove(0);
        let mut new_so_far = so_far.to_vec();
        new_so_far.push(POSITION_A);
        return calc_steps(&new_remaining, &new_so_far, max_robots, robot, (y, x));
    }
    let (ny, nx) = remaining[0];
    let mut answers: [usize; 4] = [usize::MAX; 4];
    if ny > y 
    {
        let mut new_so_far = so_far.to_vec();
        let dy = ny.abs_diff(y);
        for _ in 0..dy { new_so_far.push(POSITION_D); }
        answers[0] = calc_steps(remaining, &new_so_far, max_robots, robot, (y + dy as i8, x));
    }
    if ny < y 
    {
        let mut new_so_far = so_far.to_vec();
        let dy = ny.abs_diff(y);
        for _ in 0..dy { new_so_far.push(POSITION_U); }
        answers[1] = calc_steps(remaining, &new_so_far, max_robots, robot, (y - dy as i8, x));
    }
    if nx > x 
    {
        let mut new_so_far = so_far.to_vec();
        let dx = nx.abs_diff(x);
        for _ in 0..dx { new_so_far.push(POSITION_R); }
        answers[2] = calc_steps(remaining, &new_so_far, max_robots, robot, (y, x + dx as i8));
    }
    if nx < x 
    {
        let mut new_so_far = so_far.to_vec();
        let dx = nx.abs_diff(x);
        for _ in 0..dx { new_so_far.push(POSITION_L); }
        answers[3] = calc_steps(remaining, &new_so_far, max_robots, robot, (y, x - dx as i8));
    }
    return *answers.iter().min().unwrap()
}

fn calc_steps_numpad(remaining: &[(i8, i8)], so_far: &[(i8, i8)], max_robots: i8, robot: i8, (y, x): (i8, i8)) -> usize
{
    if (y, x) == (0, 0) { return usize::MAX }
    if remaining.len() == 0
    {
        return calc_steps(so_far, &Vec::new(), max_robots, robot + 1, POSITION_A);
    }

    if (y, x) == remaining[0]
    {
        let mut new_remaining = remaining.to_vec();
        new_remaining.remove(0);
        let mut new_so_far = so_far.to_vec();
        new_so_far.push(POSITION_A);
        return calc_steps_numpad(&new_remaining, &new_so_far, max_robots, robot, (y, x));
    }
    let (ny, nx) = remaining[0];
    let mut answers: [usize; 4] = [usize::MAX; 4];
    if ny > y 
    {
        let mut new_so_far = so_far.to_vec();
        let dy = ny.abs_diff(y);
        for _ in 0..dy { new_so_far.push(POSITION_D); }
        answers[0] = calc_steps_numpad(remaining, &new_so_far, max_robots, robot, (y + dy as i8, x));
    }
    if ny < y 
    {
        let mut new_so_far = so_far.to_vec();
        let dy = ny.abs_diff(y);
        for _ in 0..dy { new_so_far.push(POSITION_U); }
        answers[1] = calc_steps_numpad(remaining, &new_so_far, max_robots, robot, (y - dy as i8, x));
    }
    if nx > x 
    {
        let mut new_so_far = so_far.to_vec();
        let dx = nx.abs_diff(x);
        for _ in 0..dx { new_so_far.push(POSITION_R); }
        answers[2] = calc_steps_numpad(remaining, &new_so_far, max_robots, robot, (y, x + dx as i8));
    }
    if nx < x 
    {
        let mut new_so_far = so_far.to_vec();
        let dx = nx.abs_diff(x);
        for _ in 0..dx { new_so_far.push(POSITION_L); }
        answers[3] = calc_steps_numpad(remaining, &new_so_far, max_robots, robot, (y, x - dx as i8));
    }
    return *answers.iter().min().unwrap()
}
