#ifndef MENU_H
#define MENU_H

// Inclusion des bibliothèques nécessaires pour la classe menu
#include <QDialog>
#include <QWidget>
#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include "accueil.h"
#include "profil.h"
#include "competences.h"
#include "professionnel.h"
#include "gestionEmploye.h"

//Début de déclaration du namespace
namespace Ui {
class menu;
}

//Définition de la classe menu
class menu : public QDialog
{
    Q_OBJECT

public:
    explicit menu(QWidget *parent = nullptr ); //Constructeur du menu

    ~menu(); // Destructeur du menu


private slots:
    void on_Profil_clicked(); // Slot pour l'ouverture de la fenetre de gestion des profils

    void on_Competence_clicked(); // Slot pour l'ouverture de la fenetre de gestion des competences

    void on_Professionnel_clicked(); // Slot pour l'ouverture de la fenetre de gestion des professionnels

    void on_employe_clicked();  // Slot pour l'ouverture de la fenetre de gestion des employés

    void on_Importation_clicked(); // Slot pour l'importation des données d'un fichier dans la bdd

    void on_Exportation_clicked(); // Slot pour l'exportation des données de la bdd dans un fichier

    void on_Reinitialiser_clicked(); // Slot pour réinitialiser la base de donnée

private:
    Ui::menu *ui; //Pointeur vers l'interface graphique du menu
};

#endif // MENU_H
