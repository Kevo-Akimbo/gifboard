use std::{
    collections::HashSet,
    fs,
    io::{self, BufRead, Write},
    path::PathBuf,
};

fn create_cache_dir() -> io::Result<PathBuf> {
    let cache_dir = xdg::BaseDirectories::new()
        .get_cache_home()
        .ok_or(io::Error::other("Home directory not found"))?
        .join("gifboard");
    if !cache_dir.exists() {
        std::fs::create_dir_all(&cache_dir)?;
    }
    Ok(cache_dir)
}

fn create_ignored_versions() -> io::Result<fs::File> {
    let cache_dir = create_cache_dir()?;
    let ignored_versions_path = cache_dir.join("ignored_versions");
    fs::OpenOptions::new()
        .read(true)
        .write(true)
        .create(true)
        .truncate(false)
        .open(ignored_versions_path)
}

pub fn get_ignored_versions() -> io::Result<HashSet<String>> {
    let file = create_ignored_versions()?;
    io::BufReader::new(file).lines().collect()
}

pub fn write_ignored_versions(versions: &HashSet<String>) -> io::Result<()> {
    let mut file = create_ignored_versions()?;
    for version in versions {
        file.write_all(version.as_bytes())?;
        file.write_all(b"\n")?;
    }
    Ok(())
}
