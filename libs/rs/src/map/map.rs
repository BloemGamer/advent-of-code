pub use aoc_macros::ToPos;

#[macro_export]
macro_rules! map_from_file
{
    ($vec:expr) =>
    {{
        $vec.iter().map(|s| s.chars().collect::<Vec<char>>()).collect::<Vec<Vec<char>>>()
    }};
}
pub use map_from_file;

pub const WEIGHT_MAP_NONE: Option<&[[i64; 0]]> = None::<&[[i64;0]]>;

pub const DIRECTIONS: [(isize, isize); 4] = [(1, 0), (0, 1), (-1, 0), (0, -1)];
pub const HORIZONTAL_DIRECTIONS: [(isize, isize); 2] = [DIRECTIONS[1], DIRECTIONS[3]];
pub const VERTICAL_DIRECTIONS: [(isize, isize); 2] = [DIRECTIONS[0], DIRECTIONS[2]];

pub trait ToPos 
{
    fn to_pos(&self) -> Pos;
}

#[allow(dead_code)]
#[derive(Debug, Default, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Pos
{
    pub y: usize,
    pub x: usize,
}


#[allow(dead_code)]
impl Pos
{
    pub fn new() -> Self
    {
        Default::default()
    }
}

impl ToPos for Pos
{
    fn to_pos(&self) -> Pos
    {
        *self
    }
}
