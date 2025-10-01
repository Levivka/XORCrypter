#include "worker.h"

#include <QFile>
#include <QFileInfo>

const unsigned short BYTES = 1024;
const unsigned short TO_PERCENT = 100;

Worker::Worker(const QString &filePath,
               const QByteArray &key,
               const QString &outputPath,
               bool overwrite,
               bool deleteInput,
               QObject *parent)
    : QObject(parent),
    m_filePath(filePath),
    m_key(key),
    m_outputPath(outputPath),
    m_overwrite(overwrite),
    m_deleteInput(deleteInput) {}

void Worker::process() {
    QFile inputFile(m_filePath);
    if (!inputFile.open(QIODevice::ReadOnly)) {
        emit error("Не удалось открыть файл: " + m_filePath);
        return;
    }

    QFile outputFile(m_outputPath);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        emit error("Не удалось открыть файл для записи: " + m_outputPath);
        return;
    }

    const auto totalSize = inputFile.size();
    const auto keyLength = m_key.size();
    qint64 bytesRead = 0;
    QByteArray buffer;


    while (!(buffer = inputFile.read(BYTES * BYTES)).isEmpty()) {
        const int bufSize = buffer.size();
        for (int i = 0; i < bufSize; ++i) {
            buffer[i] ^= m_key[(bytesRead + i) % keyLength];
        }

        outputFile.write(buffer);
        bytesRead += buffer.size();

        int percentage = 0;
        if (totalSize > 0) {
            percentage = static_cast<int>(
                (static_cast<double>(bytesRead) / static_cast<double>(totalSize)) * TO_PERCENT
                );
        }

        emit progress(percentage);
    }

    inputFile.close();
    outputFile.close();

    if (m_deleteInput) {
        QFile::remove(m_filePath);
    }

    emit finished(m_outputPath);
}
