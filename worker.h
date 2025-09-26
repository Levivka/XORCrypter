#pragma once

#include <QObject>
#include <QByteArray>
#include <QString>

class Worker : public QObject {
    Q_OBJECT
public:
    Worker(const QString &filePath,
           const QByteArray &key,
           const QString &outputPath,
           bool overwrite,
           bool deleteInput,
           QObject *parent = nullptr);

signals:
    void progress(int percentage);
    void finished(const QString &outputPath);
    void error(const QString &message);

public slots:
    void process();

private:
    QString filePath;
    QString outputPath;
    QByteArray key;
    bool overwrite;
    bool deleteInput;
};
