#ifndef PROFIL_H
#define PROFIL_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QStandardItem>
#include <QDebug>

#include "menu.h"
#include "dbconnection.h"


namespace Ui {
class Profil;
}

class Profil : public QDialog
{
    Q_OBJECT

public:
    explicit Profil(QWidget *parent = nullptr);
    ~Profil();

private slots:
    void on_Retour_clicked();



    void chargerCptPfl(QString profil); // charger les compétences d'un profil


    void onComboBoxSelectionChanged(int index); //Lorsque l'user choisis autre chose dans la liste des profils pour un ajout ou une suppression de cpt


    void on_ListerPfl_clicked();
    void chargerlesPfl();

    void on_addNewPfl_clicked();

    void updatePflLists(); // Mettre à jour les listes déroulantes des profils

    void sortComboBox(QComboBox *comboBox);

    void on_SuppPfl_clicked();

    void on_majInfoPfl_clicked();

    void on_AddCpt_clicked();

    void on_SupCpt_clicked();

    void on_consulterInfoPfl_clicked();
    bool isSubset(const std::vector<QString> &profilCompetences, const std::vector<QString> &professionnelCompetences);

private:
    Ui::Profil *ui;

};

#endif // PROFIL_H
