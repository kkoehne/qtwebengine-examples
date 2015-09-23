import QtQuick 2.0
import QtQuick.Window 2.0
import QtWebEngine 1.0

Window {
    visible: true
    width: 1024
    height: 860

    WebEngineView {
        anchors.fill: parent
        url: "qrc:///form.html"
    }
}
