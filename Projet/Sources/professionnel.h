#ifndef PROFESSIONNEL_H
#define PROFESSIONNEL_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QStandardItem>
#include <QDebug>
#include <algorithm>
#include <iostream>
#include <vector>


#include "menu.h"
#include "dbconnection.h"

namespace Ui {
class Professionnel;
}

class Professionnel : public QDialog
{
    Q_OBJECT

public:
    explicit Professionnel(QWidget *parent = nullptr);
    ~Professionnel();

private slots:
    void on_ListerPro_clicked();//Lister les professionnels

    void on_Retour_clicked();//Retourner au menu

    void chargerlesPro(); //charger les professionnels

    void updateProLists(); // Mettre à jour les listes déroulantes des professionnels

    void onComboBoxSelectionChanged(int index); //Lorsque l'user choisis autre chose dans la liste des professionnels pour un ajout ou une suppression de cpt

    void chargerCptPro(QString identite); // charger les compétences d'un professionnel

    void on_SupCpt_clicked(); // supprimer une compétence à un Professionnel

    void on_AddCpt_clicked(); // ajouter une compétence à un pro

    void on_addNewPro_clicked(); //ajouter un nouveau professionnel

    void on_SuppPro_clicked(); //supprimer un professionnel

    void on_consulterInfoPro_clicked();//consulter les infos d'un pro
    bool isSubset(const std::vector<QString> &profilCompetences, const std::vector<QString> &professionnelCompetences);//vérifier si il y a correspondance entre les listes

    void on_majInfoPro_clicked();//Mettre à jour les informations d'un pro

    void sortComboBox(QComboBox *comboBox);//Trier les comboBox

private:
    Ui::Professionnel *ui;
};

#endif // PROFESSIONNEL_H
