import QtQuick 2.15
import QtQuick.Layouts 1.15

import XorQtQuick

Rectangle {
    id: root

    width: 250
    height: 40
    radius: 8

    color: "#21262d"

    property int value: 1
    property int from: 1
    property int to: 100
    property int stepSize: 1

    signal customValueChanged(int newValue)

    RowLayout {
        anchors.fill: parent
        spacing: 4
        anchors.margins: 4

        CustomButton {
            text: "-"

            implicitWidth: 30
            implicitHeight: parent.height - 8

            primaryColor: "#147c24"
            hoverColor: "#10631d"
            accentColor: "#439418"

            onClicked: {
                root.value = Math.max(root.from, root.value - root.stepSize)
                root.customValueChanged(root.value)
            }
        }

        Rectangle {
            implicitWidth: 50
            implicitHeight: parent.height - 8
            radius: 6

            color: "#147c24"
            border.color: "#0f4f1b"
            border.width: 1

            Text {
                anchors.centerIn: parent
                text: root.value
                font.pixelSize: 16
                color: "white"
            }
        }

        CustomButton {
            text: "+"

            implicitWidth: 30
            implicitHeight: parent.height - 8

            primaryColor: "#147c24"
            hoverColor: "#10631d"
            accentColor: "#439418"
            onClicked: {
                root.value = Math.min(root.to, root.value + root.stepSize)
                root.customValueChanged(root.value)
            }
        }

        Rectangle {
            id: sliderTrack

            Layout.fillWidth: true
            implicitHeight: 8
            radius: implicitHeight

            color: "grey"

            border.color: "#0f4f1b"
            border.width: 1

            Rectangle {
                id: sliderHandle

                height: sliderTrack.height + 6
                width: height
                radius: height

                color: "#439418"

                anchors.verticalCenter: parent.verticalCenter
                x: (root.value - root.from) / (root.to - root.from) * (sliderTrack.width - width)

                MouseArea {
                    anchors.fill: parent

                    drag.target: parent
                    drag.axis: Drag.XAxis
                    drag.minimumX: 0
                    drag.maximumX: sliderTrack.width - parent.width

                    onPositionChanged: {
                        let ratio = parent.x / (sliderTrack.width - parent.width)
                        root.value = Math.round(
                                    root.from + ratio * (root.to - root.from))
                        root.customValueChanged(root.value)
                    }
                }

                Behavior on x {
                    NumberAnimation {
                        duration: 100
                    }
                }
            }
        }
    }

    onValueChanged: {
        sliderHandle.x = (root.value - root.from) / (root.to - root.from)
                * (sliderTrack.width - sliderHandle.width)
    }
}
