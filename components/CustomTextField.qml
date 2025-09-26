import QtQuick
import QtQuick.Controls

Rectangle {
    id: root

    property alias text: input.text

    property string placeholderText: "Введите текст"
    property RegularExpressionValidator validator

    property color borderColor: "#3E3E42"
    property color focusColor: "#147c24"

    property real maximumLength: 100

    width: 250
    height: 42
    radius: 6

    border.width: 1
    border.color: input.focus ? focusColor : borderColor
    color: "transparent"

    TextInput {
        id: input

        anchors {
            fill: parent
            leftMargin: 16
            rightMargin: 16
        }

        color: "#cff7d5"
        font.pixelSize: 16
        clip: true
        focus: false

        verticalAlignment: TextInput.AlignVCenter

        maximumLength: root.maximumLength
        validator: root.validator
    }

    Text {
        anchors {
            fill: parent
            leftMargin: 16
            rightMargin: 16
        }

        text: root.placeholderText
        visible: input.text.length === 0 && !input.focus
        color: "#6c8774"
        font.pixelSize: 14

        verticalAlignment: Text.AlignVCenter
    }

    Behavior on border.color {
        ColorAnimation {
            duration: 200
        }
    }
}
