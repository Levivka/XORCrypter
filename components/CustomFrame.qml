import QtQuick 2.15

Rectangle {
    id: root

    property alias contentItem: contentItem

    radius: 12

    color: "#161b22"
    border.color: "#30363d"
    border.width: 1

    default property alias children: contentItem.children

    Column {
        id: contentItem
        anchors {
            fill: parent
            margins: 16
        }

        spacing: 12
    }
}
