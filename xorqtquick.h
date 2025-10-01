#pragma once
#include "worker.h"

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QByteArray>

class XorQtQuick : public QObject {
    Q_OBJECT
public:
    explicit XorQtQuick(QObject *parent = nullptr);

    Q_INVOKABLE void execute(const QString &fileMask,
                             const QString &outputDir,
                             const QString &xorKey,
                             bool deleteFile,
                             bool overwriteMode,
                             bool modifierMode,
                             bool singleRunMode,
                             bool timerRunMode,
                             int timerValue);
    Q_INVOKABLE bool isValidKey(const QString &xorKey) {
        return !normalizeXorKey(xorKey).isEmpty();
    };


signals:
    void progressChanged(int value);
    void finished(const QString &outPath);
    void error(const QString &msg);
    void showMessage(const QString &title, const QString &message, bool isError);


private:
    QByteArray normalizeXorKey(const QString &xorKey);
    void runTask(const QString &filePath,
                 const QByteArray &xorKeyBytes,
                 const QString &outputDir,
                 const bool &deleteFile,
                 const bool &overwrite);
    void showError(const QString &msg);
    void showInfo(const QString &msg);
    \
    std::vector<QThread*> m_activeThreads_;
    std::vector<Worker*>  m_activeWorkers_;
    std::vector<QTimer*>  m_activeTimers_;
};
