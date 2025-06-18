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

    let mut map: HashMap<&str, &str> = HashMap::new();
    for line in &file_v
    {
        let objects: Vec<&str> = line.split(')').collect();
        map.entry(objects[1]).or_insert(objects[0]);
    }

    let _ = part1(&map);
    let _ = part2(&map);

    Ok(())
}

fn part1(map: &HashMap<&str, &str>) -> io::Result<()>
{

    let mut orbits = 0;
    for (object, _next_object) in map
    {
        let mut obj: &str = object;
        while obj != "COM"
        {
            obj = map[obj];
            orbits += 1;
        }
    }

    println!("part1: {}", orbits);
    Ok(())
}

fn part2(map: &HashMap<&str, &str>) -> io::Result<()>
{
    let mut paths: Vec<Vec<&str>> = vec![Vec::new(), Vec::new()];
    
    let mut obj: &str = "YOU";
    while obj != "COM"
    {
        obj = map[obj];
        paths[0].push(obj);
    }
    obj = "SAN";
    while obj != "COM"
    {
        obj = map[obj];
        paths[1].push(obj);
    }

    let mut common_orbit: &str = "COM";
    'low_trans: for orb0 in &paths[0]
    {
        for orb1 in &paths[1]
        {
            if orb0 == orb1
            {
                common_orbit = orb0;
                break 'low_trans;
            }
        }
    }

    let mut orbit_transfers: i64 = 0;
    for i in 0..2
    {
        for orb in &paths[i]
        {
            if *orb != common_orbit
            {
                orbit_transfers += 1;
            }
            else
            {
                break;
            }
        }
    }

    println!("part2: {}", orbit_transfers);

    Ok(())
}

