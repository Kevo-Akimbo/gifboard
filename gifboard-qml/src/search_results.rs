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
        #[qproperty(u32, current_page)]
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
    impl cxx_qt::Constructor<()> for SearchResults {}
}

use cxx_qt::{CxxQtType, Threading};
use cxx_qt_lib::QString;
use gifboard_core::TOKIO;
use gifboard_core::query::klippy::KlippySource;
use gifboard_core::query::{Attachment, FetchChannels, LinkType};
use std::pin::Pin;
use tokio::sync::mpsc;

#[derive(Default)]
pub struct KlippyAttachment {
    output_uri: QString,
    hover_uri: QString,
    preview_uri: QString,
    blur_preview: QString,
    width: u32,
    height: u32,
}

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

pub struct SearchResultsRust {
    pub current_page: u32,
    query_transmitter: mpsc::Sender<(QString, usize)>,
}

impl cxx_qt::Constructor<()> for qobject::SearchResults {
    type NewArguments = ();

    type BaseArguments = ();

    type InitializeArguments = ();

    fn route_arguments(
        _: (),
    ) -> (
        Self::NewArguments,
        Self::BaseArguments,
        Self::InitializeArguments,
    ) {
        ((), (), ())
    }

    fn new(_: Self::NewArguments) -> <Self as CxxQtType>::Rust {
        // Junk TX to appease the type checker
        let (junk_tx, _) = mpsc::channel(1);
        SearchResultsRust {
            current_page: 0,
            query_transmitter: junk_tx,
        }
    }

    fn initialize(self: Pin<&mut Self>, _: Self::InitializeArguments) {
        let qt_thread = self.qt_thread();
        let FetchChannels {
            query_tx,
            mut attachment_rx,
            mut next_page_rx,
        } = gifboard_core::query::fetch_channel();
        self.rust_mut().query_transmitter = query_tx;
        let qt_thread_page = qt_thread.clone();
        TOKIO.spawn(async move {
            loop {
                next_page_rx.recv().await.unwrap();
                qt_thread_page
                    .queue(|mut self_async| {
                        self_async.as_mut().rust_mut().current_page += 1;
                        self_async.current_page_changed();
                    })
                    .unwrap();
            }
        });
        TOKIO.spawn(async move {
            loop {
                match attachment_rx.recv().await {
                    Some(Ok(Attachment::Klippy(klippy_source))) => qt_thread
                        .queue(move |self_async| {
                            let received_klippy = KlippyAttachment::from_attachment(klippy_source);
                            self_async.received_klippy(
                                received_klippy.output_uri,
                                received_klippy.hover_uri,
                                received_klippy.preview_uri,
                                received_klippy.blur_preview,
                                received_klippy.width,
                                received_klippy.height,
                            );
                        })
                        .unwrap(),
                    Some(Ok(Attachment::LocalFile(local_path))) => qt_thread
                        .queue(move |self_async| {
                            let path = QString::from(local_path.to_str().unwrap());
                            let size = qobject::get_image_size(&path);
                            if size.is_valid() {
                                self_async.received_local_image(path, size)
                            } else {
                                self_async.received_local_file(path)
                            }
                        })
                        .unwrap(),
                    Some(Err(e)) => qt_thread
                        .queue(move |self_async| {
                            self_async.query_error(QString::from(e.to_string()));
                        })
                        .unwrap(),
                    None => unreachable!(),
                }
            }
        });
    }
}

impl qobject::SearchResults {
    fn query(self: Pin<&mut Self>, query: QString, refresh: bool) {
        let page = if refresh { 0 } else { *self.current_page() };
        println!("Made query: Page {}", page);
        let qt_thread = self.qt_thread();
        let transmitter = self.query_transmitter.clone();
        TOKIO.spawn(async move {
            if let Err(e) = transmitter.send((query, page as usize)).await {
                qt_thread
                    .queue(move |self_async| {
                        self_async.query_error(QString::from(format!("{e:?}")))
                    })
                    .unwrap();
            }
        });
    }
}
