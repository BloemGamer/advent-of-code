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

    parts(&file_v)?;

    Ok(())
}

fn parts(_file: &Vec<String>) -> io::Result<()>
{
    let mut wires: Vec<Vec<(i64, i64)>> = vec![Vec::new(), Vec::new()];
    for i in 0..2
    {
        let instructions: Vec<&str> = _file[i].split(',').collect();
        let mut directions: Vec<(i64, i64)> = Vec::new();
        for inst in instructions
        {
            let (dir_char, dist_str): (&str, &str) = inst.split_at(1);
            let distance: i64 = dist_str.parse::<i64>().unwrap();
            match dir_char
            {
                "R" => directions.push((0, distance)),
                "L" => directions.push((0, -1 * distance)),
                "D" => directions.push((distance, 0)),
                "U" => directions.push((-1 * distance, 0)),
                _ => panic!("help"),
            }
        }
        let mut y: i64 = 0;
        let mut x: i64 = 0;
        for (dy, dx) in directions.iter()
        {
            if *dy > 0
            {
                for _ in 0..dy.abs()
                {
                    y += 1;
                    wires[i].push((y, x));
                }
            }
            if *dy < 0
            {
                for _ in 0..dy.abs()
                {
                    y -= 1;
                    wires[i].push((y, x));
                }
            }
            if *dx > 0
            {
                for _ in 0..dx.abs()
                {
                    x += 1;
                    wires[i].push((y, x));
                }
            }
            if *dx < 0
            {
                for _ in 0..dx.abs()
                {
                    x -= 1;
                    wires[i].push((y, x));
                }
            }
        }
    }

    let mut min_dist1: i64 = i64::MAX;
    let mut min_dist2: i64 = i64::MAX;

    for (i, w) in wires[0].iter().enumerate()
    {
        for (j, w2) in wires[1].iter().enumerate()
        {
            if w.0 == w2.0 && w.1 == w2.1
            {
                let dist1 = w.0.abs() + w.1.abs();
                min_dist1 = std::cmp::min(min_dist1, dist1);

                let dist2: i64 = i as i64 + j as i64 + 2;
                min_dist2 = std::cmp::min(min_dist2, dist2);
            }
        }
    }

    println!("part1: {}", min_dist1);
    println!("part2: {}", min_dist2);

    Ok(())
}

