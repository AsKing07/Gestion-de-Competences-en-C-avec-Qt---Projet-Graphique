#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include "accueil.h"

class DbConnection
{
  public:
    DbConnection();
    ~DbConnection();
    bool open();
    void close();
  private:
    const QString kDbType = "QSQLITE";
    const QString kDbName = "../Build/gestioncpt.db";
    QSqlDatabase database;
};



#endif // DBCONNECTION_H
