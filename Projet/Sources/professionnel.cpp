#include "professionnel.h"
#include "ui_professionnel.h"

Professionnel::Professionnel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Professionnel)
{
    ui->setupUi(this);
     setWindowTitle("Professionnel");

    updateProLists();

    connect(ui->comboBox_ProAddSuppCpt, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxSelectionChanged(int)));
}

Professionnel::~Professionnel()
{
    delete ui;
}

void Professionnel::updateProLists()
{
    DbConnection connexion;

    if (!connexion.open()) {
        qDebug() << "Echec d'ouverture de la Base de données";
        QMessageBox::critical(this, tr("Erreur de données"), tr("Impossible d'accéder à la base de données"));
        return;
    }


    // Vérifier si les différents QComboBox ont bien été créés

    ui->comboBox_ProAddSuppCpt->clear();
    ui->comboBox_ProAddSuppCpt->insertItem(0, "Sélectionnez une option");
    ui->comboBox_ProAddSuppCpt->setEditable(false);


    ui->comboBox_loadDataPro->clear();
    ui->comboBox_loadDataPro->insertItem(0, "Sélectionnez une option");
    ui->comboBox_loadDataPro->setEditable(false);


    ui->comboBox_majInfoPro->clear();
    ui->comboBox_majInfoPro->insertItem(0, "Sélectionnez une option");
    ui->comboBox_majInfoPro->setEditable(false);


    ui->comboBox_CptToSup->setEditable(false);



    // Ajouter les professionnels à la liste déroulante
    QSqlQuery query;
    query.exec("SELECT identite FROM professionnel ORDER BY identite");
    if (!query.isActive())
    {
        qDebug() << "Echec de récupération des professionnels";
        return;
    }

    while (query.next())
    {
        ui->comboBox_ProAddSuppCpt->addItem(query.value("identite").toString());
         ui->comboBox_loadDataPro->addItem(query.value("identite").toString());
          ui->comboBox_majInfoPro->addItem(query.value("identite").toString());
    }

    connexion.close();

}


void Professionnel::sortComboBox(QComboBox *comboBox)
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


void Professionnel::chargerlesPro()
{
    DbConnection connexion;
    if(!connexion.open())
    {
        qDebug() << "Echec d'ouverture de la Base de données";
        QMessageBox::critical(this,tr("Erreur de données"), tr("Impossible de d'accéder à la base de données"));
    }
    else
    {
        QSqlQuery query;
        query.exec("SELECT identite FROM professionnel ORDER BY identite");

        QSqlQueryModel *model = new QSqlQueryModel;
        model->setQuery(query);
        ui -> tableView_client ->setModel(model);

        QString statut="liste des professionnels existant ";


         ui->tableView_client->setColumnWidth(0, 400);
        //ui-> tableView_client->model()->setHeaderData(0, Qt::Horizontal, statut);
        QHeaderView* header = ui->tableView_client->horizontalHeader();
        header->setDefaultAlignment(Qt::AlignHCenter);
        ui -> tableView_client->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui -> statut -> setText(statut);
    }

        connexion.close();
}

void Professionnel::on_ListerPro_clicked()
{
   chargerlesPro();
}

void Professionnel::on_Retour_clicked()
{
    this -> hide();
    menu retour;
    retour.exec();
}

void Professionnel::chargerCptPro(QString identite)
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
        query.prepare("SELECT ListCpt FROM professionnel WHERE identite = :identite");
        query.bindValue(":identite", identite);

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

void Professionnel::onComboBoxSelectionChanged(int index)
{
     QString choix = ui->comboBox_ProAddSuppCpt->itemText(index);
     chargerCptPro(choix);

}

