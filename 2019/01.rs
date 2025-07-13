use std::{fs, io::{self, BufRead}};

pub fn setup(year: &str, day: &str) -> io::Result<()>
{
    let path = format!("{}/txt/{}.txt", year, day);
    let file = fs::File::open(path)?;
    let reader = io::BufReader::new(file);

    let mut file_v: Vec<String> = Vec::new();
    for line in reader.lines()
    {
        let line = line?;
        file_v.push(line);
    }

    let _ = part1(&file_v);
    let _ = part2(&file_v);

    Ok(())
}

fn part1(file: &Vec<String>) -> io::Result<()>
{
    let mut total_fuel: i64 = 0;
    for line in file
    {
        let mass: i64 = line.parse::<i64>().unwrap();
        let fuel: i64 = mass / 3 - 2;
        total_fuel += fuel;
    }
    println!("part1: {}", total_fuel);

    Ok(())
}

fn part2(file: &Vec<String>) -> io::Result<()>
{
    let mut total_fuel: i64 = 0;
    for line in file
    {
        let mass: i64 = line.parse::<i64>().unwrap();
        let mut fuel: i64 = mass / 3 - 2;
        while fuel > 0
        {
            total_fuel += fuel;
            fuel = fuel / 3 - 2
        }
    }


    println!("part2: {}", total_fuel);
    Ok(())
}
