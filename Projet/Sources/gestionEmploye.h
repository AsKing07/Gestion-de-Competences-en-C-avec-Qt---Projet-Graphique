#ifndef GESTIONEMPLOYE_H
#define GESTIONEMPLOYE_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QStandardItem>
#include <QDebug>
#include <QStringListModel>

#include "menu.h"
#include "dbconnection.h"

namespace Ui {
class gestionEmploye;
}

class gestionEmploye : public QDialog
{
    Q_OBJECT

public:
    explicit gestionEmploye(QWidget *parent = nullptr);
    ~gestionEmploye();

private slots:
    void on_menu_clicked();

    void updateEmpLists();

     void chargerlesEmp();

    void on_listerEmp_clicked();

    void on_AjouterEmploye_clicked();



    void on_Supprimer_clicked();

    void on_pushButton_clicked();

private:
    Ui::gestionEmploye *ui;

};

#endif // GESTIONEMPLOYE_H
