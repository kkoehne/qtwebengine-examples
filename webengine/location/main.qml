import QtQuick 2.0
import QtQuick.Window 2.0
import QtWebEngine 1.1

Window {
    visible: true
    width: 1024
    height: 750

    FeaturePermissionBar {
        id: permBar
        view: webEngineView
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }
        z: 3
    }

    WebEngineView {
        id: webEngineView
        anchors.fill: parent
        url: "qrc:/index.html"

        onFeaturePermissionRequested: {
            console.log("feature permission requested");
            permBar.securityOrigin = securityOrigin;
            permBar.requestedFeature = feature;
            permBar.visible = true;
        }

        onJavaScriptConsoleMessage: {
            console.log(sourceID + ":" + lineNumber + " "  + message)
        }
    }
}
