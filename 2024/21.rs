use std::{fs, io::{self, BufRead}};

const POSITION_A: (i64, i64) = (0, 2);
const POSITION_R: (i64, i64) = (1, 2);
const POSITION_L: (i64, i64) = (1, 0);
const POSITION_U: (i64, i64) = (0, 1);
const POSITION_D: (i64, i64) = (1, 1);

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
        let mut remaining: [(i64, i64); 4] = [(0, 0); 4];
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
        let amount_steps: i64 = calc_steps(&remaining, &Vec::new(), 2, 0, POSITION_A) as i64;
        let complexity: i64 = input.chars().take(3).collect::<String>().parse().unwrap();
        sum += amount_steps * complexity;
        println!("{}, {}", amount_steps, complexity);
    }
    println!("Part 1: {}", sum);
}

fn part2(file: &Vec<String>)
{
    let mut sum: i64 = 0;
    for input in file
    {
        let mut remaining: [(i64, i64); 4] = [(0, 0); 4];
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
        let amount_steps: i64 = calc_steps(&remaining, &Vec::new(), 25, 0, POSITION_A) as i64;
        let complexity: i64 = input.chars().take(3).collect::<String>().parse().unwrap();
        sum += amount_steps * complexity;
        println!("{}, {}", amount_steps, complexity);
    }
    println!("Part 2: {}", sum);
}

fn calc_steps(remaining: &[(i64, i64)], so_far: &[(i64, i64)], max_robots: i64, robot: i64, (y, x): (i64, i64)) -> usize
{
    if (y, x) == (0, 0) { return usize::MAX }
    if remaining.len() == 0
    {
        //println!("{}", robot);
        return if robot == max_robots { so_far.len() }
        else { calc_steps(so_far, &Vec::new(), max_robots, robot + 1, POSITION_A) };
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
        new_so_far.push(POSITION_D);
        answers[0] = calc_steps(remaining, &new_so_far, max_robots, robot, (y + 1, x));
    }
    if ny < y 
    {
        let mut new_so_far = so_far.to_vec();
        new_so_far.push(POSITION_U);
        answers[1] = calc_steps(remaining, &new_so_far, max_robots, robot, (y - 1, x));
    }
    if nx > x 
    {
        let mut new_so_far = so_far.to_vec();
        new_so_far.push(POSITION_R);
        answers[2] = calc_steps(remaining, &new_so_far, max_robots, robot, (y, x + 1));
    }
    if nx < x 
    {
        let mut new_so_far = so_far.to_vec();
        new_so_far.push(POSITION_L);
        answers[3] =  calc_steps(remaining, &new_so_far, max_robots, robot, (y, x - 1));
    }
    return *answers.iter().min().unwrap()
}
