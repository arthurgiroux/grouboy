import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import GBQml 1.0
import gbemu 1.0

ApplicationWindow {
    minimumWidth: 500
    minimumHeight: 300
    visible: true
    id: app
    property bool cartridgeLoaded
    signal cartridgeSelected(filepath: string)

    onCartridgeSelected: function (filepath) {
        var cartridgeLoaded = QGBEmulator.loadCartridge(filepath);
        console.log("cartridge loaded = " + cartridgeLoaded);
        app.cartridgeLoaded = cartridgeLoaded;
    }

    Timer {
        interval: 16
        running: true
        repeat: true
        onTriggered: {
            if (app.cartridgeLoaded) {
                QGBEmulator.renderNextFrame();
            }
        }
    }

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
