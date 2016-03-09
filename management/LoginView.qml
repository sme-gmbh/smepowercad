import QtQuick 2.4
import QtQuick.Window 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls 1.4

Window {
    id: window
    visible: true

    width: 800
    height: 600
    x: Screen.width / 2 - width / 2
    y: Screen.height / 2 - height / 2

    FontLoader {
        source: "fonts/OpenSans-Regular.ttf"
    }

    RadialGradient {
        id: bg
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#6a6a6a" }
            GradientStop { position: 0.8; color: "#4e4e4e" }
        }
    }

    Text {
        id: title
        text: "SME PowerCAD"
        color: "#e4e4e4"
        anchors.horizontalCenter: parent.horizontalCenter
        y: 85
        font.family: "Open Sans"
        font.pixelSize: 48
    }

    ExclusiveGroup {
        id: facesGroup
        onCurrentChanged: {
            if (current == faceOther) {
                username.visible = true
                username.focus = true
            } else {
                username.visible = false
                password.focus = true
            }
        }
    }

    Row {
        id: faces
        anchors.centerIn: parent
        spacing: 30

        Face {
            id: face1
            exclusiveGroup: facesGroup
            name: "Roland Dierks"
            image: "images/face1.jpg"
        }

        Face {
            id: face2
            exclusiveGroup: facesGroup
            name: "Scott Summers"
            image: "images/face2.jpg"
        }

        Face {
            id: face3
            exclusiveGroup: facesGroup
            name: "Ryan Hause"
            image: "images/face3.jpg"
        }
    }

    Face {
        id: faceOther
        exclusiveGroup: facesGroup
        name: qsTr("Other")
        image: "images/face-nobody.png"
        anchors.verticalCenter: faces.verticalCenter
        anchors.left: faces.right
        anchors.leftMargin: 50
        textSize: 12
        width: 50
        imageSizeMultiplier: 0.6
        maskImage: false
    }

    LineEdit {
        id: username
        visible: false
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: faces.bottom
        anchors.topMargin: 65
        placeholderText: qsTr("Username")
        echoMode: TextInput.Normal
        width: 150
    }

    LineEdit {
        id: password
        anchors.horizontalCenter: username.horizontalCenter
        anchors.top: username.bottom
        anchors.topMargin: 5
        placeholderText: qsTr("Password")
        echoMode: TextInput.Password
        width: 150
    }
    CustomButton {
        id: loginButton
        anchors.verticalCenter: password.verticalCenter
        anchors.left: password.right
        anchors.leftMargin: 10
        image: "images/arrow.png"
        width: 36
        height: 36
        border.width: 1
        radius: width * 0.5
    }

    CustomButton {
        id: serverButton
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: 20
        anchors.bottomMargin: 20
        width: 32
        height: 32
        border.width: 0
        image: "images/server.png"
    }
}
