import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: root
    width: 800
    height: 900
    visible: true
    title: "XOR Шифратор файлов"

    Rectangle {
        anchors.fill: parent
        color: "#0e1117"
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        CustomLabel {
            text: "Основные параметры"
            font.pixelSize: 20
            font.bold: true
        }

        CustomTextField {
            id: fileMask
            placeholderText: "Маска файлов (*.txt, *.bin)"
            Layout.fillWidth: true
        }

        CustomTextField {
            id: outputDir
            placeholderText: "Путь для сохранения файлов"
            Layout.fillWidth: true
        }

        CustomTextField {
            id: xorKey
            placeholderText: "XOR-ключ (16 hex-символов = 8 байт)"
            Layout.fillWidth: true
            maximumLength: 16
        }

        CustomLabel {
            text: "Дополнительные параметры"
            font.pixelSize: 20
            font.bold: true
        }

        CustomCheckBox {
            id: deleteFile
            text: "Удалять входные файлы после обработки"
        }

        RowLayout {
            spacing: 20
            CustomCheckBox {
                id: overwriteMode
                text: "Перезапись"
                checked: true
                onToggled: counterMode.checked = !checked
            }
            CustomCheckBox {
                id: counterMode
                text: "Добавление счётчика"
                onToggled: overwriteMode.checked = !checked
            }
        }

        CustomLabel {
            text: "Режим работы"
            font.pixelSize: 20
            font.bold: true
        }

        RowLayout {
            spacing: 20
            CustomCheckBox {
                id: singleRunMode
                text: "Одноразовый запуск"
                checked: true
                onToggled: {
                    timerRunMode.checked = !checked
                    spinSlider.enabled = false
                }
            }
            CustomCheckBox {
                id: timerRunMode
                text: "Запуск по таймеру"
                onToggled: {
                    singleRunMode.checked = !checked
                    spinSlider.enabled = true
                }
            }
        }

        SpinSlider {
            id: spinSlider

            from: 1
            to: 300

            stepSize: 1
            value: 1

            enabled: false
        }

        RowLayout {
            spacing: 20
            Layout.alignment: Qt.AlignHCenter

            CustomButton {
                text: "Запустить"
                onClicked: {
                    if (backend.isValidKey(xorKey.text)) {
                        backend.execute(fileMask.text, outputDir.text,
                                        xorKey.text, deleteFile.checked,
                                        overwriteMode.checked,
                                        counterMode.checked, singleRunMode.checked,
                                        timerRunMode.checked, spinSlider.value)
                    } else {
                        messageDialog.dialogTitle = "Некорректный ключ"
                        messageDialog.dialogText = "Необходимо ввести 16 hex-символов."
                        messageDialog.isError = true
                        messageDialog.open()
                    }
                }
            }

            CustomButton {
                text: "Выход"
                primaryColor: "#6c1c1c"
                hoverColor: "#922424"
                accentColor: "#5a0f0f"
                onClicked: Qt.quit()
            }
        }

        CustomProgressBar {
            id: progressBar
            Layout.fillWidth: true
            value: 0
            visible: false
        }

        CustomLabel {
            text: "История обработанных файлов"
            font.pixelSize: 20
            font.bold: true
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 20
            spacing: 20

            Row {
                id: headerRow
                Layout.fillWidth: true
                height: 32
                spacing: 1

                Rectangle {
                    width: 50
                    height: parent.height
                    color: "#2e3340"
                    Text {
                        text: "ID"
                        anchors.centerIn: parent
                        color: "white"
                        font.bold: true
                    }
                }
                Rectangle {
                    width: 250
                    height: parent.height
                    color: "#2e3340"
                    Text {
                        text: "Файл"
                        anchors.centerIn: parent
                        color: "white"
                        font.bold: true
                    }
                }
                Rectangle {
                    width: 120
                    height: parent.height
                    color: "#2e3340"
                    Text {
                        text: "Статус"
                        anchors.centerIn: parent
                        color: "white"
                        font.bold: true
                    }
                }
                Rectangle {
                    width: 100
                    height: parent.height
                    color: "#2e3340"
                    Text {
                        text: "Прогресс"
                        anchors.centerIn: parent
                        color: "white"
                        font.bold: true
                    }
                }
            }

            TableView {
                id: historyTable
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true

                model: tableModel

                columnWidthProvider: function (column) {
                    switch (column) {
                    case 0: return 50;   // ID
                    case 1: return 250;  // Файл
                    case 2: return 120;  // Статус
                    case 3: return 100;  // Прогресс
                    default: return 100;
                    }
                }

                rowHeightProvider: function (row) { return 30 }

                delegate: Rectangle {
                    implicitHeight: 30
                    color: (row % 2 === 0) ? "#1e222a" : "#262a33"

                    Text {
                        anchors.centerIn: parent
                        text: model.display
                        color: "white"
                        elide: Text.ElideRight
                    }
                }
            }
        }




        CustomDialog {
            id: messageDialog
        }
    }

    Connections {
        target: backend

        function onProgressChanged(percent) {
            progressBar.visible = true
            progressBar.value = percent
            if (percent === 100)
                progressBar.visible = false
        }

        function onShowMessage(title, message, isError) {
            messageDialog.dialogTitle = title
            messageDialog.dialogText = message
            messageDialog.isError = isError
            messageDialog.open()
        }
    }
}
