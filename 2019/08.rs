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
    let _ = part2(&file_v);
}

fn part1(file: &Vec<String>)
{
    let mut picture: Vec<&str> = Vec::new();   
    for i in 0..file[0].len() / (25 * 6)
    {
        picture.push(&file[0][(i * 25 * 6)..((i * 25 * 6) + (25 * 6))]);
    }

    let mut zero_count: i64 = i64::MAX;
    let mut least_zero: usize = 0;
    for (i, layer) in picture.iter().enumerate()
    {
        let mut current_zero_count = 0;
        for p in layer.chars()
        {
            if p == '0'
            {
                current_zero_count += 1;
            }
        }
        if current_zero_count < zero_count
        {
            least_zero = i;
            zero_count = current_zero_count;
        }
    }

    let mut one: i64 = 0;
    let mut two: i64 = 0;
    for p in picture[least_zero].chars()
    {
        match p
        {
            '1' => one += 1,
            '2' => two += 1,
            _ => {},
        }
    }
    println!("part1: {}", one * two);
}

fn part2(file: &Vec<String>)
{
    let mut picture: Vec<&str> = Vec::new();   
    for i in 0..file[0].len() / (25 * 6)
    {
        picture.push(&file[0][(i * 25 * 6)..((i * 25 * 6) + (25 * 6))]);
    }
    
    let mut final_picture: Vec<char> = vec!['2'; 25 * 6];

    for layer in picture.iter().rev()
    {
        for (i, c) in layer.chars().enumerate()
        {
            match c
            {
                '0' => final_picture[i] = ' ',
                '1' => final_picture[i] = '#',
                _ => {}
            }
        }
    }

    println!("part2:");

    let fp = final_picture.iter().collect::<String>();
    for line in fp.chars().collect::<Vec<_>>().chunks(25)
    {
        println!("{}", line.iter().collect::<String>());
    }
    

}

