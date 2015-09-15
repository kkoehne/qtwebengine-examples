import QtQuick 2.3
import QtQuick.Window 2.2
import QtWebEngine 1.0

Window {
    visible: true
    width: 1024
    height: 860

    WebEngineView {
        anchors.fill: parent
        url: "http://www.qtworldsummit.com"
    }
}
