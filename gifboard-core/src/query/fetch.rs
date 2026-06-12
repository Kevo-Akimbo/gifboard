use std::path::PathBuf;

use cxx_qt_lib::QString;

use crate::{
    config,
    query::{klippy::fetch_from_klippy, local::fetch_local},
};

// RawJpg refers to `blur_preview` in the `Attachment` struct
#[derive(Debug)]
pub enum LinkType {
    Url(String),
    RawJpg,
}

#[derive(Debug)]
pub struct KlippySource {
    pub output_uri: LinkType,
    pub hover_uri: Option<LinkType>,
    pub preview_uri: LinkType,
    pub blur_preview: String,
    pub height: usize,
    pub width: usize,
}

impl KlippySource {
    pub fn new(
        output_uri: LinkType,
        hover_uri: Option<LinkType>,
        preview_uri: LinkType,
        blur_preview: String,
        height: usize,
        width: usize,
    ) -> Self {
        Self {
            output_uri,
            hover_uri,
            preview_uri,
            blur_preview,
            height,
            width,
        }
    }
}

#[derive(Debug)]
pub enum Attachment {
    Klippy(KlippySource),
    LocalFile(PathBuf),
}

pub async fn fetch_query(query: &QString, page: usize) -> std::io::Result<Vec<Attachment>> {
    let mut attachments = vec![];
    let config = config::read_config()?;
    attachments.append(&mut fetch_local(query, 10, page)?);
    attachments.append(&mut fetch_from_klippy(&config, page, &query.to_string()).await?);
    Ok(attachments)
}
