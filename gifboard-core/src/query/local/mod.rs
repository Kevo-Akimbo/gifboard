use std::{
    collections::BinaryHeap,
    fs::{self, DirEntry, ReadDir},
    io,
    os::unix::ffi::OsStrExt,
    path::{Path, PathBuf},
};

use cxx_qt_lib::QString;

mod fuzzy_find;

use crate::{config, query::Attachment};

struct DirWalker {
    dir_stack: Vec<ReadDir>,
}

impl DirWalker {
    fn read_dir<P: AsRef<Path>>(path: P) -> io::Result<Self> {
        Ok(DirWalker {
            dir_stack: vec![fs::read_dir(path)?],
        })
    }
}

impl Iterator for DirWalker {
    type Item = io::Result<DirEntry>;

    fn next(&mut self) -> Option<Self::Item> {
        loop {
            let dir = self.dir_stack.last_mut()?;
            match dir.next() {
                Some(Ok(entry)) => match entry.file_type() {
                    Ok(ft) if ft.is_dir() => match fs::read_dir(entry.path()) {
                        Ok(read_dir) => self.dir_stack.push(read_dir),
                        Err(e) => return Some(Err(e)),
                    },
                    Ok(ft) if !ft.is_dir() => return Some(Ok(entry)),
                    Err(e) => return Some(Err(e)),
                    _ => unreachable!(),
                },
                None => {
                    self.dir_stack.pop();
                }
                x => return x,
            };
        }
    }
}

struct FuzzyScore<T> {
    score: i32,
    item: T,
}

impl<T> std::cmp::PartialEq for FuzzyScore<T> {
    fn eq(&self, other: &Self) -> bool {
        self.score == other.score
    }
}
impl<T> std::cmp::Eq for FuzzyScore<T> {}

impl<T> std::cmp::PartialOrd for FuzzyScore<T> {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        Some(self.cmp(other))
    }
}

impl<T> std::cmp::Ord for FuzzyScore<T> {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        self.score.cmp(&other.score)
    }
}

fn fuzzy_match(
    heap: &mut BinaryHeap<FuzzyScore<PathBuf>>,
    scores: &mut Vec<fuzzy_find::Cell>,
    hgaps: &mut Vec<i32>,
    vgaps: &mut Vec<i32>,
    path: &Path,
    query_tokens: &[&str],
) {
    fn substitution(a: &u8, b: &u8) -> i32 {
        if (*a as char).eq_ignore_ascii_case(&(*b as char)) {
            3
        } else {
            -3
        }
    }

    fn boundary(a: Option<&u8>, b: &u8) -> bool {
        let boundary_bytes = " .-_)]:/".as_bytes();
        if let Some(a) = a {
            boundary_bytes.contains(a) && !boundary_bytes.contains(b)
        } else {
            true
        }
    }

    let mut score = 0;
    let haystack = path.as_os_str().as_bytes();
    for tok in query_tokens.iter() {
        let needle = tok.as_bytes();
        score += fuzzy_find::smith_waterman(
            substitution,
            boundary,
            haystack,
            needle,
            scores,
            vgaps,
            hgaps,
        );
        heap.push(FuzzyScore {
            score,
            item: path.to_path_buf(),
        });
    }
}

pub(crate) fn fetch_local(
    query: &QString,
    count: usize,
    page: usize,
) -> std::io::Result<Vec<Attachment>> {
    let query_string = query.to_string();
    let query_tokens = query_string.split_whitespace().collect::<Vec<_>>();
    let mut heap: BinaryHeap<FuzzyScore<PathBuf>> = BinaryHeap::new();
    let mut scores = vec![fuzzy_find::Cell::default(); 100];
    let mut hgaps = vec![0; 100];
    let mut vgaps = vec![0; 100];
    for path in config::read_config()?.local_file_paths {
        if path.is_file() {
            fuzzy_match(
                &mut heap,
                &mut scores,
                &mut hgaps,
                &mut vgaps,
                &path,
                &query_tokens,
            );
            continue;
        }

        for file in DirWalker::read_dir(path)? {
            fuzzy_match(
                &mut heap,
                &mut scores,
                &mut hgaps,
                &mut vgaps,
                &file?.path(),
                &query_tokens,
            );
        }
    }
    let char_count = query_string.chars().filter(|x| !x.is_whitespace()).count() as i32;
    Ok(heap
        .into_iter()
        .filter_map(|path| {
            if path.score / char_count >= 3 {
                Some(Attachment::LocalFile(path.item))
            } else {
                None
            }
        })
        .skip(page * count)
        .take(count)
        .collect())
}
