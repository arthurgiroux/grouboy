import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import GBQml 1.0

ApplicationWindow {
    minimumWidth: 1024
    minimumHeight: 768
    visible: true
    id: app
    property bool cartridgeLoaded
    signal cartridgeSelected(filepath: string)

    function focusRenderAndLoadSave() {
        gbRender.forceActiveFocus();
        QGBEmulator.loadSaveFromFile();
    }

    Component.onCompleted: {
        if (applicationData.romFile) {
            app.cartridgeLoaded = QGBEmulator.loadCartridgeFromLocalFile(applicationData.romFile);
            focusRenderAndLoadSave();
        }
    }

    onCartridgeSelected: function (filepath) {
        app.cartridgeLoaded = QGBEmulator.loadCartridgeFromUrl(filepath);
        focusRenderAndLoadSave();
    }

    ApplicationWindow {
        id: debugTileWindow
        visible: false
        width: 400
        height: 350

        TileDebugger {
            id: tileDebugger
            emulator: QGBEmulator

            RowLayout {
                anchors.fill: parent

                ColumnLayout {
                    RowLayout {
                        RadioButton {
                            text: "Tileset 0"
                            onClicked: {
                                tileDebugger.tileSetId = 0
                            }
                            checked: true
                        }
                        RadioButton {
                            text: "Tileset 1"
                            onClicked: {
                                tileDebugger.tileSetId = 1
                            }
                        }
                    }

                    ImageItem {
                        id: selectedTileImage
                        image: tileDebugger.tileSetImage
                        property int imageHeight: 128
                        property int imageWidth: 128
                        property int scale: 2
                        property int itemSize: 8
                        property int nbrItemsPerRow: 16
                        height: imageHeight * scale
                        width: imageWidth * scale
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            onPositionChanged: {
                                var selectedRow = Math.floor(mouseY / selectedTileImage.scale / selectedTileImage.itemSize);
                                var selectedCol = Math.floor(mouseX / selectedTileImage.scale / selectedTileImage.itemSize);
                                var tileId = (selectedRow * selectedTileImage.nbrItemsPerRow + selectedCol);
                                tileDebugger.selectedTileId = tileId;
                            }
                        }
                    }
                }

                ImageItem {
                    image: tileDebugger.selectedTileImage
                    height: 128
                    width: 128
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                }
            }
       }
   }

/*
    Timer {
        interval: 16
        running: true
        repeat: true
        onTriggered: {
            if (app.cartridgeLoaded) {
                QGBEmulator.renderNextFrame();
            }
        }
    }*/

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action {
                text: qsTr("&Open...")
                onTriggered: {
                    romSelectionDialog.open()
                }
            }
            MenuSeparator { }
            Action {
                text: qsTr("&Quit")
                onTriggered: {
                    Qt.quit()
                }
            }
        }

        Menu {
            title: qsTr("&Debug")
            Action {
                text: qsTr("&Tiles")
                onTriggered: {
                    debugTileWindow.visible = true
                    // We force the focus back to the render
                    // So that keyboard events can still be received
                    gbRender.forceActiveFocus()
                }
            }
        }
    }

   FileDialog {
        id: romSelectionDialog
        title: "Please choose a rom file"
        onAccepted: {
            app.cartridgeSelected(selectedFile)
        }
   }

    ColumnLayout {
        anchors.fill: parent

        ImageItem {
            id: gbRender
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 144
            Layout.minimumWidth: 160
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            image: QGBEmulator.renderedImage
            visible: app.cartridgeLoaded

            focus: true
            Keys.onPressed: (event) => {
                QGBEmulator.onKeyPressed(event.key);
                event.accepted = true;
            }

            Keys.onReleased: (event) => {
                QGBEmulator.onKeyReleased(event.key);
                event.accepted = true;
            }

            Shortcut {
                sequences: [StandardKey.Save]
                onActivated: {
                    QGBEmulator.saveToFile();
                }
            }
        }
    }

    DropFileArea {
        anchors.fill: parent
        anchors.margins: 20
        visible: !app.cartridgeLoaded
        text: "Drag & Drop a file"
        buttonText: "Select from the explorer"

        onFileSelectionButtonClicked: {
            romSelectionDialog.open()
        }

        Component.onCompleted: {
            fileDropped.connect(app.cartridgeSelected)
        }
    }

    Connections {
        target: QGBEmulator
        function onRenderedImageChanged() {
            gbRender.image = QGBEmulator.renderedImage;
        }
    }
}
