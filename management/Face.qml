/**********************************************************************
** smepowercad
** Copyright (C) 2015 Smart Micro Engineering GmbH
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
** You should have received a copy of the GNU General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**********************************************************************/

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
