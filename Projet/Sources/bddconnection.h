#ifndef BDDCONNECTION_H
#define BDDCONNECTION_H

#include "accueil.h"


void Accueil::connClose()
{
    mydb.close();
    mydb.removeDatabase(QSqlDatabase::defaultConnection);
}
bool Accueil::connOpen()

{
    mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("gstioncpt.sql");

    if(!mydb.open())
    {
        qDebug() << "Echec de la connexion";
        return false;
    }
    else
    {
        qDebug() << "Connecté...";
        return true;
    }
}


#endif // BDDCONNECTION_H
