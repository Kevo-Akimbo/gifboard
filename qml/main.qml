import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12

import org.kde.layershell 1.0 as LayerShell

import com.estrel.gifboard 1.0

ApplicationWindow {
    id: root
    title: qsTr("Hello World")
    visible: true
    visibility: Window.Windowed
    color: palette.window

    height: 280
    width: 240
    LayerShell.Window.layer: LayerShell.Window.LayerOverlay
    LayerShell.Window.anchors: LayerShell.Window.AnchorNone
    // LayerShell.Window.keyboardInteractivity
    LayerShell.Window.exclusionZone: 0
    flags: Qt.Popup

    Component.onCompleted: {}

    readonly property MyObject myObject: MyObject {
        number: 1
        string: qsTr("My String with my number: %1").arg(number)
    }

    Column {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Label {
            text: qsTr("Number: %1").arg(root.myObject.number)
            color: palette.text
        }

        Label {
            text: qsTr("String: %1").arg(root.myObject.string)
            color: palette.text
        }

        Button {
            text: qsTr("Increment Number")

            onClicked: root.myObject.incrementNumber()
        }

        Button {
            text: qsTr("Say Hi!")

            onClicked: root.myObject.sayHi(root.myObject.string, root.myObject.number)
        }

        Button {
            text: qsTr("Quit")

            onClicked: Qt.quit()
        }
    }
}
