import QtQuick 2.1
import QtQuick.Controls 1.0
import QtWebEngine 1.1
import QtQuick.Layouts 1.0

Rectangle {
    id: permissionBar

    property var requestedFeature;
    property url securityOrigin;
    property WebEngineView view;

    gradient: Gradient {
        GradientStop { position: 0.0; color: "gray" }
        GradientStop { position: 1.0; color: "darkgray" }
    }

    visible: false
    height: acceptButton.height + 4

    onRequestedFeatureChanged: {
        message.text = securityOrigin + " wants to access " + message.textForFeature(requestedFeature);
    }

    RowLayout {
        anchors {
            fill: permissionBar
            leftMargin: 5
            rightMargin: 5
        }
        Label {
            id: message
            Layout.fillWidth: true

            function textForFeature(feature) {
                if (feature === WebEngineView.MediaAudioCapture)
                    return "your microphone"
                if (feature === WebEngineView.MediaVideoCapture)
                    return "your camera"
                if (feature === WebEngineView.MediaAudioVideoCapture)
                    return "your camera and microphone"
                if (feature === WebEngineView.Geolocation)
                    return "your position"
            }
        }

        Button {
            id: acceptButton
            text: "Accept"
            Layout.alignment: Qt.AlignRight
            onClicked: {
                console.log("grant feature permission");
                view.grantFeaturePermission(securityOrigin, requestedFeature, true);
                permissionBar.visible = false;
            }
        }

        Button {
            text: "Deny"
            Layout.alignment: Qt.AlignRight
            onClicked: {
                console.log("deny feature permission");
                view.grantFeaturePermission(securityOrigin, requestedFeature, false);
                permissionBar.visible = false
            }
        }
    }
}
