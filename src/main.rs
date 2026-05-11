use cxx_qt::casting::Upcast;
use cxx_qt_lib::QGuiApplication;
use cxx_qt_lib::QQmlApplicationEngine;
use cxx_qt_lib::QQmlEngine;
use cxx_qt_lib::QUrl;
use std::pin::Pin;

pub mod myobject;

// use smithay_client_toolkit::{
//     compositor::CompositorState, delegate_compositor, globals::GlobalData,
//     reexports::protocols_wlr::layer_shell::v1::client::zwlr_layer_shell_v1::ZwlrLayerShellV1,
//     shell::wlr_layer::LayerShell,
// };
// use wayland_client::{
//     Connection, Dispatch, QueueHandle,
//     globals::{GlobalListContents, registry_queue_init},
//     protocol::{wl_compositor::WlCompositor, wl_registry::WlRegistry},
// };
//
// struct State {}
//
// impl Dispatch<WlRegistry, GlobalListContents> for State {
//     fn event(
//         _state: &mut Self,
//         _proxy: &WlRegistry,
//         _event: <WlRegistry as wayland_client::Proxy>::Event,
//         _data: &GlobalListContents,
//         _conn: &Connection,
//         _qhandle: &QueueHandle<Self>,
//     ) {
//         dbg!(_event);
//         todo!()
//     }
// }
//
// impl Dispatch<WlCompositor, GlobalData> for State {
//     fn event(
//         _state: &mut Self,
//         _proxy: &WlCompositor,
//         _event: <WlCompositor as wayland_client::Proxy>::Event,
//         _data: &GlobalData,
//         _conn: &Connection,
//         _qhandle: &QueueHandle<Self>,
//     ) {
//         todo!()
//     }
// }
//
// impl Dispatch<ZwlrLayerShellV1, GlobalData> for State {
//     fn event(
//         _state: &mut Self,
//         _proxy: &ZwlrLayerShellV1,
//         _event: <ZwlrLayerShellV1 as wayland_client::Proxy>::Event,
//         _data: &GlobalData,
//         _conn: &Connection,
//         _qhandle: &QueueHandle<Self>,
//     ) {
//         todo!()
//     }
// }

fn main() {
    // let conn = Connection::connect_to_env().unwrap();
    // let (globals, mut event_queue) = registry_queue_init(&conn).unwrap();
    // let qh: QueueHandle<State> = event_queue.handle();
    //
    // let compositor = CompositorState::bind(&globals, &qh).expect("wl_compositor is not available");
    // // This app uses the wlr layer shell, which may not be available with every compositor.
    // let layer_shell = LayerShell::bind(&globals, &qh).expect("layer shell is not available");
    //
    // let display = conn.display();
    // let mut event_queue = conn.new_event_queue();
    // let qh = event_queue.handle();

    // WaylandSource::new(conn.clone(), event_queue)
    //     .insert_idle(move |conn| {
    //         let _ = conn;
    //     })
    //     .expect("init");
    //
    // loop {
    //     event_queue.blocking_dispatch(&mut app).unwrap();
    // }

    println!("Start");

    let mut app = QGuiApplication::new();
    let mut engine = QQmlApplicationEngine::new();

    // Load the QML path into the engine
    if let Some(engine) = engine.as_mut() {
        engine.load(&QUrl::from("qrc:/qt/qml/com/estrel/gifboard/qml/main.qml"));
    }

    if let Some(engine) = engine.as_mut() {
        let engine: Pin<&mut QQmlEngine> = engine.upcast_pin();
        // Listen to a signal from the QML Engine
        engine
            .on_quit(|_| {
                println!("QML Quit!");
            })
            .release();
    }

    // Start the app
    if let Some(app) = app.as_mut() {
        app.exec();
    }
}
