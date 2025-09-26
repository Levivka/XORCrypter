import QtQuick

Text {
    id: root

    property color textColor: "white"
    property int fontSize: 18
    property bool bold: false

    color: textColor

    font.pixelSize: fontSize
    font.bold: bold
}
