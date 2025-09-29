#include "xorqtquick.h"
#include "tablemodel.h"
#include "worker.h"

#include <QDir>
#include <QFileInfo>
#include <QRegularExpression>
#include <QTimer>


XorQtQuick::XorQtQuick(QObject *parent)
    : QObject(parent)
{
}

const unsigned short TOSECONDS = 1000;
const unsigned short HEXKEYLENGTH = 16;


QByteArray XorQtQuick::normalizeXorKey(const QString &xorKey) {
    QString key = xorKey.toUpper();
    QString cleaned = key;
    static QRegularExpression validator("[^0-9A-F]");
    cleaned.remove(validator);

    if (cleaned.size() != HEXKEYLENGTH) {
        showError("Неверный XOR-ключ (требуется 8 байт в hex)");
        return QByteArray();
    }
    return QByteArray::fromHex(cleaned.toUtf8());
}

void XorQtQuick::execute(const QString &fileMask,
                         const QString &outputDir,
                         const QString &xorKey,
                         bool deleteFile,
                         bool overwriteMode,
                         bool modifierMode,
                         bool singleRunMode,
                         bool timerRunMode,
                         int timerValue)
{
    QByteArray xorKeyBytes = normalizeXorKey(xorKey);
    if (xorKeyBytes.isEmpty()) {
        return;
    }

    QDir dir(QFileInfo(fileMask).absolutePath());
    QStringList files = dir.entryList({QFileInfo(fileMask).fileName()}, QDir::Files);
    if (files.isEmpty()) {
        showError("Файлы по заданной маске не найдены");
        return;
    }

    for (const QString &fileName : std::as_const(files)) {
        QString filePath = dir.absoluteFilePath(fileName);
        runTask(filePath, xorKeyBytes, outputDir, deleteFile, overwriteMode);
    }

    if (timerRunMode) {
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [=]() {
            QStringList files = dir.entryList({QFileInfo(fileMask).fileName()}, QDir::Files);
            for (const QString &fileName : std::as_const(files)) {
                QString filePath = dir.absoluteFilePath(fileName);
                runTask(filePath, xorKeyBytes, outputDir, deleteFile, overwriteMode);
            }
        });
        timer->start(timerValue * TOSECONDS);
    }
}

void XorQtQuick::runTask(const QString &filePath,
                         const QByteArray &xorKeyBytes,
                         const QString &outputDir,
                         bool deleteFile,
                         bool overwrite)
{
    QString outputFilePath = outputDir + "/" + QFileInfo(filePath).fileName();

    if (!overwrite) {
        int fileId = 1;
        QFileInfo info(filePath);
        while (QFile::exists(outputFilePath)) {
            outputFilePath = outputDir + "/" +
                             info.baseName() + "_" + QString::number(fileId++) + "." + info.suffix();
        }
    }

    int recordId = -1;
    if (g_tableModel) {
        g_tableModel->addRecord(QFileInfo(filePath).fileName(), "В процессе", 0.0);
        recordId = g_tableModel->rowCount() > 0
                       ? g_tableModel->index(g_tableModel->rowCount()-1, 0).data().toInt()
                       : -1;
    }


    QThread *thread = new QThread;
    Worker *worker = new Worker(filePath, xorKeyBytes, outputFilePath,
                                overwrite, deleteFile);

    worker->moveToThread(thread);

    connect(thread, &QThread::started, worker, &Worker::process);
    connect(worker, &Worker::progress, this, [recordId, this](int percent) {
        if (g_tableModel && recordId != -1) {
            g_tableModel->updateColumn(recordId, "progress", percent);
        }
        emit progressChanged(percent);
    });
    connect(worker, &Worker::finished, this, [&, recordId, thread](const QString &outPath) {
        if (g_tableModel && recordId != -1) {
            g_tableModel->updateColumn(recordId, "status", "Готово");
            g_tableModel->updateColumn(recordId, "progress", 100);
        }
        emit progressChanged(100);
        showInfo("Обработка завершена: " + outPath);
        thread->quit();
    });
    connect(worker, &Worker::error, this, [&, recordId, thread](const QString &msg) {
        if (g_tableModel && recordId != -1) {
            g_tableModel->updateColumn(recordId, "status", "Ошибка");
        }
        showError(msg);
        thread->quit();
    });

    connect(thread, &QThread::finished, worker, &QObject::deleteLater);
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);

    thread->start();
}

void XorQtQuick::showError(const QString &msg) {
    emit showMessage("Ошибка", msg, true);
}

void XorQtQuick::showInfo(const QString &msg) {
    emit showMessage("Информация", msg, false);
}

