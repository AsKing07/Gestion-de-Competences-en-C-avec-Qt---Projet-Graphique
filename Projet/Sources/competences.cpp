#include "competences.h"
#include "ui_competences.h"

Competences::Competences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Competences)
{
    ui->setupUi(this);
     setWindowTitle("Compétences");


    DbConnection connexion;// Initialisation de la connexion à la base de données

    if(!connexion.open())// Si la connexion à la base de données échoue
    {
        qDebug() << "Echec d'ouverture de la Base de données";
        QMessageBox::critical(this,tr("Erreur de données"), tr("Impossible de d'accéder à la base de données"));
        ui->comboBox_maj->insertItem(0, "Sélectionnez une option");// Insère un item avec le texte "Sélectionnez une option" dans la liste déroulante "ui->comboBox_maj"
        ui->comboBox_sup->insertItem(0, "Sélectionnez une option");// Insère un item avec le texte "Sélectionnez une option" dans la liste déroulante "ui->comboBox_sup"
    }
    else //Si la connexion à la base réussie
    {
        ui->comboBox_maj->insertItem(0, "Sélectionnez une option");// Insère un item avec le texte "Sélectionnez une option" dans la liste déroulante "ui->comboBox_maj"
        ui->comboBox_sup->insertItem(0, "Sélectionnez une option");// Insère un item avec le texte "Sélectionnez une option" dans la liste déroulante "ui->comboBox_sup"


        ui->comboBox_maj->setEditable(false);
        ui->comboBox_sup->setEditable(false);
        //Ajout des différentes compétences à la liste déroulante
        QSqlQuery query;
        query.exec("SELECT  nom FROM competences ORDER BY nom");//Requete pour récupérer les nom des competences
        while (query.next())//Pour chaque ligne renvoyée par la requete
        {
            //On ajoute le nom de chaque cpt aux listes déroulantes
          ui-> comboBox_maj->addItem(query.value("nom").toString());
          ui-> comboBox_sup->addItem(query.value("nom").toString());
        }
    }

}

Competences::~Competences()//Destructeur
{
    delete ui;
}

void Competences::on_Menu_clicked()//Slot du retour au menu
{
    this -> hide();
    menu retour;
    retour.exec();
}

void Competences::updateCompetenceLists()//Lorsque une modification au niveau des compétences est effectuée, cette fonction sert à mettre à jour les listes déroulantes
{
    DbConnection db;

    if (!db.open())
       {
           qDebug() << "Error: connection with database is not open";
           return;
       }

       QSqlQuery query;
       query.prepare("SELECT nom FROM competences ORDER BY nom");
       if (!query.exec())
       {
           qDebug() << "Error: failed to execute query - " << query.lastError();
           return;
       }

    //Ajout des différentes compétences à la liste déroulante apres avoir vidé leur ancien conteunu
    ui->comboBox_maj->clear();
    ui->comboBox_sup->clear();
    ui->comboBox_maj->insertItem(0, "Sélectionnez une option");
    ui->comboBox_sup->insertItem(0, "Sélectionnez une option");
    QSqlQuery qry;
    qry.exec("SELECT  nom FROM competences ORDER BY nom");
    while (qry.next())
    {
      ui-> comboBox_maj->addItem(qry.value("nom").toString());
      ui-> comboBox_sup->addItem(qry.value("nom").toString());
    }
}



void Competences::chargerlesCpt()// récupérer les nom des compétences
{
    DbConnection connexion;// créer une connexion à la base de données
    if(!connexion.open())// vérifier si la connexion est réussie
    {
        // afficher un message d'erreur en cas d'échec d'ouverture de la base de données
        qDebug() << "Echec d'ouverture de la Base de données";
        QMessageBox::critical(this,tr("Erreur de données"), tr("Impossible de d'accéder à la base de données"));
    }
    else
    {
        QSqlQuery query;
        query.exec("SELECT nom FROM competences ORDER BY nom");// exécuter la requête SQL pour récupérer les noms des compétences triées par nom

        QSqlQueryModel *model = new QSqlQueryModel;// créer un nouveau modèle de requête SQL
        model->setQuery(query);// définir la requête précédante pour le modèle
        ui -> tableView_client ->setModel(model);// définir le modèle pour le tableau de visualisation



        QString statut="liste des competences existantes ";// définir un message pour le statut


        ui->tableView_client->setColumnWidth(0, 390);// définir la largeur de la colonne 0 pour 390 pixels
        ui-> tableView_client->model()->setHeaderData(0, Qt::Horizontal, statut); // définir le nom de la colonne 0 pour le message de statut
        QHeaderView* header = ui->tableView_client->horizontalHeader(); // obtenir l'en-tête horizontal du tableau de visualisation
        header->setDefaultAlignment(Qt::AlignHCenter); // définir l'alignement par défaut pour l'en-tête horizontal sur le centre
        ui -> tableView_client->setEditTriggers(QAbstractItemView::NoEditTriggers); // empecher l'utilisateur de modifier le tableau
        ui -> statut -> setText(statut); // définir le message de statut

        connexion.close();//Fermer la connexion à la bdd

    }
}

void Competences::on_Lister_clicked()//Lorsquon clique sur le bouton lister les cpt
{
   chargerlesCpt();//On appelle la fonction ci
}


