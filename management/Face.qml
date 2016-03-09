import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls 1.4

Item {
    id: root
    width: 100
    height: width

    property bool checked: false
    property ExclusiveGroup exclusiveGroup: null

    onExclusiveGroupChanged: {
        if (exclusiveGroup)
            exclusiveGroup.bindCheckable(root)
    }

    // export properties
    property alias image: image.source
    property alias name: name.text
    property alias textSize: name.font.pixelSize
    property real imageSizeMultiplier: 1.0
    property bool maskImage: true

    Rectangle {
        id: rect
        anchors.fill: parent
        border.color: checked ? "#3498db" : "#919191"
        border.width: 2
        radius: width * 0.5
        color: "transparent"

        Image {
            id: image
            anchors.centerIn: parent
            width: (parent.width * imageSizeMultiplier) - (parent.border.width * 2)
            height: width
            fillMode: Image.PreserveAspectCrop
            clip: true
            smooth: true

            layer.enabled: maskImage
            layer.effect: OpacityMask {
                maskSource: Item {
                    width: image.width
                    height: image.height
                    Rectangle {
                        anchors.centerIn: parent
                        width: image.width
                        height: image.height
                        radius: Math.min(width, height)
                    }
                }
            }
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                checked = !checked
            }
        }

        Text {
            id: name
            anchors.top: rect.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "Open Sans"
            font.pixelSize: 14
            color: "#e4e4e4"
        }
    }
}
