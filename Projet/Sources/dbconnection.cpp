#include "dbconnection.h"

DbConnection::DbConnection()
{
  database = QSqlDatabase::addDatabase(kDbType);
  database.exec("SET NAMES 'utf8'");
  database.setDatabaseName(kDbName);
  database.setConnectOptions("MYSQL_SET_NAME_UTF8");

}

DbConnection::~DbConnection() {
  close();
}

bool DbConnection::open()
{
  if (QFile::exists(kDbName))
  {
    if (!database.open())
    {
      qDebug() << "Failed to open database:" << database.lastError().text();
      return false;
    }
    qDebug() << "Connected to database.";
    return true;
  }
  else
  {
    qDebug() << "Database file not found:" << kDbName;
    return false;
  }
}

void DbConnection::close()
{
  if (database.isOpen())
  {
    database.close();
    QSqlDatabase::removeDatabase(kDbName);
  }
}
