pragma ComponentBehavior: Bound
import QtQuick 2.12

Item {
    id: previewImageRoot

    signal urlCopy
    signal tmpfileCopy
    signal localCopy
    signal select
    signal unselect

    required property Flickable gifFlickable
    required property url imagePreviewUri
    required property string imageHoverUri
    required property url imageOutputUri
    required property string blurPreview
    required property int imageHeight
    required property int imageWidth
    required property int index

    height: imageHeight / (imageWidth / width)

    property bool hovered: false
    property bool selected: false
    property bool hasHoverImage: imageHoverUri !== ""

    Rectangle {
        id: previewImageContainer
        anchors.fill: parent
        color: "#505F71"
        border.width: 2
        border.color: previewImageRoot.selected ? "red" : "blue"

        Image {
            id: blurPreviewImage
            anchors.fill: parent
            anchors.margins: previewImageContainer.border.width
            fillMode: Image.PreserveAspectFit
            visible: previewImage.status != Image.Ready
            source: previewImageRoot.blurPreview
        }

        AnimatedImage {
            id: previewImage
            anchors.fill: parent
            anchors.margins: previewImageContainer.border.width
            fillMode: Image.PreserveAspectFit

            property bool loadedPreview: false
            asynchronous: true

            source: previewImageRoot.imagePreviewUri
            playing: true
        }

        Loader {
            id: hoverLoader
            anchors.fill: parent
            anchors.margins: previewImageContainer.border.width
            active: previewImageRoot.hasHoverImage
            sourceComponent: AnimatedImage {
                id: hoverImage

                anchors.fill: parent
                fillMode: Image.PreserveAspectFit

                playing: previewImageRoot.hovered || previewImageRoot.selected

                property bool previouslyHovered: false
                visible: previewImageRoot.hovered || previewImageRoot.selected
                source: previouslyHovered ? previewImageRoot.imageHoverUri : ""

                // Timer so that hoverimages don't all load when flicking through the browser
                property Timer hoverTimer: Timer {
                    interval: 300
                    repeat: false
                    onTriggered: {
                        hoverImage.previouslyHovered = true;
                    }
                }

                Rectangle {
                    id: loadingBar
                    anchors.left: parent.left
                    anchors.margins: 10
                    implicitHeight: 10
                    color: "blue"

                    anchors.top: parent.top

                    state: previewImageRoot.gifFlickable.contentY >= previewImageRoot.y ? "bottom" : "top"

                    states: [
                        State {
                            name: "top"
                            AnchorChanges {
                                target: loadingBar
                                anchors.top: parent.top
                                anchors.bottom: undefined
                            }
                        },
                        State {
                            name: "bottom"
                            AnchorChanges {
                                target: loadingBar
                                anchors.top: undefined
                                anchors.bottom: parent.bottom
                            }
                        }
                    ]
                    visible: hoverImage.status != Image.Ready
                    // Also doesnt make it show if the hoverimage loaded fast enough
                    Behavior on width {
                        NumberAnimation {
                            duration: 50
                        }
                    }
                }

                onProgressChanged: {
                    loadingBar.width = hoverImage.progress * (parent.width - loadingBar.anchors.margins * 2);
                }
            }
        }
        Rectangle {
            anchors.fill: parent
            anchors.margins: previewImageContainer.border.width
            color: "black"
            opacity: (previewImageRoot.selected || previewImageRoot.hovered) ? 0 : 0.2

            Behavior on opacity {
                NumberAnimation {
                    duration: 150
                }
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true
        propagateComposedEvents: true
        onEntered: {
            previewImageRoot.hovered = true;
            if (previewImageRoot.hasHoverImage) {
                hoverLoader.item.currentFrame = previewImage.currentFrame;
                hoverLoader.item.hoverTimer.start();
            }
        }
        onExited: {
            previewImageRoot.hovered = false;
            if (previewImageRoot.hasHoverImage) {
                hoverLoader.item.hoverTimer.stop();
            }
        }

        function toggleSelect() {
            if (previewImageRoot.selected) {
                previewImageRoot.selected = false;
                previewImageRoot.unselect();
            } else {
                previewImageRoot.selected = true;
                previewImageRoot.select();
            }
        }

        onClicked: mouse => {
            if (mouse.button === Qt.LeftButton) {
                const ctrlShift = Qt.ShiftModifier | Qt.ControlModifier;
                if ((mouse.modifiers & ctrlShift) == ctrlShift) {
                    previewImageRoot.selected = true;
                    previewImageRoot.select();
                    previewImageRoot.localCopy();
                } else if (mouse.modifiers & Qt.ShiftModifier) {
                    toggleSelect();
                } else if (mouse.modifiers & Qt.ControlModifier) {
                    previewImageRoot.selected = true;
                    previewImageRoot.select();
                    previewImageRoot.tmpfileCopy();
                } else {
                    previewImageRoot.selected = true;
                    previewImageRoot.select();
                    previewImageRoot.urlCopy();
                }
            } else if (mouse.button === Qt.RightButton) {
                toggleSelect();
            }
            return true;
        }
    }
}
