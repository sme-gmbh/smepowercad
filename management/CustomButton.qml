import QtQuick 2.0

Rectangle {
    property alias image: image.source
    signal clicked

    color: "transparent"
    border.color: mouseArea.containsMouse ? "#3498db" : "#9b9b9b"

    Image {
        id: image
        anchors.centerIn: parent
        fillMode: Image.PreserveAspectCrop
        clip: true
        smooth: true
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: parent.clicked()
    }
}
