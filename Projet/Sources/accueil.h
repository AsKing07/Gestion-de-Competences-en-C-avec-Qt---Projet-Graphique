#ifndef ACCUEIL_H
#define ACCUEIL_H

// Inclusion des bibliothèques nécessaires pour la classe Accueil
#include <QDialog>
#include <QApplication>
#include <QWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QtDebug>
#include <QFileInfo>
#include <QtSql>
#include "menu.h"
#include "dbconnection.h"


// Début de la déclaration du namespace
QT_BEGIN_NAMESPACE
namespace Ui { class Accueil; }
QT_END_NAMESPACE

// Définition de la classe Accueil
class Accueil : public QDialog
{
    Q_OBJECT


// Déclaration des slots de la classe Accueil
private slots:
    void on_Connection_clicked(); // Slot pour la connexion à la base de données

    void on_apropos_clicked(); // Slot pour afficher les informations sur l'application

public:
    Accueil(QWidget *parent = nullptr);// Constructeur de la classe Accueil
    ~Accueil(); // Destructeur de la classe Accueil

private:
    Ui::Accueil *ui;// Pointeur vers l'interface graphique de la classe Accueil
};

#endif // ACCUEIL_H
