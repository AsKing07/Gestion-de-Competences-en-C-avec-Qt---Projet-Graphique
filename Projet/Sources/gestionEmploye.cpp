#include "gestionEmploye.h"
#include "ui_gestionEmploye.h"

gestionEmploye::gestionEmploye(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gestionEmploye)
{
    ui->setupUi(this);
    updateEmpLists();
     setWindowTitle("Employé");

}

gestionEmploye::~gestionEmploye()
{
    delete ui;
}



void gestionEmploye::updateEmpLists()
{
    DbConnection connexion;

    if (!connexion.open()) {
        qDebug() << "Echec d'ouverture de la Base de données";
        QMessageBox::critical(this, tr("Erreur de données"), tr("Impossible d'accéder à la base de données"));
        return;
    }


    ui->comboBox->clear();
    ui->comboBox->insertItem(0, "Sélectionnez une option");
    ui->comboBox->setEditable(false);





    // Ajouter les professionnels à la liste déroulante
    QSqlQuery query;
    query.exec("SELECT nom FROM admin ORDER BY nom");
    if (!query.isActive())
    {
        qDebug() << "Echec de récupération des employés";
        return;
    }

    while (query.next())
    {
        ui->comboBox->addItem(query.value("nom").toString());

    }

    connexion.close();

}

void gestionEmploye::on_menu_clicked()
{
    this -> hide();
    menu retour;
    retour.exec();
}


void gestionEmploye::chargerlesEmp()
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
        query.exec("SELECT nom,username FROM admin ORDER BY nom");

        QSqlQueryModel *model = new QSqlQueryModel;
        model->setQuery(query);
        ui -> tableView ->setModel(model);

        QString statut="liste des Employés ";


         ui->tableView->setColumnWidth(0, 400);

        QHeaderView* header = ui->tableView->horizontalHeader();
        header->setDefaultAlignment(Qt::AlignHCenter);
        ui -> tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }

        connexion.close();
}


void gestionEmploye::on_listerEmp_clicked()
{
     chargerlesEmp();
}


void gestionEmploye::on_AjouterEmploye_clicked()
{

    QString username,mdp,identite;
    QStringList empList,usernamelist;
    identite=ui->nomEmploye->text();
    username=ui->usernameEmploye->text();
    mdp=ui->mdpNewEmp->text();

    if(identite.isEmpty() || username.isEmpty() || mdp.isEmpty())  //vérification si les champs nom d'utilisateur et mot de passe sont vides
    {

        QMessageBox::critical(this,"Remplissage","Les trois champs doivent être renseignés!");//affichage d'une boîte de dialogue d'erreur
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
        query.prepare("SELECT nom FROM admin WHERE lower(nom) = lower(:identite)");
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
             empList = query.value("identite").toString().split(",");
        }
        if(empList.contains(identite,Qt::CaseInsensitive))
        {
            QMessageBox::critical(this,tr("Echec"),tr("Un employé de ce nom existe déjà existe déjà!"));
            connexion.close();
            return;
        }



        query.prepare("SELECT username FROM admin WHERE lower(username) = lower(:username)");
        query.bindValue(":username", username);

        // Exécuter la requête
        if (!query.exec())
        {
            QMessageBox::warning(this, "Erreur d'exécution", "Une erreur s'est produite.");
            connexion.close();
            return;
        }


        while (query.next())
        {
             usernamelist = query.value("username").toString().split(",");
        }
        if(usernamelist.contains(username,Qt::CaseInsensitive))
        {
            QMessageBox::critical(this,tr("Echec"),tr("Ce nom d'utilisateur est déjà pris!"));
            connexion.close();
            return;
        }


        QSqlQuery qry;
        qry.prepare("INSERT INTO   admin(nom,username,mdp) VALUES(:identite, :username, :mdp) ");
        qry.bindValue(":identite", identite);
        qry.bindValue(":username", username);
        qry.bindValue(":mdp", mdp);
        if(qry.exec())
        {
            QMessageBox::information(this,tr("Succes"),tr("L'employé a bien été ajouté!"));
            ui->nomEmploye->setText("");
            ui->mdpNewEmp->setText("");
            ui->usernameEmploye->setText("");

                ui->comboBox->addItem(identite);


        }
        else
        {
            QMessageBox::information(this,tr("Erreur"),tr("Une erreur s'est produite!"));
        }
        connexion.close();

    }

}




