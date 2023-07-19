#include "profil.h"
#include "ui_profil.h"

Profil::Profil(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Profil)
{
    ui->setupUi(this);
     setWindowTitle("Profil");
    updatePflLists();
     connect(ui->comboBox_PflAddSuppCpt, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxSelectionChanged(int)));
}

Profil::~Profil()
{
    delete ui;
}


void Profil::updatePflLists()
{
    DbConnection connexion;

    if (!connexion.open()) {
        qDebug() << "Echec d'ouverture de la Base de données";
        QMessageBox::critical(this, tr("Erreur de données"), tr("Impossible d'accéder à la base de données"));
        return;
    }

 //   QString text = "Sélectionnez une option";

    // Vérifier si les différents QComboBox ont bien été créés
 //  QList<QComboBox*> comboBoxList = {ui->comboBox_ProAddSuppCpt, ui->comboBox_loadDataPro, ui->comboBox_majInfoPro};

    ui->comboBox_PflAddSuppCpt->clear();
    ui->comboBox_PflAddSuppCpt->insertItem(0, "Sélectionnez une option");
    ui->comboBox_PflAddSuppCpt->setEditable(false);


    ui->comboBox_loadDataPfl->clear();
    ui->comboBox_loadDataPfl->insertItem(0, "Sélectionnez une option");
    ui->comboBox_loadDataPfl->setEditable(false);


    ui->comboBox_majInfoPfl->clear();
    ui->comboBox_majInfoPfl->insertItem(0, "Sélectionnez une option");
    ui->comboBox_majInfoPfl->setEditable(false);


    ui->comboBox_CptToSup->setEditable(false);




    // Ajouter les professionnels à la liste déroulante
    QSqlQuery query;
    query.exec("SELECT nom FROM profil ORDER BY nom");
    if (!query.isActive())
    {
        qDebug() << "Echec de récupération des professionnels";
        return;
    }

    while (query.next())
    {
        ui->comboBox_PflAddSuppCpt->addItem(query.value("nom").toString());
        ui->comboBox_loadDataPfl->addItem(query.value("nom").toString());
        ui->comboBox_majInfoPfl->addItem(query.value("nom").toString());
    }

    connexion.close();

}



void Profil::sortComboBox(QComboBox *comboBox)//trier comBox
{
    QStringList items;
    for (int i = 1; i < comboBox->count(); i++)
    {
        items.append(comboBox->itemText(i));
    }
    items.sort(Qt::CaseInsensitive);
    comboBox->clear();
    comboBox->addItem("Selectionner une option");
    comboBox->addItems(items);
}

void Profil::on_Retour_clicked()
{
    this -> hide();
    menu retour;
    retour.exec();
}

void Profil::chargerCptPfl(QString profil)
{
    DbConnection connexion;
    if(!connexion.open())
    {
         QMessageBox::warning(this, "Erreur d'exécution", "Une erreur s'est produite.");
         return;
    }
    else
    {

        // Préparer la requête SQL
        QSqlQuery query;
        query.prepare("SELECT ListCpt FROM profil WHERE nom = :profil");
        query.bindValue(":profil", profil);

        // Exécuter la requête
        if (!query.exec())
        {
            QMessageBox::warning(this, "Erreur d'exécution", "Une erreur s'st produite.");
            connexion.close();
            return;
        }

        // Vider la boîte déroulante des cpt à supprimer
        ui->comboBox_CptToSup->clear();

        // Récupérer les valeurs de la colonne "ListCpt"
        while (query.next())
        {
            QStringList proCpt = query.value("ListCpt").toString().split(",");
            proCpt.sort(Qt::CaseInsensitive);

            // Ajouter les éléments du QStringList à la boîte déroulante des compétences à supprimer
            for (int i = 0; i < proCpt.size(); i++)
            {
                ui->comboBox_CptToSup->addItem(proCpt[i].trimmed());
            }
        }

        // Fermer la connexion à la base de données
        connexion.close();

    }
}

void Profil::onComboBoxSelectionChanged(int index)
{
     QString choix = ui->comboBox_PflAddSuppCpt->itemText(index);
     chargerCptPfl(choix);

}



