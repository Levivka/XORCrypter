#include "tablemodel.h"
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

TableModel* globalTableModel = nullptr;

TableModel::TableModel(QObject *parent) : QAbstractTableModel(parent) {
    globalTableModel = this;
    initDatabase();
    loadFromDatabase();
}


void TableModel::initDatabase() {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("database.db");
    if (!m_db.open()) {
        qWarning() << "Не удалось открыть БД:" << m_db.lastError();
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS records ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "fileName TEXT, "
               "status TEXT, "
               "progress REAL, "
               "timestamp TEXT, "
               "xorKey TEXT)");
}

int TableModel::rowCount(const QModelIndex &) const {
    return m_records.size();
}

int TableModel::columnCount(const QModelIndex &) const {
    return ColumnCount;
}

QVariant TableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole)
        return {};

    const auto &rec = m_records[index.row()];
    switch (index.column()) {
    case IdColumn: return rec.id;
    case FileNameColumn: return rec.fileName;
    case StatusColumn: return rec.status;
    case ProgressColumn: return QString("%1 %").arg(rec.progress);
    case TimeColumn: return rec.timestamp;
    case KeyColumn: return rec.xorKey;
    }
    return {};
}

QVariant TableModel::headerData(int section, const Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case IdColumn: return "ID";
        case FileNameColumn: return "Файл";
        case StatusColumn: return "Статус";
        case ProgressColumn: return "Прогресс";
        case TimeColumn: return "Время";
        case KeyColumn: return "XOR-Ключ";
        }
    }
    return {};
}

void TableModel::addRecord(const QString &fileName, const QString &status, double progress, const QString &xorKey) {
    beginInsertRows(QModelIndex(), m_records.size(), m_records.size());

    EncryptionRecord rec;
    rec.id = -1;
    rec.fileName = fileName;
    rec.status = status;
    rec.progress = progress;
    rec.timestamp = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
    rec.xorKey = xorKey;

    m_records.push_back(rec);
    endInsertRows();

    syncRecordToDatabase(m_records.last());
}

void TableModel::updateColumn(int id, const QString &columnName, const QVariant &value) {
    auto it = std::find_if(m_records.begin(), m_records.end(),
                           [id](const EncryptionRecord &rec) { return rec.id == id; });

    if (it == m_records.end())
        return;

    int row = std::distance(m_records.begin(), it);

    if (columnName == "progress") {
        double newProgress = value.toDouble();
        if (qFuzzyCompare(it->progress, newProgress))
            return;

        if (newProgress - it->progress < 1.0 && newProgress != 100.0)
            return;

        it->progress = newProgress;
    }
    else if (columnName == "status") {
        it->status = value.toString();
    }
    else {
        return;
    }

    QSqlQuery query;
    query.prepare(QString("UPDATE records SET %1 = :val WHERE id = :id").arg(columnName));
    query.bindValue(":val", value);
    query.bindValue(":id", id);
    query.exec();

    emit dataChanged(index(row, 0), index(row, ColumnCount - 1));
}




void TableModel::loadFromDatabase() {
    beginResetModel();

    m_records.clear();

    QSqlQuery query("SELECT id, fileName, status, progress, timestamp, xorKey FROM records");
    while (query.next()) {
        EncryptionRecord rec;

        rec.id = query.value(0).toInt();
        rec.fileName = query.value(1).toString();
        rec.status = query.value(2).toString();
        rec.progress = query.value(3).toDouble();
        rec.timestamp = query.value(4).toString();
        rec.xorKey = query.value(5).toString();

        m_records.push_back(rec);
    }

    endResetModel();
}

void TableModel::syncRecordToDatabase(const EncryptionRecord &rec) {
    QSqlQuery query;
    query.prepare("INSERT INTO records (fileName, status, progress, timestamp, xorKey) "
                  "VALUES (:file, :status, :progress, :timestamp, :xorKey)");

    query.bindValue(":file", rec.fileName);
    query.bindValue(":status", rec.status);
    query.bindValue(":progress", rec.progress);
    query.bindValue(":timestamp", rec.timestamp);
    query.bindValue(":xorKey", rec.xorKey);

    if (!query.exec()) {
        qWarning() << "Ошибка записи в БД:" << query.lastError();
    } else {
        int newId = query.lastInsertId().toInt();
        if (newId > 0) {
            m_records.last().id = newId;
        }
    }
}

void TableModel::clearAll() {
    beginResetModel();

    m_records.clear();

    QSqlQuery query;

    if (!query.exec("DELETE FROM records")) {
        qWarning() << "Ошибка очистки БД:" << query.lastError();
    }

    if (!query.exec("DELETE FROM sqlite_sequence WHERE name='records'")) {
        qWarning() << "Ошибка сброса автоинкремента:" << query.lastError();
    }


    endResetModel();
}
