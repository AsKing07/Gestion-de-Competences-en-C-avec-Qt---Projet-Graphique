#ifndef COMPETENCES_H
#define COMPETENCES_H

#include <QDialog>

#include "dbconnection.h"
#include "menu.h"

namespace Ui {
class Competences;
}

class Competences : public QDialog
{
    Q_OBJECT

public:
    explicit Competences(QWidget *parent = nullptr);
    ~Competences();

private slots:
    void on_Menu_clicked(); // slot du retour au menu

    void on_Lister_clicked(); //afficher ttes les compétences

    void on_supprimer_clicked(); //Supprimer une cpt

    void on_ajouter_clicked(); //ajouter une cpt

    void on_maj_clicked(); //mettre à jour le nom d'une compétence

    void updateCompetenceLists(); // recharger la liste des compétences dans les comboBox

    void chargerlesCpt(); //charger les compétence

private:
    Ui::Competences *ui;
};

#endif // COMPETENCES_H