void Profil::chargerlesPfl()
{
    DbConnection connexion;
    if(!connexion.open())
    {
        qDebug() << "Echec d'ouverture de la Base de données";
        QMessageBox::critical(this,tr("Erreur de données"), tr("Une erreur s'est produite!"));
    }
    else
    {
        QSqlQuery query;
        query.exec("SELECT nom FROM profil ORDER BY nom");

        QSqlQueryModel *model = new QSqlQueryModel;
        model->setQuery(query);
        ui -> tableView_client ->setModel(model);

        QString statut="liste des profils existant ";


         ui->tableView_client->setColumnWidth(0, 400);
        QHeaderView* header = ui->tableView_client->horizontalHeader();
        header->setDefaultAlignment(Qt::AlignHCenter);
        ui -> tableView_client->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui -> statut -> setText(statut);
    }

        connexion.close();
}



void Profil::on_ListerPfl_clicked()
{
    chargerlesPfl();
}


void Profil::on_addNewPfl_clicked()//Ajouter un nouveau Pfl
{

        QString nomPfl,descPfl,cpt;
        QStringList pfList;
        nomPfl=ui->nomNouvPfl->text().trimmed();
        descPfl=ui->DescriptionNouvPfl->text().trimmed();
        cpt=ui->primCptAddNewPfl->text().trimmed();


        if(nomPfl.isEmpty() || cpt.isEmpty())  //vérification si les champs nom d'utilisateur et mot de passe sont vides
        {
            ui -> statut -> setText("Le nom du profil doit être renseigné!Vous devez également rajouté au moins une compétence de base au Profil! ");//affichage d'un message d'erreur dans l'interface graphique
            QMessageBox::critical(this,"Remplissage","Le nom du profil doit être renseigné!Vous devez également rajouté au moins une compétence de base au Profil! ");//affichage d'une boîte de dialogue d'erreur
            return;//sortie de la méthode
        }



        DbConnection connexion;
        if(!connexion.open())
        {
            // gérer les erreurs de connexion ici
        }
        else
        {
            QSqlQuery query;
            query.prepare("SELECT nom FROM profil WHERE lower(nom) = lower(:nomPfl)");
            query.bindValue(":nomPfl", nomPfl);

            // Exécuter la requête
            if (!query.exec())
            {
                QMessageBox::warning(this, "Erreur d'exécution", "Une erreur s'est produite.");
                connexion.close();
                return;
            }


            while (query.next())
            {
                 pfList = query.value("nom").toString().split(",");
            }
            if(pfList.contains(nomPfl,Qt::CaseInsensitive))
            {
                QMessageBox::critical(this,tr("Echec"),tr("Le profil existe déjà!"));
                connexion.close();
                return;
            }

             QSqlQuery qry;
            if(!descPfl.isEmpty())
            {

                qry.prepare("INSERT INTO   profil(nom,ListCpt, description, nbrCpt) VALUES(:nomPfl, :cpt, :description, 1); ");
                qry.bindValue(":nomPfl", nomPfl);
                qry.bindValue(":cpt", cpt);
                qry.bindValue(":description", descPfl);
            }
            else
            {
                qry.prepare("INSERT INTO   profil(nom,ListCpt,nbrCpt) VALUES(:nomPfl, :cpt, 1); ");
                qry.bindValue(":nomPfl", nomPfl);
                qry.bindValue(":cpt", cpt);
            }

            if(qry.exec())
            {
                QMessageBox::information(this,tr("Succes"),tr("Le profil a bien été ajouté!"));
                ui->nomNouvPfl->setText("");
                ui->DescriptionNouvPfl->setText("");
                ui->primCptAddNewPfl->setText("");
                QList<QComboBox*> comboBoxList = {ui->comboBox_PflAddSuppCpt, ui->comboBox_loadDataPfl, ui->comboBox_majInfoPfl};
                for (QComboBox* comboBox : comboBoxList)
                {
                    comboBox->addItem(nomPfl);
                    sortComboBox(comboBox);
                }
            }
            else
            {
                QMessageBox::information(this,tr("Erreur"),tr("Malheureusement Une erreur s'est produite!"));
            }
            connexion.close();

        }




}


