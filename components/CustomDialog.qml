import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: root
    modal: true
    property string dialogTitle: ""
    property string dialogText: ""
    property bool isError: false

    standardButtons: Dialog.NoButton
    focus: true

    background: Rectangle {
        color: "#161b22"
        radius: 12
        border.color: root.isError ? "#f85149" : "#30363d"
        border.width: 2
    }

    contentItem: ColumnLayout {
        id: content
        width: 520
        anchors.margins: 16
        spacing: 12

        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            Rectangle {
                width: 36
                height: 36
                radius: 8
                color: root.isError ? "#5a1e1e" : "#1a472a"
                Text {
                    anchors.centerIn: parent
                    text: root.isError ? "⚠" : "✓"
                    font.pixelSize: 16
                    color: root.isError ? "#f85149" : "#56d364"
                }
            }

            ColumnLayout {
                spacing: 2
                Text {
                    id: hdr
                    text: root.dialogTitle
                    font.pixelSize: 16
                    font.bold: true
                    color: "#f0f6fc"
                    elide: Text.ElideRight
                }
                Text {
                    id: subtitle
                    text: root.isError ? "Ошибка" : "Информация"
                    font.pixelSize: 12
                    color: "#8b949e"
                }
            }
        }

        Text {
            id: body
            text: root.dialogText
            wrapMode: Text.WordWrap
            font.pixelSize: 14
            color: "#cbd6c3"
            Layout.fillWidth: true
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignRight
            spacing: 12

            CustomButton {
                text: "OK"
                onClicked: root.close()
            }
        }
    }
}