void Competences::on_supprimer_clicked()
{
      QMessageBox msgBox;

      msgBox.setWindowTitle("ATTENTION");
      msgBox.setText("Cette action supprimera définitivement la compétence de la base de données");//boîte de dialogue pour avertir l'utilisateur que l'importation effacera toutes les données
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
      int ret = msgBox.exec();

      if (ret == QMessageBox::Ok)//l'utilisateur dit ok
      {
        DbConnection connexion;
        if(!connexion.open())// échec d'ouverture de la connexion à la base de données
        {
            qDebug() << "Echec d'ouverture de la Base de données";
            QMessageBox::critical(this,tr("Erreur de données"), tr("Impossible de d'accéder à la base de données"));
        }
        else
        {

            QString selectedName ;

            if(ui->comboBox_sup->currentIndex()!=0)
            {
              selectedName = ui->comboBox_sup->currentText();// récupération du nom sélectionné pour suppression
            }



            if(selectedName.isEmpty())// pas de compétence sélectionnée
            {
                QMessageBox::critical(this,tr("Erreur de sélection"), tr("Aucune compétence n'a été sélectionnée!"));
            }
            else// une compétence a été sélectionnée
            {
                QSqlQuery query;
                query.prepare("DELETE FROM competences WHERE nom='"+selectedName+"'");//préparation de la requette de suppression
                if(query.exec())//Si la suppression réussie
                {
                    QMessageBox::information(this,tr("Succès"), tr("Suppression réussie!"));
                     updateCompetenceLists();// mise à jour de la liste de compétences dans les comboBox
                     chargerlesCpt(); // rechargement de la liste de compétences
                }
                else// échec d'exécution
                {
                    QMessageBox::critical(this,tr("Erreur innatendue"), tr("Une erreure innatendue s'est produite! Veuillez réessayer."));
                }
            }
        }

            connexion.close();//Fermetture de la connexion
      }

      else if (ret == QMessageBox::Cancel)// l'utilisateur a cliqué sur annuler
      {
         msgBox.close();
      }

}



void Competences::on_ajouter_clicked()
{
    DbConnection connexion;
    if(!connexion.open())
    {
        qDebug() << "Echec d'ouverture de la Base de données";
        QMessageBox::critical(this,tr("Erreur de données"), tr("Impossible de d'accéder à la base de données"));
    }
    else//la connexion à la bdd réussie
    {
        QString selectedName = ui->NouvCpt->text();//récupération du nom de la nouvelle cpt
        if(selectedName.isEmpty())//Aucun nom n'a été renseigné
        {
            QMessageBox::critical(this,tr("Erreur de sélection"), tr("Aucune compétence n'a été sélectionnée!"));//Message pr informer
        }
        else
        {



            QSqlQuery qry;
            qry.prepare("SELECT COUNT(*) FROM competences WHERE lower(nom) = lower(:name)");//preparation de la requette pour vérifier si la cpt existe déja
            qry.bindValue(":name", selectedName);

            if (qry.exec())//Si la requette a pu s'éxécuter
            {
                qry.first();
                int count = qry.value(0).toInt();

                if (count > 0)//Si la compétence existe déja
                {
                    QMessageBox::information(this, tr("Ajout Impossible"), tr("Cette compétence existe déjà!"));
                }
                else//Sinon
                {
                    QSqlQuery query;
                    query.prepare("INSERT INTO competences (nom) VALUES (:name)");//Requete pour ajouter la nouvelle cpt à la bdd
                    query.bindValue(":name", selectedName);
                    if (query.exec()) {
                        QMessageBox::information(this, tr("Succès"), tr("Ajout réussi!"));
                        ui->NouvCpt->setText("");
                        updateCompetenceLists();//Mise à jour de la liste des cpt
                        chargerlesCpt();
                    }
                    else
                    {
                        QMessageBox::critical(this, tr("Erreur innatendue"), tr("Une erreur innatendue s'est produite! Veuillez réessayer."));
                    }
                }
            }

          }
     }

        connexion.close();//Fermetture d la connexion
}


void Competences::on_maj_clicked()
{
    DbConnection connexion;
    if(!connexion.open())
    {
        qDebug() << "Echec d'ouverture de la Base de données";
        QMessageBox::critical(this,tr("Erreur de données"), tr("Impossible de d'accéder à la base de données"));
    }
    else
    {
        QString anciennecpt;

        if(ui->comboBox_sup->currentIndex()!=0)
        {
          anciennecpt = ui->comboBox_maj->currentText();// récupération du nom sélectionné pour maj
        }


        if(anciennecpt.isEmpty())
        {
            QMessageBox::critical(this,tr("Erreur de sélection"), tr("Aucune compétence n'a été sélectionnée!"));
        }
        else
        {
           QString nouvellecpt= ui->NouvNom->text();

           if(nouvellecpt.isEmpty())
           {
                QMessageBox::critical(this,tr("Erreur de saisie"), tr("Veuillez spécifier le nouveau nom de la compétence!"));
           }
           else
           {
               QSqlQuery qry;
               qry.prepare("SELECT COUNT(*) FROM competences WHERE nom = :name");
               qry.bindValue(":name", nouvellecpt);

               if (qry.exec())
               {
                   qry.first();
                   int count = qry.value(0).toInt();

                   if (count > 0)
                   {
                       QMessageBox::information(this, tr("Ajout Impossible"), tr("Une compétence de ce nom existe déjà!"));
                       return;
                   }
               }

                QSqlQuery query;
                query.prepare("UPDATE competences SET nom='"+nouvellecpt+"' WHERE nom='"+anciennecpt+"'");
                if(query.exec())
                {
                    QMessageBox::information(this,tr("Succès"), tr("Mise à jour réussie!"));
                    ui->NouvNom->setText("");
                    updateCompetenceLists();
                    chargerlesCpt();//actualiser la liste des competences dans le Qtable_View
                }
                else
                {
                    QMessageBox::critical(this,tr("Erreur innatendue"), tr("Une erreure innatendue s'est produite! Veuillez réessayer."));
                }
           }

        }

    }

        connexion.close();
}