void Profil::on_SuppPfl_clicked()
{
    QMessageBox msgBox;

    msgBox.setWindowTitle("ATTENTION");
    msgBox.setText("Cette action supprimera définitivement le profil de la base de données");//boîte de dialogue pour avertir l'utilisateur que l'importation effacera toutes les données
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Ok)//l'utilisateur dit ok
    {
         QString nom;
        if(ui->comboBox_loadDataPfl->currentIndex()!=0)
        {
          nom=ui->comboBox_loadDataPfl->currentText();
        }
        if(nom.isEmpty())
        {
            QMessageBox::information(this,tr("Selection"),tr("Aucun profil sélectionné"));
            return;
        }

        DbConnection connexion;
        if(!connexion.open())
        {
            // gérer les erreurs de connexion ici
        }
        else
        {
            QSqlQuery query;
            query.prepare("DELETE FROM profil WHERE lower(nom) = lower(:nom)");
            query.bindValue(":nom", nom);

            // Exécuter la requête
            if (query.exec())
            {
                QMessageBox::information(this, "Succes", "Le profil a bien été supprimé.");
                QList<QComboBox*> comboBoxList = {ui->comboBox_PflAddSuppCpt, ui->comboBox_loadDataPfl, ui->comboBox_majInfoPfl};
                for (QComboBox* comboBox : comboBoxList)
                {
                    comboBox->removeItem(ui->comboBox_majInfoPfl->findText(nom));
                    sortComboBox(comboBox);
                }

            }
            else
            {
                 QMessageBox::critical(this, "Erreur", "Une erreur s'est produite!");
            }
            connexion.close();
        }

    }

    else if (ret == QMessageBox::Cancel)
    {
       msgBox.close();
    }
}


void Profil::on_majInfoPfl_clicked()
{
    DbConnection connexion;
    if(!connexion.open())
    {
        qDebug() << "Echec d'ouverture de la Base de données";
        QMessageBox::critical(this,tr("Erreur de données"), tr("Impossible de d'accéder à la base de données"));
    }
    else
    {
        if(ui->comboBox_majInfoPfl->currentIndex()==0)
        {
            QMessageBox::critical(this,tr("Attention"),tr("Aucun profil n'a été effectué"));
                    return;
        }
        QString ancienpfl = ui->comboBox_majInfoPfl->currentText();
        if(ancienpfl.isEmpty())
        {
            QMessageBox::critical(this,tr("Erreur de sélection"), tr("Aucune profil n'a été sélectionné!"));
        }
        else
        {
           QString nouvNom= ui->NouvNomMajInfoPfl->text().trimmed();
           QString nouvDesc= ui->NouvDescMajInfoPfl->text().trimmed();

           if(nouvNom.isEmpty() && nouvDesc.isEmpty())
           {
                QMessageBox::critical(this,tr("Erreur de sélection"), tr("Vous devez au moins remplir l'un des deux champs!"));
           }
           else
           {
               if(!nouvNom.isEmpty())
               {
                   QSqlQuery qry;
                   qry.prepare("SELECT COUNT(*) FROM profil WHERE lower(nom) = lower(:name)");
                   qry.bindValue(":name", nouvNom);

                   if (qry.exec())
                   {
                       qry.first();
                       int count = qry.value(0).toInt();

                       if (count > 0)
                       {
                           QMessageBox::information(this, tr("Mise à jour  Impossible"), tr("Un profil de ce nom existe déjà!"));
                           return;
                       }
                   }
               }


               QSqlQuery query;
               if(!nouvDesc.isEmpty())
               {
                    query.prepare("UPDATE profil SET description='"+nouvDesc+"' WHERE lower(nom)=lower('"+ancienpfl+"')");
                    if(query.exec())
                    {
                        QMessageBox::information(this,tr("Succès"), tr("Mise à jour de la description réussie!"));
                        ui->NouvDescMajInfoPfl->setText("");

                    }
                    else
                    {
                        QMessageBox::critical(this,tr("Erreur innatendue"), tr("Une erreure innatendue empeche la mise à jour de la description! Veuillez réessayer."));
                    }
               }
                if(!nouvNom.isEmpty())
               {
                   query.prepare("UPDATE profil SET nom='"+nouvNom+"' WHERE lower(nom)=lower('"+ancienpfl+"')");
                   if(query.exec())
                   {
                       QMessageBox::information(this,tr("Succès"), tr("Mise à jour du nom réussie!"));
                       ui->NouvNomMajInfoPfl->setText("");

                       QList<QComboBox*> comboBoxList = {ui->comboBox_PflAddSuppCpt, ui->comboBox_loadDataPfl, ui->comboBox_majInfoPfl};
                       for (QComboBox* comboBox : comboBoxList)
                       {
                           comboBox->removeItem(ui->comboBox_majInfoPfl->findText(ancienpfl));
                           comboBox->addItem(nouvNom);
                           sortComboBox(comboBox);
                       }

                   }
                   else
                   {
                       QMessageBox::critical(this,tr("Erreur innatendue"), tr("Une erreure innatendue empêche la mise à jour du nom! Veuillez réessayer."));
                   }
               }


           }

        }

    }

        connexion.close();
}


