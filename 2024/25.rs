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

    let _ = part1(&file_v);
}

fn part1(file: &Vec<String>)
{
    let mut keys: Vec<[i8; 5]> = Vec::new();
    let mut locks: Vec<[i8; 5]> = Vec::new();
    let schemactis: Vec<&[String]>= file.split(|line| { line == ""}).collect::<Vec<_>>();

    for schematic in schemactis
    {
        let mut k: [i8; 5] = [0; 5];
        let ch = schematic[0].chars().nth(0).unwrap();
        for (i, s) in schematic.iter().enumerate()
        {
            for (j, c) in s.chars().enumerate()
            {
                if c == ch
                {
                    k[j] = i as i8;
                }
            }
        }
        if ch == '#'
        {
            keys.push(k);
        } else {
            locks.push(k);
        }
    }

    let mut sum: i64 = 0;
    for &key in &keys
    {
        'next: for &lock in &locks
        {
            for (&k, l) in key.iter().zip(lock)
            {
                if k > l { continue 'next; }
            }
            sum += 1;
        }
    }
    println!("Part 1: {}", sum);
}
