#include "worker.h"

#include <QFile>
#include <QFileInfo>

Worker::Worker(const QString &filePath,
               const QByteArray &key,
               const QString &outputPath,
               bool overwrite,
               bool deleteInput,
               QObject *parent)
    : QObject(parent),
    filePath(filePath),
    key(key),
    outputPath(outputPath),
    overwrite(overwrite),
    deleteInput(deleteInput) {}

void Worker::process() {
    QFile inputFile(filePath);
    if (!inputFile.open(QIODevice::ReadOnly)) {
        emit error("Не удалось открыть файл: " + filePath);
        return;
    }

    QFile outputFile(outputPath);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        emit error("Не удалось открыть файл для записи: " + outputPath);
        return;
    }

    const auto totalSize = inputFile.size();
    qint64 bytesRead = 0;
    QByteArray buffer;
    const auto keyLength = key.size();

    while (!(buffer = inputFile.read(1024 * 1024)).isEmpty()) {
        for (int i = 0; i < buffer.size(); ++i) {
            buffer[i] = buffer[i] ^ key[(bytesRead + i) % keyLength];
        }
        outputFile.write(buffer);
        bytesRead += buffer.size();

        int percent = int((double(bytesRead) / totalSize) * 100);

        emit progress(percent);
    }

    inputFile.close();
    outputFile.close();

    if (deleteInput) {
        QFile::remove(filePath);
    }

    emit finished(outputPath);
}
