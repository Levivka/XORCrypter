import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Popup {
    id: root

    property string dialogTitle: ""
    property string dialogText: ""
    property bool isError: false
    property int autoCloseDelay: 3000

    modal: false
    focus: false

    y: 20
    x: parent ? parent.width - width - 20 : 20

    padding: 16
    implicitWidth: 400
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    background: Rectangle {
        color: "#161b22"
        radius: 12
        opacity: 0.95
        layer.enabled: true
    }

    contentItem: ColumnLayout {
        spacing: 8
        RowLayout {
            spacing: 12

            Rectangle {
                implicitWidth: 36
                implicitHeight: 36
                radius: 8
                color: root.isError ? "#5a1e1e" : "#1a472a"
                Text {
                    anchors.centerIn: parent
                    text: root.isError ? "⚠" : "✓"
                    font.pixelSize: 18
                    color: root.isError ? "#f85149" : "#56d364"
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 2
                Text {
                    text: root.dialogTitle
                    font.pixelSize: 16
                    font.bold: true
                    color: "#f0f6fc"
                    elide: Text.ElideRight
                }
                Text {
                    text: root.dialogText
                    wrapMode: Text.WordWrap
                    font.pixelSize: 14
                    color: "#cbd6c3"
                }
            }
        }
    }

    Timer {
        id: autoCloseTimer
        interval: root.autoCloseDelay
        onTriggered: root.close()
    }

    onOpened: {
        autoCloseTimer.restart()
    }

    enter: Transition {
        NumberAnimation {
            property: "opacity"
            from: 0
            to: 1
            duration: 200
        }
        NumberAnimation {
            property: "y"
            from: -20
            to: 20
            duration: 200
            easing.type: Easing.OutCubic
        }
    }

    exit: Transition {
        NumberAnimation {
            property: "opacity"
            from: 1
            to: 0
            duration: 200
        }
        NumberAnimation {
            property: "y"
            from: 20
            to: 0
            duration: 200
            easing.type: Easing.InCubic
        }
    }
}
