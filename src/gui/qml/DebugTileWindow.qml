import QtQuick
import GBQml 1.0
import gbemu 1.0

Window {
    width: 100
    height: 100

    QTileMap {
        id: tilemap
        emulator: QGBEmulator
    }

    ImageItem {
        image: tilemap.image
        height: 50
        width: 50
    }
}
