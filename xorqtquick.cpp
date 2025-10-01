#include "xorqtquick.h"
#include "tablemodel.h"
#include "worker.h"

#include <QDir>
#include <QFileInfo>
#include <QRegularExpression>
#include <QTimer>
#include <QThread>
#include <QSharedPointer>

XorQtQuick::XorQtQuick(QObject *parent)
    : QObject(parent)
{
}

const unsigned short TO_SECONDS = 1000;
const unsigned short HEX_KEY_LENGTH = 16;
const double PROGRESS_COMPLETE = 100;

QByteArray XorQtQuick::normalizeXorKey(const QString &xorKey) {
    const QString key = xorKey.toUpper();
    QString cleaned = key;
    static const QRegularExpression validator("[^0-9A-F]");
    cleaned.remove(validator);

    if (cleaned.size() != HEX_KEY_LENGTH) {
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

    const QDir dir(QFileInfo(fileMask).absolutePath());
    const QStringList files = dir.entryList({QFileInfo(fileMask).fileName()}, QDir::Files);
    if (files.isEmpty()) {
        showError("Файлы по заданной маске не найдены");
        return;
    }

    for (const QString &fileName : files) {
        runTask(dir.absoluteFilePath(fileName), xorKeyBytes, outputDir, deleteFile, overwriteMode);
    }

    if (timerRunMode) {
        QTimer* timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [=]() {
            QStringList newFiles = dir.entryList({QFileInfo(fileMask).fileName()}, QDir::Files);
            for (const QString &fileName : std::as_const(newFiles)) {
                QString filePath = dir.absoluteFilePath(fileName);
                runTask(filePath, xorKeyBytes, outputDir, deleteFile, overwriteMode);
            }
        });
        timer->start(timerValue * TO_SECONDS);
        m_activeTimers_.push_back(timer);
    }
}

void XorQtQuick::runTask(const QString &filePath,
                         const QByteArray &xorKeyBytes,
                         const QString &outputDir,
                         const bool &deleteFile,
                         const bool &overwrite)
{
    const QDir outDir(outputDir);
    if (!outDir.exists()) {
        if (!outDir.mkpath(".")) {
            showError("Не удалось создать директорию: " + outputDir);
            return;
        }
    }

    QString outputFilePath = outDir.absoluteFilePath(QFileInfo(filePath).fileName());

    if (!overwrite) {
        const QFileInfo info(filePath);
        QFileInfo outInfo(outputFilePath);

        for (int fileId = 1; outInfo.exists(); ++fileId) {
            QString newName = info.baseName() +
                              "_" + QString::number(fileId) +
                              "." + info.suffix();
            outputFilePath = outDir.absoluteFilePath(newName);
            outInfo.setFile(outputFilePath);
        }
    }

    QString xorKeyHex = xorKeyBytes.toHex().toUpper();

    int recordId = -1;
    if (globalTableModel) {
        globalTableModel->addRecord(QFileInfo(filePath).fileName(), "В процессе", 0.0, xorKeyHex);
        recordId = globalTableModel->rowCount() > 0
                       ? globalTableModel->index(globalTableModel->rowCount()-1, 0).data().toInt()
                       : -1;
    }

    QThread* thread = new QThread(this);
    Worker* worker = new Worker(filePath, xorKeyBytes, outputFilePath, overwrite, deleteFile);
    worker->moveToThread(thread);

    connect(worker, &Worker::progress, this, [this, recordId](int percent) {
        if (globalTableModel && recordId != -1) {
            globalTableModel->updateColumn(recordId, "progress", percent);
        }
        emit progressChanged(percent);
    });

    connect(worker, &Worker::finished, this, [this, recordId, thread](const QString &outPath) {
        if (globalTableModel && recordId != -1) {
            globalTableModel->updateColumn(recordId, "status", "Готово");
            globalTableModel->updateColumn(recordId, "progress", PROGRESS_COMPLETE);
        }
        emit progressChanged(PROGRESS_COMPLETE);
        showInfo("Обработка завершена: " + outPath);
        thread->quit();
    });

    connect(worker, &Worker::error, this, [this, recordId, thread](const QString &msg) {
        if (globalTableModel && recordId != -1) {
            globalTableModel->updateColumn(recordId, "status", "Ошибка");
        }
        showError(msg);
        thread->quit();
    });

    connect(thread, &QThread::started, worker, &Worker::process);
    thread->start();

    m_activeThreads_.push_back(thread);
    m_activeWorkers_.push_back(worker);

    connect(thread, &QThread::finished, worker, &QObject::deleteLater);
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);
}


void XorQtQuick::showError(const QString &msg) {
    emit showMessage("Ошибка", msg, true);
}

void XorQtQuick::showInfo(const QString &msg) {
    emit showMessage("Информация", msg, false);
}

