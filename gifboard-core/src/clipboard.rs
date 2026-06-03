use std::io::Write;

use crate::TOKIO;

use cxx_qt_lib::QList;
use cxx_qt_lib::QString;
use cxx_qt_lib::QUrl;
use std::io;
use tokio::sync::oneshot;

async fn make_tmpfiles(urls: QList<QUrl>) -> io::Result<QList<QUrl>> {
    let mut local_links = QList::default();
    let tmp_dir = std::env::temp_dir().join("gifboard");
    if !tmp_dir.exists() {
        std::fs::create_dir(&tmp_dir)?;
    } else if !tmp_dir.is_dir() {
        std::fs::remove_file(&tmp_dir)?;
        std::fs::create_dir(&tmp_dir)?;
    }
    let client = reqwest::Client::new();
    for url in urls.into_iter() {
        if url.is_local_file() {
            local_links.append(url.clone());
        } else {
            let file_path = tmp_dir.join(url.file_name().to_string());
            if !file_path.exists() {
                let mut handle = std::fs::File::create_new(&file_path)?;
                let contents = client
                    .get(url.to_string())
                    .send()
                    .await
                    .map_err(io::Error::other)?
                    .bytes()
                    .await
                    .map_err(io::Error::other)?;
                handle.write_all(contents.as_ref())?;
            }
            local_links.append(QUrl::from_local_file(&QString::from(
                file_path.to_str().expect("Invalid characters in filepath"),
            )));
        }
    }
    Ok(local_links)
}

pub fn save_url_to_temp(urls: QList<QUrl>) -> oneshot::Receiver<io::Result<QList<QUrl>>> {
    let (tx, rx) = oneshot::channel();
    TOKIO.spawn(async move {
        tx.send(make_tmpfiles(urls).await)
            .expect("The receiver was unexpectedly dropped");
    });
    rx
}
