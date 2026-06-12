#[cxx_qt::bridge]
pub mod qobject {

    unsafe extern "C++" {
        include!("cxx-qt-lib/qstring.h");
        type QString = cxx_qt_lib::QString;

        include!("cxx-qt-lib/qsize.h");
        type QSize = cxx_qt_lib::QSize;

        include!("src/image_size.h");
        #[rust_name = "get_image_size"]
        fn getImageSize(filepath: &QString) -> QSize;
    }

    extern "RustQt" {
        #[qobject]
        #[qml_element]
        #[qproperty(usize, current_page)]
        type SearchResults = super::SearchResultsRust;

        #[qsignal]
        #[cxx_name = "queryError"]
        fn query_error(self: Pin<&mut Self>, error: QString);

        #[qinvokable]
        fn query(self: Pin<&mut Self>, query: QString, refresh: bool);

        #[qsignal]
        #[cxx_name = "receivedKlippy"]
        fn received_klippy(
            self: Pin<&mut Self>,
            output_uri: QString,
            hover_uri: QString,
            preview_uri: QString,
            blur_preview: QString,
            width: u32,
            height: u32,
        );

        #[qsignal]
        #[cxx_name = "receivedLocalImage"]
        fn received_local_image(self: Pin<&mut Self>, path: QString, size: QSize);

        #[qsignal]
        #[cxx_name = "receivedLocalFile"]
        fn received_local_file(self: Pin<&mut Self>, path: QString);
    }

    impl cxx_qt::Threading for SearchResults {}
}

#[derive(Default)]
pub struct KlippyAttachment {
    output_uri: QString,
    hover_uri: QString,
    preview_uri: QString,
    blur_preview: QString,
    width: u32,
    height: u32,
}

#[derive(Default)]
pub struct SearchResultsRust {
    pub current_page: usize,
}

use cxx_qt::{CxxQtType, Threading};
use cxx_qt_lib::QString;
use gifboard_core::query::{Attachment, KlippySource, LinkType};
use std::pin::Pin;

impl KlippyAttachment {
    fn from_attachment(attachment: KlippySource) -> KlippyAttachment {
        let output_uri = match attachment.output_uri {
            LinkType::Url(url) => QString::from(url),
            LinkType::RawJpg => QString::from(&attachment.blur_preview),
        };

        let hover_uri = match attachment.hover_uri {
            Some(LinkType::Url(url)) => QString::from(&url),
            Some(LinkType::RawJpg) => QString::from(&attachment.blur_preview),
            None => QString::from(""),
        };

        let preview_uri = match attachment.preview_uri {
            LinkType::Url(url) => QString::from(url),
            LinkType::RawJpg => QString::from(&attachment.blur_preview),
        };
        let blur_preview = QString::from(&attachment.blur_preview);
        let height = attachment.height as u32;
        let width = attachment.width as u32;
        KlippyAttachment {
            output_uri,
            hover_uri,
            preview_uri,
            blur_preview,
            height,
            width,
        }
    }
}

impl qobject::SearchResults {
    fn query(self: Pin<&mut Self>, query: QString, refresh: bool) {
        let qt_thread = self.qt_thread();
        let page = if refresh { 0 } else { *self.current_page() };
        println!("Made query: Page {}", page);
        gifboard_core::TOKIO.spawn(async move {
            match gifboard_core::query::fetch_query(&query, page).await {
                Err(e) => {
                    qt_thread
                        .queue(move |self_async| {
                            self_async.query_error(QString::from(e.to_string()));
                        })
                        .unwrap();
                }
                Ok(attachments) => {
                    qt_thread
                        .queue(move |self_async| {
                            self_async.rust_mut().current_page = page + 1;
                        })
                        .unwrap();
                    for attachment in attachments {
                        qt_thread
                            .queue(move |self_async| match attachment {
                                Attachment::Klippy(klippy_source) => {
                                    let attachment =
                                        KlippyAttachment::from_attachment(klippy_source);
                                    self_async.received_klippy(
                                        attachment.output_uri,
                                        attachment.hover_uri,
                                        attachment.preview_uri,
                                        attachment.blur_preview,
                                        attachment.width,
                                        attachment.height,
                                    )
                                }
                                Attachment::LocalFile(path) => {
                                    let path = QString::from(path.to_str().unwrap());
                                    let size = qobject::get_image_size(&path);
                                    if size.is_valid() {
                                        self_async.received_local_image(path, size)
                                    } else {
                                        self_async.received_local_file(path)
                                    }
                                }
                            })
                            .unwrap();
                    }
                }
            }
        });
    }
}
