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
    const auto keyLength = key.size();
    qint64 bytesRead = 0;
    QByteArray buffer;


    while (!(buffer = inputFile.read(BYTES ^ 2)).isEmpty()) {
        for (int i = 0; i < buffer.size(); ++i) {
            buffer[i] = buffer[i] ^ key[(bytesRead + i) % keyLength];
        }

        outputFile.write(buffer);
        bytesRead += buffer.size();

        int percentage = 0;
        if (totalSize > 0) {
            percentage = static_cast<int>(
                (static_cast<double>(bytesRead) / static_cast<double>(totalSize)) * TOPERCENT
                );
        }

        emit progress(percentage);
    }

    inputFile.close();
    outputFile.close();

    if (deleteInput) {
        QFile::remove(filePath);
    }

    emit finished(outputPath);
}
