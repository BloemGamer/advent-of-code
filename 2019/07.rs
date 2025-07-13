use std::{fs, io::{self, BufRead}};
use std::collections::VecDeque;

use itertools::Itertools;

enum IcOutput
{
    WaitingOnInput(()),
    Halt(()),
}

struct Ic
{
    ic: Vec<i64>,
    i: usize,
    next_input: VecDeque<i64>,
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
    let mut max_output: i64 = 0;
    for perm in (0..5).permutations(5)
    {
        let [a, b, c, d, e] = <[i64; 5]>::try_from(perm).unwrap();
        let mut a_ic = Ic { ic: ic.clone(), i: 0, next_input: VecDeque::from([b]) };
        let mut b_ic = Ic { ic: ic.clone(), i: 0, next_input: VecDeque::from([c]) };
        let mut c_ic = Ic { ic: ic.clone(), i: 0, next_input: VecDeque::from([d]) };
        let mut d_ic = Ic { ic: ic.clone(), i: 0, next_input: VecDeque::from([e]) };
        let mut e_ic = Ic { ic: ic.clone(), i: 0, next_input: VecDeque::from([a, 0]) };

        let _ =  run_intcode(&mut a_ic, &mut e_ic.next_input).unwrap();
        let _ =  run_intcode(&mut b_ic, &mut a_ic.next_input).unwrap();
        let _ =  run_intcode(&mut c_ic, &mut b_ic.next_input).unwrap();
        let _ =  run_intcode(&mut d_ic, &mut c_ic.next_input).unwrap();
        let _ =  run_intcode(&mut e_ic, &mut d_ic.next_input).unwrap();
        max_output = std::cmp::max(max_output, *e_ic.next_input.front().unwrap());
    }

    println!("part 1: {}", max_output);
}

fn part2(ic: &Vec<i64>)
{
    let mut max_output: i64 = 0;
    for perm in (5..10).permutations(5)
    {
        let [a, b, c, d, e] = <[i64; 5]>::try_from(perm).unwrap();
        let mut a_ic = Ic { ic: ic.clone(), i: 0, next_input: VecDeque::from([b]) };
        let mut b_ic = Ic { ic: ic.clone(), i: 0, next_input: VecDeque::from([c]) };
        let mut c_ic = Ic { ic: ic.clone(), i: 0, next_input: VecDeque::from([d]) };
        let mut d_ic = Ic { ic: ic.clone(), i: 0, next_input: VecDeque::from([e]) };
        let mut e_ic = Ic { ic: ic.clone(), i: 0, next_input: VecDeque::from([a, 0]) };

        'feedback: loop
        {
            let _ =  run_intcode(&mut a_ic, &mut e_ic.next_input).unwrap();
            let _ =  run_intcode(&mut b_ic, &mut a_ic.next_input).unwrap();
            let _ =  run_intcode(&mut c_ic, &mut b_ic.next_input).unwrap();
            let _ =  run_intcode(&mut d_ic, &mut c_ic.next_input).unwrap();
            match run_intcode(&mut e_ic, &mut d_ic.next_input).unwrap()
            {
                IcOutput::WaitingOnInput(_) => {},
                IcOutput::Halt(_) => {break 'feedback},
            };
        }
        max_output = std::cmp::max(max_output, *e_ic.next_input.front().unwrap());
    }

    println!("part 2: {}", max_output);
}

fn run_intcode(intcode: &mut Ic, input: &mut VecDeque<i64>) -> Result<IcOutput, ()>
{
    let mut places: [usize; 4] = [0; 4];
    //let mut i: usize = 0;
    while intcode.i < intcode.ic.len()
    {
        match intcode.ic[intcode.i] % 100
        {
            1 => { get_places(intcode.i, &mut intcode.ic, &mut places, 3); ic_add(&mut intcode.i, &mut intcode.ic, &places); },
            2 => { get_places(intcode.i, &mut intcode.ic, &mut places, 3); ic_mult(&mut intcode.i, &mut intcode.ic, &places); },

            3 => { get_places(intcode.i, &mut intcode.ic, &mut places, 1);
                if !input.is_empty() {ic_input(&mut intcode.i, &mut intcode.ic, &places, input.pop_front().unwrap())} else {return Ok(IcOutput::WaitingOnInput(()))};},
            4 => { get_places(intcode.i, &mut intcode.ic, &mut places, 1); intcode.next_input.push_back(ic_output(&mut intcode.i, &mut intcode.ic, &places)); },

            5 => { get_places(intcode.i, &mut intcode.ic, &mut places, 2); ic_jump(&mut intcode.i, &mut intcode.ic, &places, true); },
            6 => { get_places(intcode.i, &mut intcode.ic, &mut places, 2); ic_jump(&mut intcode.i, &mut intcode.ic, &places, false); },
            7 => { get_places(intcode.i, &mut intcode.ic, &mut places, 3); ic_cmp(&mut intcode.i, &mut intcode.ic, &places, |a, b| a < b); },
            8 => { get_places(intcode.i, &mut intcode.ic, &mut places, 3); ic_cmp(&mut intcode.i, &mut intcode.ic, &places, |a, b| a == b); },

            99 => break,
            _ => return Err(())
        }
    }

    return Ok(IcOutput::Halt(()));
}

fn get_places(i: usize, intcode: &mut Vec<i64>, places: &mut [usize; 4], amount: i64)
{
    places[0] = intcode[i] as usize;

    let mut par: i64 = intcode[i] / 100;
    places[1] = if par % 10 == 0 { intcode[i + 1] as usize } else { i + 1 };

    if amount >= 2
    {
        par = par / 10;
        places[2] = if par % 10 == 0 { intcode[i + 2] as usize } else { i + 2 };
    }
    if amount >= 3
    {
        par = par / 10;
        places[3] = if par % 10 == 0 { intcode[i + 3] as usize } else { i + 3 };
    }
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


fn ic_output(i: &mut usize, intcode: &mut Vec<i64>, places: &[usize; 4]) -> i64
{
    *i += 2;
    return intcode[places[1]];
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


