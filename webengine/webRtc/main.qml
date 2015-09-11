import QtQuick 2.3
import QtQuick.Window 2.2
import QtWebEngine 1.1

Window {
    visible: true
    width: 1024
    height: 860

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
        url: "https://apprtc.appspot.com/"

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
