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
                                        counterMode.checked,
                                        singleRunMode.checked,
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
        HistoryTable {
            model: tableModel
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
