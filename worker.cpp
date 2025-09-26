#include "worker.h"

#include <QFile>
#include <QFileInfo>

const unsigned short BYTES = 1024;
const unsigned short TOPERCENT = 100;

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

    while (!(buffer = inputFile.read(BYTES ^ 2)).isEmpty()) {
        for (int i = 0; i < buffer.size(); ++i) {
            buffer[i] = buffer[i] ^ key[(bytesRead + i) % keyLength];
        }
        outputFile.write(buffer);
        bytesRead += buffer.size();

        int percentage = int((double(bytesRead) / totalSize) * TOPERCENT);

        emit progress(percentage);
    }

    inputFile.close();
    outputFile.close();

    if (deleteInput) {
        QFile::remove(filePath);
    }

    emit finished(outputPath);
}