void Profil::on_AddCpt_clicked()
{
    QString cpt,nom; QStringList pflCpt;
    int nbrCpt=0;
    cpt = ui ->cptToAdd->text();
    nom = ui -> comboBox_PflAddSuppCpt->currentText();

    if(cpt.isEmpty() || nom.isEmpty())
    {
        ui -> statut -> setText("Le nom du profil et le nom de la compétence doivent être renseignés! ");//affichage d'un message d'erreur dans l'interface graphique
        QMessageBox::critical(this,"Remplissage","Le nom du profil et le nom de la compétence doivent être renseignés!");//affichage d'une boîte de dialogue d'erreur
        return;//sortie de la méthode
    }

    DbConnection connexion;
    if(!connexion.open())
    {
         QMessageBox::warning(this, "Erreur d'exécution", "Une erreur s'est produite.");
        return;
    }
    else
    {
        QSqlQuery query;
        query.prepare("SELECT ListCpt FROM profil WHERE nom = :nom");
        query.bindValue(":nom", nom);

        // Exécuter la requête
        if (!query.exec())
        {
            QMessageBox::warning(this, "Erreur d'exécution", "Une erreur s'est produite.");
            connexion.close();
            return;
        }

        // Récupérer les valeurs de la colonne "ListCpt"
        while (query.next())
        {
             pflCpt = query.value("ListCpt").toString().split(",");
        }
        if(pflCpt.contains(cpt,Qt::CaseInsensitive))
        {
            QMessageBox::critical(this,tr("Echec"),tr("Le Profil existe déjà!"));
            connexion.close();
            return;
        }
        pflCpt.append(cpt);
        pflCpt.sort(Qt::CaseInsensitive);
        QString list = pflCpt.join(",");


        query.prepare("SELECT nbrCpt FROM profil WHERE nom = :nom");
        query.bindValue(":nom", nom);
        if(query.exec())
        {
            while (query.next())
            {
                nbrCpt = query.value(0).toInt();
            }
        }

        nbrCpt+=1;


        QSqlQuery qry;
        qry.prepare("UPDATE  profil SET ListCpt = :list ,nbrCpt = :nbrCpt WHERE nom = :nom; ");
        qry.bindValue(":list", list);
        qry.bindValue(":nbrCpt", nbrCpt);
        qry.bindValue(":nom", nom);
        if(qry.exec())
        {
            QMessageBox::information(this,tr("Succes"),tr("La compétence a bien été ajoutée!"));
            chargerCptPfl(nom);
            ui->cptToAdd->setText("");


        }
        else
        {
            QMessageBox::information(this,tr("Erreur"),tr("Une erreur s'est produite!"));
        }




        qry.prepare("SELECT COUNT(*) FROM competences WHERE lower(nom) = lower(:name)");
        qry.bindValue(":name", cpt);

        if (qry.exec())
        {
            qry.first();
            int count = qry.value(0).toInt();

            if (count == 0)
            {
                QSqlQuery query;
                query.prepare("INSERT INTO competences (nom) VALUES (:name)");
                query.bindValue(":name", cpt);
                query.exec();
            }

        }

        connexion.close();
    }

}


