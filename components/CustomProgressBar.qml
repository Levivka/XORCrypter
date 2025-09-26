import QtQuick

Rectangle {
    id: root

    property int value: 0
    property int maximumValue: 100

    property color backgroundColor: "#2a2d2f"
    property color progressColor: "#147c24"

    width: 320
    height: 20
    radius: 6

    color: backgroundColor

    Rectangle {
        id: progress

        width: (root.value / root.maximumValue) * root.width
        height: parent.height
        radius: parent.radius

        color: root.progressColor

        Behavior on width {
            NumberAnimation {
                duration: 200
                easing.type: Easing.InOutQuad
            }
        }
    }
}
