import QtQuick
import QtQuick.Controls

Rectangle {
    id: root

    property alias text: label.text

    property color primaryColor: "#147c24"
    property color hoverColor: "#10631d"
    property color accentColor: "#439418"
    property color textColor: "white"

    property int borderRadius: 8

    signal clicked

    width: 160
    height: 42
    radius: root.borderRadius
    color: primaryColor

    Text {
        id: label
        anchors.centerIn: parent
        color: root.textColor
        font.pixelSize: 16
        font.bold: true
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        onClicked: root.clicked()

        onPressed: root.color = root.accentColor
        onReleased: root.color = containsMouse ? root.hoverColor : root.primaryColor

        onEntered: root.color = root.hoverColor
        onExited: root.color = root.primaryColor
    }

    Behavior on color {
        ColorAnimation {
            duration: 150
        }
    }
}