void Profil::on_SupCpt_clicked()
{
    //récupéré le nom de la compétence
    QString cpt = ui->comboBox_CptToSup->currentText();
    if(cpt.isEmpty())
    {
        QMessageBox::information(this,tr("Pas de Selection"), tr("Aucune compétence n'a été choisie"));
    }
    else
    {

        QMessageBox msgBox;

          msgBox.setWindowTitle("ATTENTION");
          msgBox.setText("Cette action supprimera définitivement la compétence de la base de donnée.Voulez-vous continuer?");//boîte de dialogue pour avertir l'utilisateur que l'importation effacera toutes les données
          msgBox.setIcon(QMessageBox::Critical);
          msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
          int ret = msgBox.exec();

          if (ret == QMessageBox::Yes)//l'utilisateur dit ok
          {
              int nbrCpt=0;
              QStringList pflCpt;
              // Récupérer l'identité du professionnel
              QString nomPfl = ui->comboBox_PflAddSuppCpt->currentText(); // Préparer la requête SQL

              DbConnection connexion;
              if(!connexion.open())
              {
                   QMessageBox::warning(this, "Erreur d'exécution", "Une erreur s'est produite au niveau de la BD.Veuillez réessayer!");
                   return;
              }
              else
              {
                  QSqlQuery query;
                  query.prepare("SELECT ListCpt FROM profil WHERE nom = :nom");
                  query.bindValue(":nom", nomPfl);

                  // Exécuter la requête
                  if (!query.exec())
                  {
                      QMessageBox::warning(this, "Erreur d'exécution", "Une erreur s'est produite lors d l'exécution de la requete.Veuillez réeesayez!");
                      connexion.close();
                      return;
                  }

                  // Récupérer les valeurs de la colonne "ListCpt"
                  while (query.next())
                  {
                       pflCpt = query.value("ListCpt").toString().split(",");
                  }
                  pflCpt.removeOne(cpt);
                  pflCpt.sort(Qt::CaseInsensitive);
                  QString list = pflCpt.join(",");



                  query.prepare("SELECT nbrCpt FROM profil WHERE nom = :nom");
                  query.bindValue(":nom", nomPfl);
                  if(query.exec())
                  {
                      while (query.next())
                      {
                          nbrCpt = query.value(0).toInt();
                      }
                  }

                  nbrCpt-=1;


                  QSqlQuery qry;
                  qry.prepare("UPDATE  profil SET ListCpt = :list, nbrCpt= :nbrCpt WHERE nom = :nom");
                  qry.bindValue(":list", list);
                  qry.bindValue(":nbrCpt", nbrCpt);
                  qry.bindValue(":nom", nomPfl);
                  if(qry.exec())
                  {
                      QMessageBox::information(this,tr("Succes"),tr("La compétence a bien été supprimée!"));
                      chargerCptPfl(nomPfl);
                  }
                  else
                  {
                      QMessageBox::information(this,tr("Erreur"),tr("Une erreur s'est produite!"));
                  }
                  connexion.close();
              }

          }
          else if (ret == QMessageBox::Cancel)
          {
             msgBox.close();
          }
    }

}