void Professionnel::on_SupCpt_clicked()
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
          msgBox.setText("Cette action supprimera définitivement la competence de la base de donnée.Voulez-vous continuer?");//boîte de dialogue pour avertir l'utilisateur que l'importation effacera toutes les données
          msgBox.setIcon(QMessageBox::Critical);
          msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
          int ret = msgBox.exec();

          if (ret == QMessageBox::Ok)//l'utilisateur dit ok
          {
              int nbrCpt=0;
              QStringList proCpt;
              // Récupérer l'identité du professionnel
              QString identite = ui->comboBox_ProAddSuppCpt->currentText(); // Préparer la requête SQL

              DbConnection connexion;
              if(!connexion.open())
              {
                   QMessageBox::warning(this, "Erreur d'exécution", "Une erreur s'est produite.");
                   return;
              }
              else
              {
                  QSqlQuery query;
                  query.prepare("SELECT ListCpt FROM professionnel WHERE identite = :identite");
                  query.bindValue(":identite", identite);

                  // Exécuter la requête
                  if (!query.exec())
                  {
                      QMessageBox::warning(this, "Erreur d'exécution", "Impossible d'exécuter la requête SQL.");
                      connexion.close();
                      return;
                  }

                  // Récupérer les valeurs de la colonne "ListCpt"
                  while (query.next())
                  {
                       proCpt = query.value("ListCpt").toString().split(",");
                  }
                  proCpt.removeOne(cpt);
                  proCpt.sort(Qt::CaseInsensitive);
                  QString list = proCpt.join(",");



                  query.prepare("SELECT nbrCpt FROM professionnel WHERE identite = :identite");
                  query.bindValue(":identite", identite);
                  if(query.exec())
                  {
                      while (query.next())
                      {
                          nbrCpt = query.value(0).toInt();
                      }
                  }

                  nbrCpt-=1;


                  QSqlQuery qry;
                  qry.prepare("UPDATE  professionnel SET ListCpt = :list, nbrCpt= :nbrCpt WHERE identite = :identite");
                  qry.bindValue(":list", list);
                  qry.bindValue(":nbrCpt", nbrCpt);
                  qry.bindValue(":identite", identite);
                  if(qry.exec())
                  {
                      QMessageBox::information(this,tr("Succes"),tr("La compétence a bien été supprimée!"));
                      chargerCptPro(identite);
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


void Professionnel::on_AddCpt_clicked()
{
    QString cpt,identite; QStringList proCpt;
    int nbrCpt=0;
    cpt = ui ->cptToAdd->text();
    identite = ui -> comboBox_ProAddSuppCpt->currentText();

    if(cpt.isEmpty() || identite.isEmpty())  //vérification si les champs nom d'utilisateur et mot de passe sont vides
    {
        ui -> statut -> setText("Le nom du professionnel et le nom de la compétence doivent être renseignés! ");//affichage d'un message d'erreur dans l'interface graphique
        QMessageBox::critical(this,"Remplissage","Le nom du professionnel et le nom de la compétence doivent être renseignés!");//affichage d'une boîte de dialogue d'erreur
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
        query.prepare("SELECT ListCpt FROM professionnel WHERE identite = :identite");
        query.bindValue(":identite", identite);

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
             proCpt = query.value("ListCpt").toString().split(",");
        }
        if(proCpt.contains(cpt,Qt::CaseInsensitive))
        {
            QMessageBox::critical(this,tr("Echec"),tr("La compétence existe déjà!"));
            connexion.close();
            return;
        }
        proCpt.append(cpt);
        proCpt.sort(Qt::CaseInsensitive);
        QString list = proCpt.join(",");


        query.prepare("SELECT nbrCpt FROM professionnel WHERE identite = :identite");
        query.bindValue(":identite", identite);
        if(query.exec())
        {
            while (query.next())
            {
                nbrCpt = query.value(0).toInt();
            }
        }

        nbrCpt+=1;


        QSqlQuery qry;
        qry.prepare("UPDATE  professionnel SET ListCpt = :list ,nbrCpt = :nbrCpt WHERE identite = :identite; ");
        qry.bindValue(":list", list);
        qry.bindValue(":nbrCpt", nbrCpt);
        qry.bindValue(":identite", identite);
        if(qry.exec())
        {
            QMessageBox::information(this,tr("Succes"),tr("La compétence a bien été ajoutée!"));
            chargerCptPro(identite);
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


void Professionnel::on_addNewPro_clicked()
{
    QString nomPro,prenomPro,identite;
    QStringList proList;
    nomPro=ui->nomNouvPro->text();
    prenomPro=ui->PrenomNouvPro->text();
    identite=nomPro.trimmed()+" "+prenomPro.trimmed();

    if(nomPro.isEmpty() || prenomPro.isEmpty())  //vérification si les champs nom d'utilisateur et mot de passe sont vides
    {
        ui -> statut -> setText("Le nom et le prénom du professionnel doivent être renseignés! ");//affichage d'un message d'erreur dans l'interface graphique
        QMessageBox::critical(this,"Remplissage","Le nom du professionnel et le prénom du professionnel doivent être renseignés!");//affichage d'une boîte de dialogue d'erreur
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
        query.prepare("SELECT identite FROM professionnel WHERE lower(identite) = lower(:identite)");
        query.bindValue(":identite", identite);

        // Exécuter la requête
        if (!query.exec())
        {
            QMessageBox::warning(this, "Erreur d'exécution", "Une erreur s'est produite.");
            connexion.close();
            return;
        }


        while (query.next())
        {
             proList = query.value("identite").toString().split(",");
        }
        if(proList.contains(identite,Qt::CaseInsensitive))
        {
            QMessageBox::critical(this,tr("Echec"),tr("Le professionnel existe déjà!"));
            connexion.close();
            return;
        }

        QSqlQuery qry;
        qry.prepare("INSERT INTO   professionnel(nom,prenom,identite) VALUES(:nomPro, :prenomPro, :identitePro) ");
        qry.bindValue(":nomPro", nomPro);
        qry.bindValue(":prenomPro", prenomPro);
        qry.bindValue(":identitePro", identite);
        if(qry.exec())
        {
            QMessageBox::information(this,tr("Succes"),tr("Le professionnel a bien été ajouté!"));
            ui->nomNouvPro->setText("");
            ui->PrenomNouvPro->setText("");
            QList<QComboBox*> comboBoxList = {ui->comboBox_ProAddSuppCpt, ui->comboBox_loadDataPro, ui->comboBox_majInfoPro};
            for (QComboBox* comboBox : comboBoxList)
            {
                comboBox->addItem(identite);
                sortComboBox(comboBox);
            }
        }
        else
        {
            QMessageBox::information(this,tr("Erreur"),tr("Une erreur s'est produite!"));
        }
        connexion.close();

    }


}


void Professionnel::on_SuppPro_clicked()
{
    QMessageBox msgBox;

    msgBox.setWindowTitle("ATTENTION");
    msgBox.setText("Cette action supprimera définitivement le professionnel de la base de données");//boîte de dialogue pour avertir l'utilisateur que l'importation effacera toutes les données
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Ok)//l'utilisateur dit ok
    {
         QString identite;
        if(ui->comboBox_loadDataPro->currentIndex()!=0)
        {
          identite=ui->comboBox_loadDataPro->currentText();
        }
        if(identite.isEmpty())
        {
            QMessageBox::information(this,tr("Selection"),tr("Aucun professionnel sélectionné"));
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
            query.prepare("DELETE FROM professionnel WHERE lower(identite) = lower(:identite)");
            query.bindValue(":identite", identite);

            // Exécuter la requête
            if (query.exec())
            {
                QMessageBox::information(this, "Succes", "Le professionnel a bien été supprimé.");
                QList<QComboBox*> comboBoxList = {ui->comboBox_ProAddSuppCpt, ui->comboBox_loadDataPro, ui->comboBox_majInfoPro};
                for (QComboBox* comboBox : comboBoxList)
                {
                    comboBox->removeItem(ui->comboBox_majInfoPro->findText(identite));
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

bool Professionnel::isSubset(const std::vector<QString>&profilCompetences, const std::vector<QString>&professionnelCompetences)
{
   std::vector<QString> sortedProfilCompetences = profilCompetences;
    std::vector<QString> sortedProfessionnelCompetences = professionnelCompetences;

    std::sort(sortedProfilCompetences.begin(), sortedProfilCompetences.end());
    std::sort(sortedProfessionnelCompetences.begin(), sortedProfessionnelCompetences.end());

    return std::includes(sortedProfessionnelCompetences.begin(), sortedProfessionnelCompetences.end(),
                        sortedProfilCompetences.begin(), sortedProfilCompetences.end());

  /*  std::vector<QString> sortedProfilCompetences = profilCompetences;
        std::vector<QString> sortedProfessionnelCompetences = professionnelCompetences;

        std::sort(sortedProfilCompetences.begin(), sortedProfilCompetences.end(), [](const QString &a, const QString &b)
        {
            return a.compare(b, Qt::CaseInsensitive) < 0;
        });
        std::sort(sortedProfessionnelCompetences.begin(), sortedProfessionnelCompetences.end(), [](const QString &a, const QString &b)
        {
            return a.compare(b, Qt::CaseInsensitive) < 0;
        });

        return std::includes(sortedProfessionnelCompetences.begin(), sortedProfessionnelCompetences.end(),
                            sortedProfilCompetences.begin(), sortedProfilCompetences.end(),
                            [](const QString &a, const QString &b) {
                                return a.compare(b, Qt::CaseInsensitive) == 0;
                            });
*/


}



 void Professionnel::on_consulterInfoPro_clicked()
    {
        QString selectedProfessional,competencePro,competencePfl; // nom du professionnel sélectionné
        QStringList professionalSkills,profilSkills,profilName; // liste des compétences du professionnel
        QStringList matchingProfiles; // liste des profils correspondants
        std::vector<QString> profilCompetences;
        std::vector<QString> professionnelCompetences;


        if(ui->comboBox_loadDataPro->currentIndex()!=0)
        {
            selectedProfessional=ui->comboBox_loadDataPro->currentText();
        }

        if(selectedProfessional.isEmpty())
        {
            QMessageBox::information(this, tr("Erreur"),tr("Aucun professionnel sélectionné"));
            return;
        }

        DbConnection connexion;
        if(!connexion.open())
        {
            // Gérer les erreurs de connexion ici
            return;
        }

        QSqlQuery query;
        // Récupération des compétences du professionnel
        query.prepare("SELECT ListCpt FROM professionnel WHERE lower(identite) = lower(:nom)");
        query.bindValue(":nom", selectedProfessional);
        if (query.exec())
        {
            while (query.next())
            {
                professionalSkills=query.value(0).toString().split(",");
                for (int i = 0; i < professionalSkills.size(); i++)
                {
                    competencePro = professionalSkills[i];
                    professionnelCompetences.push_back(competencePro);

                }

            }

        }

        query.prepare("SELECT nom FROM profil");
        //query.bindValue(":competences", professionalSkills.join(","));
        if (query.exec())
        {
            while (query.next())
            {
                profilName.append(query.value(0).toString());
            }
        }

        // Récupération des profils correspondants
        for(int i = 0; i<profilName.size();i++)
        {
            query.prepare("SELECT ListCpt FROM profil WHERE lower(nom) = lower(:nom)");
            query.bindValue(":nom", profilName[i]);
            if (query.exec())
            {
                while (query.next())
                {
                    profilCompetences.clear();
                    profilSkills.clear();

                    profilSkills=query.value(0).toString().split(",");
                    for (int i = 0; i < profilSkills.size(); i++)
                    {
                        competencePfl = profilSkills[i];
                        profilCompetences.push_back(competencePfl);
                    }




                  bool match = isSubset(profilCompetences, professionnelCompetences);
                    if (match)
                    {

                         matchingProfiles.append(profilName[i]);
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

        if (matchingProfiles.isEmpty())
        {
            QMessageBox::information(this, tr("Aucun profil correspondant"), tr("Aucun profil ne correspond aux compétences de ") + selectedProfessional);
        }





            QStandardItemModel *model = new QStandardItemModel();
             matchingProfiles.sort();
             professionalSkills.sort();
            for(int i = 0; i < matchingProfiles.size(); i++)
            {
                model->setItem(i, 1, new QStandardItem(matchingProfiles[i]));
            }
            for(int i = 0; i < professionalSkills.size(); i++)
            {
                model->setItem(i, 0, new QStandardItem(professionalSkills[i]));
            }





            ui -> tableView_client -> setModel(model);

            QString t = QString::number(matchingProfiles.size());


            QString statut =t+" profils correspondant à "+ selectedProfessional;

            ui->tableView_client->setColumnWidth(0, 200);
             ui->tableView_client->setColumnWidth(1, 400);
            ui->tableView_client->model()->setHeaderData(0, Qt::Horizontal, "Compétences");
            ui->tableView_client->model()->setHeaderData(1, Qt::Horizontal, statut);
            QHeaderView* header = ui->tableView_client->horizontalHeader();
            header->setDefaultAlignment(Qt::AlignHCenter);
            ui -> tableView_client->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui -> statut -> setText(statut);


        connexion.close();
    }



void Professionnel::on_majInfoPro_clicked()
{
    DbConnection connexion;
    if(!connexion.open())
    {
        qDebug() << "Echec d'ouverture de la Base de données";
        QMessageBox::critical(this,tr("Erreur de données"), tr("Impossible de d'accéder à la base de données"));
    }
    else
    {
        if(ui->comboBox_majInfoPro->currentIndex()==0)
        {
            QMessageBox::critical(this,tr("Attention"),tr("Aucun choix n'a été effectué"));
                    return;
        }
        QString ancienpro = ui->comboBox_majInfoPro->currentText();
        if(ancienpro.isEmpty())
        {
            QMessageBox::critical(this,tr("Erreur de sélection"), tr("Aucune professionnel n'a été sélectionné!"));
        }
        else
        {
           QString nouvNom= ui->NouvNomMajInfoPro->text();
           QString nouvPre= ui->NouvPrenomMajInfoPro->text();
           QString identite= nouvNom.trimmed() + " " + nouvPre.trimmed();

           if(nouvNom.isEmpty() && nouvPre.isEmpty())
           {
                QMessageBox::critical(this,tr("Erreur de saisie"), tr("Au moins un des champs nouveau nom ou nouveau prénom doit être rempli!"));
           }
           else
           {
               QSqlQuery qry;
               if(!nouvNom.isEmpty())
               {

                   qry.prepare("SELECT COUNT(*) FROM professionnel WHERE lower(identite) = lower(:name)");
                   qry.bindValue(":name", identite);

                   if (qry.exec())
                   {
                       qry.first();
                       int count = qry.value(0).toInt();

                       if (count > 0)
                       {
                           QMessageBox::information(this, tr("Ajout Impossible"), tr("Un professionnel de cette identité existe déjà!"));
                           return;
                       }
                   }

                   qry.prepare("UPDATE professionnel, SET  nom='"+nouvNom+"'  WHERE lower(identite)=lower('"+ancienpro+"'); ");
                   if(qry.exec())
                   {
                       QMessageBox::information(this,tr("Succès"), tr("Mise à jour du nom réussie!"));
                       ui->NouvNomMajInfoPro->setText("");




                   }
                   else
                   {
                       QMessageBox::critical(this,tr("Erreur innatendue"), tr("Une erreure innatendue nous empêche de mettre à jour le nom! Veuillez réessayer."));
                   }
               }
               if(!nouvPre.isEmpty())
               {
                    qry.prepare("UPDATE professionnel, SET  prenom='"+nouvPre+"'  WHERE lower(identite)=lower('"+ancienpro+"'); ");
                   if(qry.exec())
                   {
                       QMessageBox::information(this,tr("Succès"), tr("Mise à jour du prénom réussie!"));
                       ui->NouvPrenomMajInfoPro->setText("");

                   }
                   else
                   {
                       QMessageBox::critical(this,tr("Erreur innatendue"), tr("Une erreure innatendue nous empêche de mettre à jour le prénom! Veuillez réessayer."));
                   }
               }

               QList<QComboBox*> comboBoxList = {ui->comboBox_ProAddSuppCpt, ui->comboBox_loadDataPro, ui->comboBox_majInfoPro};
               for (QComboBox* comboBox : comboBoxList)
               {
                   comboBox->removeItem(ui->comboBox_majInfoPro->findText(ancienpro));
                   comboBox->addItem(identite);
                  sortComboBox(comboBox);
               }

           }

        }

    }

        connexion.close();
}

