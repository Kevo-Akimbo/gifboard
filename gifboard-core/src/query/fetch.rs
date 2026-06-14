use cxx_qt_lib::QString;
use std::{io, path::PathBuf, sync::Arc};
use tokio::sync::mpsc;

use crate::{
    TOKIO, config,
    query::{
        klippy::{KlippySource, fetch_from_klippy},
        local::fetch_local,
    },
};

// RawJpg refers to `blur_preview` in the `Attachment` struct
#[derive(Debug)]
pub enum LinkType {
    Url(String),
    RawJpg,
}

#[derive(Debug)]
pub enum Attachment {
    Klippy(KlippySource),
    LocalFile(PathBuf),
}

pub struct FetchChannels {
    pub query_tx: mpsc::Sender<(QString, usize)>,
    pub attachment_rx: mpsc::Receiver<io::Result<Attachment>>,
    pub next_page_rx: mpsc::Receiver<()>,
}

pub fn fetch_channel() -> FetchChannels {
    let (query_tx, mut query_rx) = mpsc::channel(1);
    let (attachment_tx, attachment_rx) = mpsc::channel(10);
    let (next_page_tx, next_page_rx) = mpsc::channel(1);

    TOKIO.spawn(async move {
        loop {
            if let Some((query, page)) = query_rx.recv().await {
                next_page_tx.send(()).await.unwrap();
                let query: Arc<QString> = Arc::new(query);
                let attachment_tx_local = attachment_tx.clone();
                let query_local = query.clone();
                TOKIO.spawn(async move {
                    if let Ok(local_files) = fetch_local(&query_local, 10, page) {
                        for file in local_files {
                            attachment_tx_local
                                .send(Ok(Attachment::LocalFile(file)))
                                .await
                                .unwrap();
                        }
                    }
                });
                let attachment_tx_klippy = attachment_tx.clone();
                let query_klippy = query.clone();
                TOKIO.spawn(async move {
                    let config = match config::read_config() {
                        Ok(config) => config,
                        Err(e) => {
                            attachment_tx_klippy.send(Err(e)).await.unwrap();
                            return;
                        }
                    };
                    if let Ok(klippy_responses) =
                        fetch_from_klippy(&config, page, &query_klippy.to_string()).await
                    {
                        for response in klippy_responses {
                            attachment_tx_klippy
                                .send(Ok(Attachment::Klippy(response)))
                                .await
                                .unwrap();
                        }
                    }
                });
            }
        }
    });

    FetchChannels {
        query_tx,
        attachment_rx,
        next_page_rx,
    }
}
