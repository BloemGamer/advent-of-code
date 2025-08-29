#[macro_use]
mod macros;
#[macro_use]
pub mod map;

pub use aoc_macros;

pub enum WhichFile
{
    Main,
    Test(i8),
}

pub fn read_file(year: &str, day: &str, which_file: WhichFile) -> Vec<String>
{
    let path: String = match which_file
    {
        WhichFile::Main => format!("{}/txt/{}.txt", year, day),
        WhichFile::Test(test) => format!("{}/txt/{}.test{}.txt", year, day, test),
    };
    println!("{}", path);
    let file: std::fs::File = std::fs::File::open(path).expect("Can't open file");
    let reader: std::io::BufReader<std::fs::File> = std::io::BufReader::new(file);
    std::io::BufRead::lines(reader).collect::<std::io::Result<Vec<_>>>().unwrap()
}
