import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Canvas {
    id: dropFileArea
    signal fileDropped(filepath: string)
    property bool isFileBeingDropped
    property string text
    Layout.fillHeight: true
    Layout.fillWidth: true
    Layout.margins: 20

   SystemPalette {
        id: colorPalette
        colorGroup: SystemPalette.Active
    }

    DropArea {
        anchors.fill: parent

        onEntered: function(drag) {
            dropFileArea.isFileBeingDropped = true
            drag.accept(Qt.LinkAction);
            dropFileArea.requestPaint()
        }

        onDropped: function(drop) {
            dropFileArea.isFileBeingDropped = false
            dropFileArea.requestPaint()
            fileDropped(drop.urls)
        }

        onExited: {
            dropFileArea.isFileBeingDropped = false
            dropFileArea.requestPaint()
        }
    }

    Text {
        anchors.centerIn: parent
        text: dropFileArea.text
        color: colorPalette.text
        font.pointSize: 30
    }

    onPaint: {
        var ctx = getContext("2d");

        // Fill the background
        ctx.fillStyle = dropFileArea.isFileBeingDropped ? colorPalette.highlight : colorPalette.light;
        ctx.fillRect(0, 0, width, height);

        // Define dash line style
        ctx.setLineDash([5, 5]);
        ctx.lineWidth = 2;
        ctx.strokeStyle = colorPalette.dark;

        // Create 4 lines for a dash rectangle
        ctx.beginPath();
        ctx.moveTo(0, 0);
        ctx.lineTo(width, 0);
        ctx.stroke();

        ctx.beginPath();
        ctx.moveTo(0, 0);
        ctx.lineTo(0, height);
        ctx.stroke();

        ctx.beginPath();
        ctx.moveTo(width, 0);
        ctx.lineTo(width, height);
        ctx.stroke();

        ctx.beginPath();
        ctx.moveTo(0, height);
        ctx.lineTo(width, height);
        ctx.stroke();
     }
}