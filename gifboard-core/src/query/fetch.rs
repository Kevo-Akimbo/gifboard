use cxx_qt_lib::QString;

use crate::{config, query::klippy::fetch_from_klippy};

// RawJpg refers to `blur_preview` in the `Attachment` struct
#[derive(Debug)]
pub enum AttachmentType {
    Url(String),
    LocalFile(String),
    RawJpg,
}

#[derive(Debug)]
pub struct Attachment {
    pub output_uri: AttachmentType,
    pub hover_uri: Option<AttachmentType>,
    pub preview_uri: AttachmentType,
    pub blur_preview: String,
    pub height: usize,
    pub width: usize,
}

impl Attachment {
    /// output_uri, hover_uri, preview_uri, width, height
    pub fn to_received_result_item(self) -> (QString, QString, QString, u32, u32) {
        let output_uri = match self.output_uri {
            AttachmentType::Url(s) => QString::from(s),
            AttachmentType::LocalFile(s) => QString::from(s),
            AttachmentType::RawJpg => {
                panic!("Output type should not use blur_preview")
            }
        };

        let hover_uri = match self.hover_uri {
            Some(AttachmentType::Url(s)) => QString::from(s),
            Some(AttachmentType::LocalFile(s)) => QString::from(s),
            Some(AttachmentType::RawJpg) => QString::from(self.blur_preview.clone()),
            None => QString::default(),
        };

        let preview_uri = match self.preview_uri {
            AttachmentType::Url(s) => QString::from(s),
            AttachmentType::LocalFile(s) => QString::from(s),
            AttachmentType::RawJpg => QString::from(self.blur_preview),
        };

        (
            output_uri,
            hover_uri,
            preview_uri,
            self.width as u32,
            self.height as u32,
        )
    }
}

pub async fn fetch_query(query: &QString, page: usize) -> std::io::Result<Vec<Attachment>> {
    let mut attachments = vec![];
    let config = config::read_config()?;
    attachments.append(&mut fetch_from_klippy(&config, page, &query.to_string()).await?);
    Ok(attachments)
}
