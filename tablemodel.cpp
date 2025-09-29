#include "tablemodel.h"
#include <QSqlError>
#include <QDebug>

TableModel* globalTableModel = nullptr;

TableModel::TableModel(QObject *parent) : QAbstractTableModel(parent) {
    globalTableModel = this;
    initDatabase();
    loadFromDatabase();
}


void TableModel::initDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.db");
    if (!db.open()) {
        qWarning() << "Не удалось открыть БД:" << db.lastError();
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS records ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "fileName TEXT, "
               "status TEXT, "
               "progress REAL)");
}

int TableModel::rowCount(const QModelIndex &) const {
    return records.size();
}

int TableModel::columnCount(const QModelIndex &) const {
    return ColumnCount;
}

QVariant TableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole)
        return {};

    const auto &rec = records[index.row()];
    switch (index.column()) {
    case IdColumn: return rec.id;
    case FileNameColumn: return rec.fileName;
    case StatusColumn: return rec.status;
    case ProgressColumn: return QString("%1 %").arg(rec.progress);
    }
    return {};
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case IdColumn: return "ID";
        case FileNameColumn: return "Файл";
        case StatusColumn: return "Статус";
        case ProgressColumn: return "Прогресс";
        }
    }
    return {};
}

void TableModel::addRecord(const QString &fileName, const QString &status, double progress) {
    beginInsertRows(QModelIndex(), records.size(), records.size());
    EncryptionRecord rec{-1, fileName, status, progress};
    records.push_back(rec);
    endInsertRows();

    syncRecordToDatabase(records.last());
    loadFromDatabase();
}

void TableModel::updateColumn(int id, const QString &columnName, const QVariant &value) {
    for (int i = 0; i < records.size(); ++i) {
        if (records[i].id == id) {
            if (columnName == "status") records[i].status = value.toString();
            else if (columnName == "progress") records[i].progress = value.toDouble();

            QSqlQuery query;
            query.prepare(QString("UPDATE records SET %1 = :val WHERE id = :id").arg(columnName));
            query.bindValue(":val", value);
            query.bindValue(":id", id);

            query.exec();

            emit dataChanged(index(i, 0), index(i, ColumnCount - 1));
            break;
        }
    }
}

void TableModel::loadFromDatabase() {
    beginResetModel();

    records.clear();

    QSqlQuery query("SELECT id, fileName, status, progress FROM records");
    while (query.next()) {
        EncryptionRecord rec;

        rec.id = query.value(0).toInt();
        rec.fileName = query.value(1).toString();
        rec.status = query.value(2).toString();
        rec.progress = query.value(3).toDouble();

        records.push_back(rec);
    }

    endResetModel();
}

void TableModel::syncRecordToDatabase(const EncryptionRecord &rec) {
    QSqlQuery query;
    query.prepare("INSERT INTO records (fileName, status, progress) VALUES (:file, :status, :progress)");

    query.bindValue(":file", rec.fileName);
    query.bindValue(":status", rec.status);
    query.bindValue(":progress", rec.progress);

    query.exec();
}
