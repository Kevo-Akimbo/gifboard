#[cxx_qt::bridge]
pub mod qobject {

    unsafe extern "C++" {
        include!("cxx-qt-lib/qstring.h");
        type QString = cxx_qt_lib::QString;
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
        #[cxx_name = "receivedResult"]
        fn received_result(
            self: Pin<&mut Self>,
            output_uri: QString,
            hover_uri: QString,
            preview_uri: QString,
            width: u32,
            height: u32,
        );
    }
    impl cxx_qt::Threading for SearchResults {}
}

#[derive(Default)]
pub struct SearchResultsRust {
    pub current_page: usize,
}

use cxx_qt::{CxxQtType, Threading};
use cxx_qt_lib::QString;
use std::pin::Pin;

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
                            .queue(move |self_async| {
                                let (output_uri, hover_uri, preview_uri, width, height) =
                                    attachment.to_received_result_item();

                                self_async.received_result(
                                    output_uri,
                                    hover_uri,
                                    preview_uri,
                                    width,
                                    height,
                                );
                            })
                            .unwrap();
                    }
                }
            }
        });
    }
}