void Profil::on_consulterInfoPfl_clicked()
{
    QString selectedProfil,competencePfl,competencePro; // nom du profil sélectionné
    QStringList professionalSkills,profilSkills,professionalName; // liste des compétences du profil
    QStringList matchingProfessional,profilDescription; // liste des professionnels correspondants
    std::vector<QString> profilCompetences;
    std::vector<QString> professionnelCompetences;


    if(ui->comboBox_loadDataPfl->currentIndex()!=0)
    {
        selectedProfil=ui->comboBox_loadDataPfl->currentText();
    }

    if(selectedProfil.isEmpty())
    {
        QMessageBox::information(this, tr("Erreur"),tr("Aucun profil sélectionné"));
        return;
    }

    DbConnection connexion;
    if(!connexion.open())
    {
        // Gérer les erreurs de connexion ici
        return;
    }

    QSqlQuery query;
    // Récupération des compétences du profil
    query.prepare("SELECT ListCpt FROM profil WHERE lower(nom) = lower(:selectedProfil)");
    query.bindValue(":selectedProfil", selectedProfil);
    if (query.exec())
    {
        while (query.next())
        {
            profilSkills=query.value(0).toString().split(",");
            for (int i = 0; i < profilSkills.size(); i++)
            {
                competencePfl = profilSkills[i];
                profilCompetences.push_back(competencePfl);

            }

        }

    }

    query.prepare("SELECT identite FROM professionnel");

    if (query.exec())
    {
        while (query.next())
        {
            professionalName.append(query.value(0).toString());
        }
    }

    // Récupération des profils correspondants
    for(int i = 0; i<professionalName.size();i++)
    {
        query.prepare("SELECT ListCpt FROM professionnel WHERE lower(identite) = lower(:identite)");
        query.bindValue(":identite", professionalName[i]);
        if (query.exec())
        {
            while (query.next())
            {
                professionnelCompetences.clear();
                professionalSkills.clear();

                professionalSkills=query.value(0).toString().split(",");
                for (int i = 0; i < professionalSkills.size(); i++)
                {
                    competencePro = professionalSkills[i];
                    professionnelCompetences.push_back(competencePro);
                }




              bool match = isSubset(profilCompetences, professionnelCompetences);//comparaison des cpt de chaque professionnel à ceux du profil
                if (match)
                {

                     matchingProfessional.append(professionalName[i]);
                }

            }


        }
    }


    // Affichage des résultats

    if(ui->tableView_client != nullptr)
    {
         QAbstractItemModel *modl = ui->tableView_client->model();
         if(modl!=nullptr)
         {
             modl->removeRows(0, modl->rowCount());
             modl->removeColumns(0,modl->columnCount());
         }

    }

    if (matchingProfessional.isEmpty())
    {
        QMessageBox::information(this, tr("Aucun professionnel correspondant"), tr("Aucun professionnel ne correspond au profil de ") + selectedProfil);
    }




    query.prepare("SELECT description FROM profil WHERE lower(nom) = lower(:selectedProfil)");
    query.bindValue(":selectedProfil", selectedProfil);
    if (query.exec())
    {


        while (query.next())
        {
            profilDescription.append(query.value(0).toString());
        }
    }



        QStandardItemModel *model = new QStandardItemModel();
         matchingProfessional.sort();
         professionalSkills.sort();
        for(int i = 0; i < matchingProfessional.size(); i++)
        {
            model->setItem(i, 2, new QStandardItem(matchingProfessional[i]));
        }
        for(int i = 0; i < profilSkills.size(); i++)
        {
            model->setItem(i, 0, new QStandardItem(profilSkills[i]));
        }
        for(int i = 0; i < profilDescription.size(); i++)
        {
            model->setItem(i, 1, new QStandardItem(profilDescription[i]));
        }


        ui -> tableView_client -> setModel(model);

        QString t = QString::number(matchingProfessional.size());


        QString statut =t+" professionnel(s) correspondant au profil  "+ selectedProfil;

        ui->tableView_client->setColumnWidth(0, 200);
        ui->tableView_client->setColumnWidth(1, 400);
        ui->tableView_client->setColumnWidth(2, 300);
        ui->tableView_client->model()->setHeaderData(0, Qt::Horizontal, "Compétences \n du profil \n"+selectedProfil);
         ui->tableView_client->model()->setHeaderData(1, Qt::Horizontal, "Description");
        ui->tableView_client->model()->setHeaderData(2, Qt::Horizontal, statut);
        QHeaderView* header = ui->tableView_client->horizontalHeader();
        header->setDefaultAlignment(Qt::AlignHCenter);
        ui -> tableView_client->setEditTriggers(QAbstractItemView::NoEditTriggers);

        ui -> statut -> setText(statut);


    connexion.close();
}



bool Profil::isSubset(const std::vector<QString>&profilCompetences, const std::vector<QString>&professionnelCompetences)
{
   std::vector<QString> sortedProfilCompetences = profilCompetences;
    std::vector<QString> sortedProfessionnelCompetences = professionnelCompetences;

    std::sort(sortedProfilCompetences.begin(), sortedProfilCompetences.end());
    std::sort(sortedProfessionnelCompetences.begin(), sortedProfessionnelCompetences.end());

    return std::includes(sortedProfessionnelCompetences.begin(), sortedProfessionnelCompetences.end(),
                        sortedProfilCompetences.begin(), sortedProfilCompetences.end());



}

