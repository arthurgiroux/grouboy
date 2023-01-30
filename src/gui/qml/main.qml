import QtQuick
import QtQuick.Controls

ApplicationWindow {
    width: 400
    height: 400
    visible: true

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action { text: qsTr("&Open...") }
            MenuSeparator { }
            Action { text: qsTr("&Quit") }
        }
    }
}
