import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import GBQml 1.0
import gbemu 1.0

ApplicationWindow {
    width: 600
    height: 800
    visible: true
    id: app
    property bool isSetup

    Timer {
        interval: 16;
        running: true;
        repeat: true
        onTriggered: {
            if (app.isSetup) {
                QGBEmulator.renderNextFrame();
            }
        }
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action {text: qsTr("&Open...")
            }
            MenuSeparator { }
            Action { text: qsTr("&Quit") }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        ImageItem {
            id: gbRender
            width: 160 * 3
            height: 144 * 3
            image: QGBEmulator.renderedImage
        }

        Button {
            text: "Load cartridge"
            width: 150
            height: 50
            onClicked: {
                var cartridgeLoaded = QGBEmulator.loadCartridge('/Users/tutur/Downloads/loz.gb');
                console.log("cartridge loaded = " + cartridgeLoaded);
                app.isSetup = true;
            }
        }
        Text {
            text: "Frame id: " + QGBEmulator.frameId
        }
    }

    Connections {
        target: QGBEmulator
        function onRenderedImageChanged() {
            gbRender.image = QGBEmulator.renderedImage;
        }
    }
}
