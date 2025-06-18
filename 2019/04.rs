// 1548 too high

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

    let _ = part1(&file_v[0]);
    let _ = part2(&file_v[0]);

    Ok(())
}

fn part1(_file: &str) -> io::Result<()>
{
    let range: Vec<i64> = _file.split('-').map(|part| part.parse::<i64>().expect("TF")).collect();
    let mut answer = 0;
    'password_loop: for password in range[0]..range[1]
    {
        let mut numbers: [i64; 6] = [0; 6];
        let mut pass = password;
        numbers[0] = pass % 10;
        pass = pass / 10;
        numbers[1] = pass % 10;
        pass = pass / 10;
        numbers[2] = pass % 10;
        pass = pass / 10;
        numbers[3] = pass % 10;
        pass = pass / 10;
        numbers[4] = pass % 10;
        pass = pass / 10;
        numbers[5] = pass % 10;

        let mut double_digits: bool = false;
        for i in 0..5
        {
            if numbers[i] < numbers[i + 1]
            {
                continue 'password_loop;
            }
            if numbers[i] == numbers[i + 1]
            {
                if i > 0
                {
                    if numbers[i - 1] == numbers[i]
                    {
                        continue;
                    }
                }
                if i < 4
                {
                    if numbers[i + 2] == numbers[i]
                    {
                        continue;
                    }
                }
                double_digits = true;
            }
        }

        if !double_digits // should be not double digits
        {
            continue 'password_loop;
        }

        //println!("{}", password);
        answer += 1;
    }

    println!("part1: {}", answer);
    Ok(())
}

fn part2(_file: &str) -> io::Result<()>
{

    Ok(())
}
