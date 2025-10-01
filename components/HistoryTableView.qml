import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    Layout.fillWidth: true
    Layout.fillHeight: true

    property var model: null
    property real idColumnRatio: 0.1
    property real fileColumnRatio: 0.5
    property real statusColumnRatio: 0.2
    property real progressColumnRatio: 0.2

    Row {
        id: headerRow
        Layout.fillWidth: true
        Layout.preferredHeight: 32
        spacing: 1

        Rectangle {
            id: idHeader
            width: parent.width * root.idColumnRatio
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
            id: fileHeader
            width: parent.width * root.fileColumnRatio
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
            id: statusHeader
            width: parent.width * root.statusColumnRatio
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
            id: progressHeader
            width: parent.width * root.progressColumnRatio - headerRow.spacing * 3
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
            var availableWidth = table.width - (table.leftMargin
                                                || 0) - (table.rightMargin || 0)
            switch (column) {
            case 0:
                return availableWidth * root.idColumnRatio
            case 1:
                return availableWidth * root.fileColumnRatio
            case 2:
                return availableWidth * root.statusColumnRatio
            case 3:
                return availableWidth * root.progressColumnRatio
            default:
                return availableWidth * 0.25
            }
        }

        rowHeightProvider: function (row) {
            return 30
        }

        delegate: Rectangle {
            implicitHeight: 30
            color: (row % 2 === 0) ? "#1e222a" : "#262a33"

            Text {
                anchors.fill: parent
                anchors.margins: 8
                text: model.display
                color: "#cff7d5"
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
        }

        onWidthChanged: {
            if (table.contentItem) {
                table.forceLayout()
            }
        }
    }
}
