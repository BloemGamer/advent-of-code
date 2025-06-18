use std::{fs, io::{self, BufRead}, collections::HashMap};

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

    let _ = part1(&file_v);
    let _ = part2(&file_v);

    Ok(())
}

fn part1(_file: &Vec<String>) -> io::Result<()>
{
    let mut map: HashMap<&str, &str> = HashMap::new();
    for line in _file
    {
        let objects: Vec<&str> = line.split(')').collect();
        map.entry(objects[1]).or_insert(objects[0]);
    }

    let mut orbits = 0;
    for (object, _next_object) in &map
    {
        let mut obj: &str = object;
        while obj != "COM"
        {
            obj = map[obj];
            orbits += 1;
        }
    }

    println!("{}", orbits);
    Ok(())
}

fn part2(_file: &Vec<String>) -> io::Result<()>
{

    Ok(())
}

