#[cxx_qt::bridge]
pub mod ffi {

    unsafe extern "C++" {
        include!("cxx-qt-lib/qstring.h");
        type QString = cxx_qt_lib::QString;
    }

    extern "RustQt" {
        #[qobject]
        #[qml_element]
        type Config = super::ConfigRust;

        #[qinvokable]
        #[cxx_name = "checkLatestVersion"]
        fn check_latest_version(&self);

        #[qinvokable]
        #[cxx_name = "getLocalVersion"]
        fn get_local_version(&self) -> QString;

        #[qinvokable]
        #[cxx_name = "ignoreVersion"]
        fn ignore_version(self: Pin<&mut Self>, version: QString);

        #[qsignal]
        #[cxx_name = "outdatedVersion"]
        fn outdated_version(self: Pin<&mut Self>, latest_version: QString);
    }

    impl cxx_qt::Threading for Config {}
}

use std::{collections::HashSet, pin::Pin};

use cxx_qt::CxxQtType;
use cxx_qt::Threading;
use cxx_qt_lib::QString;
use gifboard_core::TOKIO;

pub struct ConfigRust {
    version: semver::Version,
    ignored_versions: HashSet<String>,
    client: reqwest::Client,
}

impl Default for ConfigRust {
    fn default() -> Self {
        let version = semver::Version::parse(env!("CARGO_PKG_VERSION"))
            .expect("Invalid cargo package version");
        let client = reqwest::Client::builder()
            .user_agent(concat!(
                env!("CARGO_PKG_NAME"),
                "/",
                env!("CARGO_PKG_VERSION")
            ))
            .build()
            .expect("Failed to build http client");
        let ignored_versions =
            gifboard_core::cache::get_ignored_versions().expect("Failed to open cache");
        ConfigRust {
            version,
            client,
            ignored_versions,
        }
    }
}

impl ffi::Config {
    fn get_local_version(&self) -> QString {
        QString::from(&self.version.to_string())
    }

    fn ignore_version(self: Pin<&mut Self>, version: QString) {
        let ignored_versions = &mut self.rust_mut().ignored_versions;
        ignored_versions.insert(version.to_string());
        gifboard_core::cache::write_ignored_versions(ignored_versions)
            .expect("Failed to write to cache");
    }

    fn check_latest_version(&self) {
        let qt_thread = self.qt_thread();
        let local_version = self.version.clone();
        let request = self
            .client
            .get("https://api.github.com/repos/Kaisia-Estrel/gifboard/releases/latest")
            .send();
        TOKIO.spawn(async move {
            let res = if let Ok(res) = request.await {
                res.text().await
            } else {
                return;
            };

            let body: serde_json::Value = if let Ok(res) = res {
                serde_json::from_str(&res).expect("Malformed JSON response from github")
            } else {
                return;
            };

            let version = body
                .get("tag_name")
                .and_then(|x| x.as_str())
                .and_then(|x| x.strip_prefix("v"))
                .expect("Malformed JSON response from github");

            let latest_version =
                semver::Version::parse(version).expect("Malformed version from github");
            if latest_version > local_version {
                eprintln!("A new version is available '{}'", latest_version);
                qt_thread
                    .queue(move |self_async| {
                        if !self_async
                            .ignored_versions
                            .contains(&latest_version.to_string())
                        {
                            self_async.outdated_version(QString::from(&latest_version.to_string()));
                        }
                    })
                    .unwrap();
            }
        });
    }
}
