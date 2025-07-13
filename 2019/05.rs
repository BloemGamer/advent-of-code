use std::{fs, io::{self, BufRead}};


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

    let mut input: Vec<i64> = Vec::new();
    for ic in file_v[0].split(',')
    {
        input.push(ic.parse::<i64>().unwrap());
    }

    part1(&input);
    part2(&input);
}

fn part1(ic: &Vec<i64>)
{
    println!("part 1: ");
    let mut intcode: Vec<i64> = ic.clone();
    let _ = run_intcode(&mut intcode, 1);
    println!("");
    
}

fn part2(ic: &Vec<i64>)
{
    println!("part 2: ");
    let mut intcode: Vec<i64> = ic.clone();
    let _ = run_intcode(&mut intcode, 5);
    println!("");
}

fn run_intcode(intcode: &mut Vec<i64>, input: i64) -> Result<(), ()>
{
    let mut places: [usize; 4] = [0; 4];
    let mut i: usize = 0;
    while i < intcode.len()
    {
        match intcode[i] % 100
        {
            1 => { get_places(i, intcode, &mut places); ic_add(&mut i, intcode, &places) },
            2 => { get_places(i, intcode, &mut places); ic_mult(&mut i, intcode, &places) },
            3 => { get_places(i, intcode, &mut places); ic_input(&mut i, intcode, &places, input) },
            4 => { get_places(i, intcode, &mut places); ic_output(&mut i, intcode, &places) },
            5 => { get_places(i, intcode, &mut places); ic_jump(&mut i, intcode, &places, true) },
            6 => { get_places(i, intcode, &mut places); ic_jump(&mut i, intcode, &places, false) },
            7 => { get_places(i, intcode, &mut places); ic_cmp(&mut i, intcode, &places, |a, b| a < b) },
            8 => { get_places(i, intcode, &mut places); ic_cmp(&mut i, intcode, &places, |a, b| a == b) },

            99 => break,
            _ => return Err(())
        }
    }

    Ok(())
}

fn get_places(i: usize, intcode: &mut Vec<i64>, places: &mut [usize; 4])
{
    places[0] = intcode[i] as usize;

    let mut par: i64 = intcode[i] / 100;
    places[1] = if par % 10 == 0 { intcode[i + 1] as usize } else { i + 1 };

    par = par / 10;
    places[2] = if par % 10 == 0 { intcode[i + 2] as usize } else { i + 2 };

    par = par / 10;
    places[3] = if par % 10 == 0 { intcode[i + 3] as usize } else { i + 3 };
}

fn ic_add(i: &mut usize, intcode: &mut Vec<i64>, places: &[usize; 4])
{
    intcode[places[3]] = intcode[places[1]] + intcode[places[2]];
    *i += 4;
}

fn ic_mult(i: &mut usize, intcode: &mut Vec<i64>, places: &[usize; 4])
{
    intcode[places[3]] = intcode[places[1]] * intcode[places[2]];
    *i += 4;
}

fn ic_input(i: &mut usize, intcode: &mut Vec<i64>, places: &[usize; 4], input: i64)
{
    intcode[places[1]] = input;
    *i += 2;
}


fn ic_output(i: &mut usize, intcode: &mut Vec<i64>, places: &[usize; 4])
{
    print!("{} ", intcode[places[1]]);
    *i += 2;
}


fn ic_jump(i: &mut usize, intcode: &mut Vec<i64>, places: &[usize; 4], jump_if: bool)
{
    if (intcode[places[1]] != 0) == jump_if
    {
        *i = intcode[places[2]] as usize;
    }
    else
    {
        *i += 3;
    }
}

fn ic_cmp<F>(i: &mut usize, intcode: &mut Vec<i64>, places: &[usize; 4], cmp: F)
where F: Fn(i64, i64) -> bool,
{
    if cmp(intcode[places[1]], intcode[places[2]])
    {
        intcode[places[3]] = 1;
    }
    else
    {
        intcode[places[3]] = 0;
    }
    *i += 4;
}


