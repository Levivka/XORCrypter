import QtQuick

Item {
    id: root
    property alias text: label.text
    property bool checked: false
    signal toggled(bool state)

    width: 200
    height: 28

    Row {
        spacing: 8
        anchors.verticalCenter: parent.verticalCenter

        Rectangle {
            id: box
            width: 20
            height: 20
            radius: 4
            border.width: 1
            border.color: root.checked ? "#147c24" : "#6c8774"
            color: root.checked ? "#147c24" : "transparent"

            Behavior on color {
                ColorAnimation {
                    duration: 150
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    root.checked = !root.checked
                    root.toggled(root.checked)
                }
            }

            Text {
                anchors.centerIn: parent
                text: root.checked ? "✓" : ""
                color: "white"
                font.pixelSize: 14
                font.bold: true
            }
        }

        Text {
            id: label
            color: "#cff7d5"
            font.pixelSize: 15
            verticalAlignment: Text.AlignVCenter
        }
    }
}
