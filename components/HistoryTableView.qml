import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    Layout.fillWidth: true
    Layout.fillHeight: true

    property var model: null
    property real idColumnRatio: 0.08
    property real fileColumnRatio: 0.20
    property real statusColumnRatio: 0.15
    property real progressColumnRatio: 0.12
    property real timeColumnRatio: 0.25
    property real keyColumnRatio: 0.20

    RowLayout {
        Layout.fillWidth: true
        Layout.preferredHeight: 40

        AppLabel {
            text: "История обработанных файлов"
            font.pixelSize: 16
            font.bold: true
            Layout.fillWidth: true
        }

        AppButton {
            text: "Очистить историю"
            primaryColor: "#6c1c1c"
            hoverColor: "#922424"
            accentColor: "#5a0f0f"
            Layout.preferredWidth: 150
            onClicked: {
                root.model.clearAll()
            }
        }
    }

    Row {
        id: headerRow
        Layout.fillWidth: true
        Layout.preferredHeight: 32
        spacing: 1

        Rectangle {
            width: parent.width * root.idColumnRatio
            height: parent.height
            color: "#2e3340"
            Text {
                text: "ID"
                anchors.centerIn: parent
                color: "#cff7d5"
                font.bold: true
                font.pixelSize: 12
            }
        }
        Rectangle {
            width: parent.width * root.fileColumnRatio
            height: parent.height
            color: "#2e3340"
            Text {
                text: "Файл"
                anchors.centerIn: parent
                color: "#cff7d5"
                font.bold: true
                font.pixelSize: 12
            }
        }
        Rectangle {
            width: parent.width * root.statusColumnRatio
            height: parent.height
            color: "#2e3340"
            Text {
                text: "Статус"
                anchors.centerIn: parent
                color: "#cff7d5"
                font.bold: true
                font.pixelSize: 12
            }
        }
        Rectangle {
            width: parent.width * root.progressColumnRatio
            height: parent.height
            color: "#2e3340"
            Text {
                text: "Прогресс"
                anchors.centerIn: parent
                color: "#cff7d5"
                font.bold: true
                font.pixelSize: 12
            }
        }
        Rectangle {
            width: parent.width * root.timeColumnRatio
            height: parent.height
            color: "#2e3340"
            Text {
                text: "Время"
                anchors.centerIn: parent
                color: "#cff7d5"
                font.bold: true
                font.pixelSize: 12
            }
        }
        Rectangle {
            width: parent.width * root.keyColumnRatio - headerRow.spacing * 5
            height: parent.height
            color: "#2e3340"
            Text {
                text: "XOR-Ключ"
                anchors.centerIn: parent
                color: "#cff7d5"
                font.bold: true
                font.pixelSize: 12
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
            case 4:
                return availableWidth * root.timeColumnRatio
            case 5:
                return availableWidth * root.keyColumnRatio
            default:
                return availableWidth * 0.16
            }
        }

        rowHeightProvider: function (row) {
            return 35
        }

        delegate: Rectangle {
            implicitHeight: 35
            color: (row % 2 === 0) ? "#1e222a" : "#262a33"

            Text {
                anchors.fill: parent
                anchors.margins: 8
                text: {
                    switch (column) {
                    case 3:
                        return model.display
                    case 5:
                        var key = model.display
                        return key.length > 16 ? key.substring(0,
                                                               12) + "..." : key
                    default:
                        return model.display
                    }
                }
                color: {
                    switch (column) {
                    case 2:
                        if (model.display === "Готово")
                            return "#4CAF50"
                        if (model.display === "Ошибка")
                            return "#F44336"
                        if (model.display === "В процессе")
                            return "#FF9800"
                        return "#cff7d5"
                    default:
                        return "#cff7d5"
                    }
                }
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 11
            }
        }

        onWidthChanged: {
            if (table.contentItem) {
                table.forceLayout()
            }
        }
    }
}
