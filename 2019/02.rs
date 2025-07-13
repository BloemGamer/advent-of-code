use std::{fs, io::{self, BufRead}};

pub fn setup(year: &str, day: &str) -> io::Result<()>
{
    let path: String = format!("{}/txt/{}.txt", year, day);
    let file: fs::File = fs::File::open(path)?;
    let reader: io::BufReader<fs::File> = io::BufReader::new(file);

    let mut file_v: Vec<String> = Vec::new();
    for line_result in reader.lines()
    {
        let line: String = line_result?;
        file_v.push(line);
    }

    let mut input: Vec<i64> = Vec::new();
    for ic in file_v[0].split(',')
    {
        input.push(ic.parse::<i64>().unwrap());
    }

    part1(&input);
    part2(&input);

    Ok(())
}

fn part1(ic: &Vec<i64>)
{
    
    let mut intcode: Vec<i64> = ic.clone();
    intcode[1] = 12;
    intcode[2] = 2;
    let _ = run_intcode(&mut intcode);
    
    println!("part1: {}", intcode[0]);
}

fn part2(ic: &Vec<i64>)
{
    'brute_force: for noun in 0..100
    {
        for verb in 0..100
        {
            let mut intcode: Vec<i64> = ic.clone();
            intcode[1] = noun;
            intcode[2] = verb;
            match run_intcode(&mut intcode)
            {
                Ok(_) => {
                    if intcode[0] == 19690720
                    {
                        println!("{}", 100 * noun + verb);
                        break 'brute_force
                    }
                },
                Err(_) => continue,
            }
        }
    }
}

fn run_intcode(intcode: &mut Vec<i64>) -> Result<(), ()>
{
    let mut places: [usize; 4] = [0; 4];
    let mut i: usize = 0;
    while i < intcode.len()
    {
        match intcode[i] % 100
        {
            1 => {get_places(i, intcode, &mut places); ic_add(&mut i, intcode, &places)},
            2 => {get_places(i, intcode, &mut places); ic_mult(&mut i, intcode, &places)},

            99 => break,
            _ => return Err(())
            
        }
    }

    Ok(())
}

fn get_places(i: usize, intcode: &mut Vec<i64>, places: &mut [usize; 4])
{
    places[0] = intcode[i] as usize;
    places[1] = intcode[i + 1] as usize;
    places[2] = intcode[i + 2] as usize;
    places[3] = intcode[i + 3] as usize;
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
