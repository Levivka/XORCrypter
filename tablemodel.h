#pragma once
#include <QAbstractTableModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVector>

struct EncryptionRecord {
    int id;
    QString fileName;
    QString status;
    double progress;
};


class TableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit TableModel(QObject *parent = nullptr);


    enum Columns {
        IdColumn,
        FileNameColumn,
        StatusColumn,
        ProgressColumn,
        ColumnCount
    };
    Q_ENUM(Columns)

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    Q_INVOKABLE void updateColumn(int id, const QString &columnName, const QVariant &value);
    Q_INVOKABLE void addRecord(const QString &fileName, const QString &status, double progress);
    Q_INVOKABLE void loadFromDatabase();

private:
    QVector<EncryptionRecord> m_records;
    QSqlDatabase m_db;

    void initDatabase();
    void syncRecordToDatabase(const EncryptionRecord &rec);
};

extern TableModel* g_tableModel;

