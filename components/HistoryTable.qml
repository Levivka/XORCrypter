import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    Layout.fillWidth: true
    Layout.fillHeight: true

    property var model: null

    Row {
        id: headerRow
        Layout.fillWidth: true
        Layout.preferredHeight: 32
        spacing: 1

        Rectangle {
            width: 100
            height: parent.height
            color: "#2e3340"
            Text {
                text: "ID"
                anchors.centerIn: parent
                color: "#cff7d5"
                font.bold: true
            }
        }
        Rectangle {
            width: 300
            height: parent.height
            color: "#2e3340"
            Text {
                text: "Файл"
                anchors.centerIn: parent
                color: "#cff7d5"
                font.bold: true
            }
        }
        Rectangle {
            width: 200
            height: parent.height
            color: "#2e3340"
            Text {
                text: "Статус"
                anchors.centerIn: parent
                color: "#cff7d5"
                font.bold: true
            }
        }
        Rectangle {
            width: 150
            height: parent.height
            color: "#2e3340"
            Text {
                text: "Прогресс"
                anchors.centerIn: parent
                color: "#cff7d5"
                font.bold: true
            }
        }
    }

    TableView {
        id: table
        Layout.fillWidth: true
        Layout.fillHeight: true
        clip: true

        model: root.model

        columnWidthProvider: function (column) {
            switch (column) {
            case 0:
                return 100
            case 1:
                return 300
            case 2:
                return 200
            case 3:
                return 153
            default:
                return 150
            }
        }

        rowHeightProvider: function (row) {
            return 30
        }

        delegate: Rectangle {
            implicitHeight: 30
            color: (top % 2 === 0) ? "#1e222a" : "#262a33"

            Text {
                anchors.centerIn: parent
                text: model.display
                color: "#cff7d5"
                elide: Text.ElideRight
            }
        }
    }
}
