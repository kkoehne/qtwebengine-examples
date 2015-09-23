import QtQuick 2.0
import QtQuick.Window 2.0
import QtWebEngine 1.0

Window {
    visible: true
    width: 1024
    height: 750

    WebEngineView {
        anchors.fill: parent
        url: "index.html"

        onJavaScriptConsoleMessage: {
	    // forwards to qDebug
            console.log(sourceID + ":" + lineNumber + " "  + message)
        }
    }
}