void gestionEmploye::on_Supprimer_clicked()
{
    QMessageBox msgBox;

    msgBox.setWindowTitle("ATTENTION");
    msgBox.setText("Cette action supprimera définitivement l'employé de la base de données");//boîte de dialogue pour avertir l'utilisateur que l'importation effacera toutes les données
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Ok)//l'utilisateur dit ok
    {
         QString identite;
        if(ui->comboBox->currentIndex()!=0)
        {
          identite=ui->comboBox->currentText();
        }
        if(identite.isEmpty())
        {
            QMessageBox::information(this,tr("Selection"),tr("Aucun employé sélectionné"));
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
            query.prepare("DELETE FROM admin WHERE lower(nom) = lower(:identite)");
            query.bindValue(":identite", identite);

            // Exécuter la requête
            if (query.exec())
            {
                QMessageBox::information(this, "Succes", "L'employé a bien été supprimé.");

                  ui->comboBox->removeItem(ui->comboBox->findText(identite));

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




void gestionEmploye::on_pushButton_clicked()
{
    DbConnection connexion;
    if(!connexion.open())
    {
        qDebug() << "Echec d'ouverture de la Base de données";
        QMessageBox::critical(this,tr("Erreur de données"), tr("Impossible de d'accéder à la base de données"));
    }
    else
    {
        if(ui->comboBox->currentIndex()==0)
        {
            QMessageBox::critical(this,tr("Attention"),tr("Aucun choix n'a été effectué"));
                    return;
        }
        QString ancienemp = ui->comboBox->currentText();
        if(ancienemp.isEmpty())
        {
            QMessageBox::critical(this,tr("Erreur de sélection"), tr("Aucun employé n'a été sélectionné!"));
        }
        else
        {
           QString nouvIdentite= ui->newIdentEmpMaj->text();
           QString nouvUsername= ui->newUsernameMaj->text();
           QString mdp= ui->newMdpEmpMaj->text();

           if(nouvIdentite.isEmpty() && nouvUsername.isEmpty() && mdp.isEmpty())
           {
                QMessageBox::critical(this,tr("Erreur de saisie"), tr("Au moins un des champs de mise à jour doit être rempli!"));
           }
           else
           {
               QSqlQuery qry;
               if(!nouvIdentite.isEmpty())
               {

                   qry.prepare("SELECT COUNT(*) FROM admin WHERE lower(nom) = lower(:name)");
                   qry.bindValue(":name", nouvIdentite);

                   if (qry.exec())
                   {
                       qry.first();
                       int count = qry.value(0).toInt();

                       if (count > 0)
                       {
                           QMessageBox::information(this, tr("Ajout Impossible"), tr("Un employé de cette identité existe déjà!"));
                           return;
                       }
                       else
                       {
                           qry.prepare("UPDATE admin, SET  nom='"+nouvIdentite+"'  WHERE lower(nom)=lower('"+ancienemp+"'); ");
                           if(qry.exec())
                           {
                               QMessageBox::information(this,tr("Succès"), tr("Mise à jour du nom réussie!"));
                               ui->newIdentEmpMaj->setText("");

                           }
                           else
                           {
                               QMessageBox::critical(this,tr("Erreur innatendue"), tr("Une erreure innatendue nous empêche de mettre à jour le nom! Veuillez réessayer."));
                           }


                           ui->comboBox->removeItem(ui->comboBox->findText(ancienemp));
                           ui->comboBox->addItem(nouvIdentite);

                       }
                   }


               }
               if(!nouvUsername.isEmpty())
               {


                   qry.prepare("SELECT COUNT(*) FROM admin WHERE lower(username) = lower(:username)");
                   qry.bindValue(":username", nouvUsername);

                   if (qry.exec())
                   {
                       qry.first();
                       int count = qry.value(0).toInt();

                       if (count > 0)
                       {
                           QMessageBox::information(this, tr("Ajout Impossible"), tr("Un employé avec ce nom d'utilisateur existe déjè existe déjà!"));
                           return;
                       }
                       else
                       {
                           qry.prepare("UPDATE admin, SET  username='"+nouvUsername+"'  WHERE lower(nom)=lower('"+ancienemp+"'); ");
                          if(qry.exec())
                          {
                              QMessageBox::information(this,tr("Succès"), tr("Mise à jour du nom d'utilisateur réussie!"));
                              ui->newUsernameMaj->setText("");

                          }
                          else
                          {
                              QMessageBox::critical(this,tr("Erreur innatendue"), tr("Une erreure innatendue nous empêche de mettre à jour le nom d'utilisateur! Veuillez réessayer."));
                          }
                       }
                   }


               }

               if(!mdp.isEmpty())
               {


                           qry.prepare("UPDATE admin, SET  mdp='"+mdp+"'  WHERE lower(nom)=lower('"+ancienemp+"'); ");
                          if(qry.exec())
                          {
                              QMessageBox::information(this,tr("Succès"), tr("Mise à jour du mot de passe réussie!"));
                              ui->newUsernameMaj->setText("");

                          }
                          else
                          {
                              QMessageBox::critical(this,tr("Erreur innatendue"), tr("Une erreure innatendue nous empêche de mettre à jour le mot de passe! Veuillez réessayer."));
                          }
                       }
                   }


               }
        connexion.close();

    }


}

