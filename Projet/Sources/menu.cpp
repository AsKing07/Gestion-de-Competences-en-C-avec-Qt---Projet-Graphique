// Inclusion des fichiers nécessaires pour la création de la classe menu
#include "menu.h"
#include "ui_menu.h"

menu::menu(QWidget *parent) : // Constructeur de la classe menu
    QDialog(parent),// Initialisation de la fenêtre de dialogue
    ui(new Ui::menu)// Initialisation de l'interface utilisateur
{
    ui->setupUi(this);// Mise en place de l'interface utilisateur pour la fenêtre de dialogue
    setWindowTitle("Menu");
}

menu::~menu()  // Destructeur de la classe menu
{
    delete ui; // Suppression de l'UI
}

void menu::on_Profil_clicked() // Fonction qui s'exécute lorsque le bouton "Profil" est cliqué
{
    this -> hide(); this -> hide(); // Cache la fenêtre de menu
    Profil pfl;// Initialisation de la fenêtre de profil
    pfl.exec();// Exécution de la fenêtre de profil
}


void menu::on_Competence_clicked() // Slot pour l'ouverture de la fenetre de gestion des compétences
{
    this -> hide(); //On ferme la fenetre du menu
    Competences cpt; //Création d'un objet de la classe compétence
    cpt.exec(); //On ouvre la fenetre des compétences
}


void menu::on_Professionnel_clicked() // Slot pour l'ouverture de la fenetre de gestion des professionnels
{
    this -> hide();
    Professionnel pro;
    pro.exec();
}


void menu::on_employe_clicked() // Slot pour l'ouverture de la fenetre de gestion des employés
{
    this -> hide();
    gestionEmploye emp;
    emp.exec();
}


void menu::on_Importation_clicked() //Slot pour l'importation des données d'un fichier au format cpt dans la base de données(lorsque on clique sur le bouton importation)
{

/*
     *       Cette fonction gère l'événement de clic sur le bouton "Importation" dans le menu de l'application.
     *        Elle affiche d'abord une boîte de dialogue pour avertir l'utilisateur que l'importation effacera toutes
     *        les données existantes dans la base de données. Si l'utilisateur clique sur "OK", la fonction ouvre une
     *        connexion à la base de données et supprime toutes les données des tables "profil", "competences" et "professionnel".
     *         Si aucune erreur n'est rencontrée lors de la suppression des données, la fonction ouvre une boîte de dialogue pour que
     *         l'utilisateur puisse sélectionner un fichier .cpt à importer. Si un fichier est sélectionné, il est lu et les données
     *           sont ajoutées aux tables appropriées dans la base de données.
     *
*/




    QMessageBox msgBox; // Création d'une boîte de dialogue pour avertir l'utilisateur
      
      msgBox.setWindowTitle("ATTENTION");
      msgBox.setText("Cette action supprimera définitivement l'ancienne base de données");//boîte de dialogue pour avertir l'utilisateur que l'importation effacera toutes les données
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
      int ret = msgBox.exec();
  
      if (ret == QMessageBox::Ok)// Si l'utilisateur clique sur "Ok"
      {
                  DbConnection connection;//connexion à la bdd
                  if(!connection.open())
                  {
                     qDebug() << "Echec d'ouverture de la Base de données";
                     QMessageBox::critical(this,tr("Importation interrompue"), tr("Nous n'avons pas pu nous connecter à la database"));
                  }
                  else
                  {

                       int erreur=0;


                       QSqlQuery qry;

                       // Suppression des données de la table "profil"
                       qry.prepare("DELETE FROM profil;");
                       if(!qry.exec())
                       {
                           qDebug() << "Erreur lors de la suppression des données de la table 'profil' : " << qry.lastError().text();
                            erreur++;
                       }

                       // Suppression des données de la table "competences"
                       qry.prepare("DELETE FROM competences;");
                       if(!qry.exec())
                       {
                           qDebug() << "Erreur lors de la suppression des données de la table 'competences' : " << qry.lastError().text();
                            erreur++;
                       }

                       // Suppression des données de la table "professionnel"
                       qry.prepare("DELETE FROM professionnel;");
                       if(!qry.exec())
                       {
                           qDebug() << "Erreur lors de la suppression des données de la table 'professionnel' : " << qry.lastError().text();
                           erreur++;
                       }




                          if(erreur==0)//si aucune erreur lors de la suppression
                          {
                              QMessageBox::information(this,tr("Information"),tr("Le contenu de la base a été vidée en attendant de recevoir de nouvelles données"));

                              QString fileName;
                              fileName = QFileDialog::getOpenFileName(this, tr("Choisir un fichier .cpt"),tr("C:\\"), tr("CPT files (*.cpt);;All Files (*.*)"));//boite de dialogue pour choisir le fichier à importer


                              if (fileName.isEmpty())
                              {
                                  msgBox.close();
                              }
                              else//si un fichier est sélectionné
                              {
                                            QFile file(fileName);
                                            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) //Si on arrive à ouvrir le fichier en lecture
                                            {
                                                     QStringList listCpt;
                                                    QTextStream in(&file);
                                                    in.setCodec("UTF-8");
                                                    while (!in.atEnd())//lecture et traitement de chaque ligne du fichier
                                                    {

                                                        QString line = in.readLine();
                                                         if (line.contains("PFL:"))// si je tombe sur un profil
                                                         {
                                                                 QString nom; int nbrCpt;

                                                                 line = in.readLine();//Je récupere la ligne qui suit qui est celle du nom du profil comme dans le fichier
                                                                 nom=line.trimmed();//J'enregistre le nom dans la variable nom.trimmed() sert à supprimer les espaces et tabulation en début et fin de ligne
                                                                 line = in.readLine(); //je récupere la ligne suivante qui est celle du nombre de compétence
                                                                 nbrCpt=line.toInt();//j'enregistre ce nombre dans la variable nbrCpt apres l'avoir converti en int

                                                                 //Les lignes qui suivent sont celles des cpt qui sont égales à nbrCpt compétence
                                                                 QStringList list;
                                                                 for (int i=0; i<nbrCpt;i++)//Je vais donc parcourir chaque compétence avec un maximum de cpt égal à nbrCpt
                                                                 {
                                                                     line=in.readLine();
                                                                             if (!list.contains(line.trimmed()) )//Je verifie si la des competences du profil contient deja la cpt actu
                                                                             {
                                                                                 list.append(line.trimmed()); //J'ajoute cette cpt à la liste
                                                                             }
                                                                             if(!listCpt.contains(line.trimmed()))
                                                                             {
                                                                                 listCpt.append(line.trimmed());//Je l'ajoute également à la liste générale de toutes les cpt; liste dont j'autai besoin plus tard
                                                                             }
                                                                 }
                                                                 list.sort(); //Je trie en ordre alphabétique la liste
                                                                 QString listString = list.join(","); // J'enregistre les élément de la liste dans un QStringList afin de pouvoir les enregistrer dans la bdd; chaque élément étant séparé d'un autre par ","

                                                                  QSqlQuery qry;
                                                                  qry.prepare("SELECT COUNT(*) AS total FROM profil WHERE Nom='"+nom+"' "); //requette pour vérifier si le profil existe déja
                                                                  if(qry.exec())
                                                                  {
                                                                      int count = 0;
                                                                      while(qry.next())
                                                                      {
                                                                          count = qry.value(0).toInt();
                                                                      }
                                                                      if(count==0) //si le profil n'existe
                                                                      {
                                                                          QSqlQuery qry;
                                                                          qry.exec("SET NAMES 'utf8'");
                                                                          qry.prepare("INSERT INTO  profil(nom, nbrCpt,ListCpt) VALUES(:nom, :nbrCpt, :list) "); //préparation de la requette pour insérer les infos dans la bdd
                                                                          qry.bindValue(":nom", nom);
                                                                          qry.bindValue(":nbrCpt", nbrCpt);
                                                                          qry.bindValue(":list", listString);
                                                                          qry.exec();// exécution de la requête
                                                                      }

                                                                   }

                                                          }



                                                         //Mm méthode pour quand on tombe sur un Professionnel

                                                         if (line.contains("PRO:"))
                                                         {
                                                                 QString nom,prenom,identite; int nbrCpt;

                                                                 line = in.readLine();
                                                                 nom=line.trimmed();

                                                                 line = in.readLine();
                                                                 prenom=line.trimmed();

                                                                 identite=nom+" "+prenom;

                                                                 line = in.readLine();
                                                                 nbrCpt=line.toInt();

                                                                 QStringList list;
                                                                 for (int i=0; i<nbrCpt;i++)
                                                                 {
                                                                     line=in.readLine();
                                                                             if (!list.contains(line.trimmed()) )
                                                                             {
                                                                                 list.append(line.trimmed());
                                                                             }
                                                                             if(!listCpt.contains(line.trimmed()))
                                                                             {
                                                                                 listCpt.append(line.trimmed());
                                                                             }
                                                                 }
                                                                 list.sort();
                                                                 QString listString = list.join(",");

                                                                  QSqlQuery qry;
                                                                  qry.exec("SET NAMES 'utf8'");
                                                                  qry.prepare("SELECT COUNT(*) AS total FROM professionnel WHERE Nom='"+nom+"' AND prenom='"+prenom+"' ");
                                                                  if(qry.exec())
                                                                  {
                                                                      int count = 0;
                                                                      while(qry.next())
                                                                      {
                                                                          count = qry.value(0).toInt();
                                                                      }
                                                                      if(count==0)
                                                                      {
                                                                          QSqlQuery qry;
                                                                          qry.exec("SET NAMES 'utf8'");
                                                                          qry.prepare("INSERT INTO  professionnel(nom,prenom,identite,nbrCpt,ListCpt) VALUES(:nom, :prenom, :identite, :nbrCpt, :list) ");
                                                                          qry.bindValue(":nom", nom);
                                                                          qry.bindValue(":prenom", prenom);
                                                                          qry.bindValue(":identite", identite);
                                                                          qry.bindValue(":nbrCpt", nbrCpt);
                                                                          qry.bindValue(":list", listString);
                                                                          qry.exec();
                                                                      }

                                                                   }

                                                          }




                                                         if (line.contains("CPT:"))
                                                         {
                                                                 int nbrCpt;
                                                                 line = in.readLine();
                                                                 nbrCpt=line.toInt();


                                                                 for (int i=0; i<nbrCpt;i++)
                                                                 {
                                                                     line=in.readLine();

                                                                             if(!listCpt.contains(line.trimmed()))
                                                                             {
                                                                                 listCpt.append(line.trimmed());
                                                                             }
                                                                 }


                                                          }


                                                     }


                                                            listCpt.sort();

                                                            QSqlQuery qry;
                                                            qry.exec("SET NAMES 'utf8'");
                                                            foreach (QString item, listCpt)
                                                            {
                                                                         qry.prepare("INSERT INTO competences (nom) VALUES (:item)");
                                                                         qry.bindValue(":item", item);
                                                                         qry.exec();
                                                            }

                                                           QMessageBox::information(this,tr("Importation terminée"),tr("L'opération est achevée.")); //QMessageBox pour informer l'utilisateur de la fin de l'importation

                                             }

                                            else//Si on a pas pu ouvrir le fichier en lecture
                                            {
                                                QMessageBox::critical(this,tr("impossible de lire les données du fichier"), tr("Erreur lecture de fichier"));
                                            }
                              }
                          }

                          else//Si on a pas pu vider les tables de la bdd
                          {
                                QMessageBox::critical(this,tr("Importation impossible"),tr("Une erreur s'est produite!"));
                          }
                          connection.close();
                     }

           }


            else if (ret == QMessageBox::Cancel)
            {
               msgBox.close();
            }
    
    
  }
    




void menu::on_Exportation_clicked()//slot de l'exportation
{

/*

      Ce code est une fonction qui est appelée lorsque l'utilisateur clique sur un bouton "Exportation" dans une interface utilisateur.
      Il a pour but d'exporter les données de la base de données vers un fichier texte.

      La première chose qui se passe est que la fonction essaie d'ouvrir une connexion à la base de données en utilisant la classe DbConnection.
      Si l'ouverture échoue, un message d'erreur s'affiche et la fonction s'arrête.
      Sinon, la fonction ouvre une boîte de dialogue pour permettre à l'utilisateur de choisir un nom et un emplacement pour le fichier à exporter.
      Si l'utilisateur ne choisit pas de nom, la fonction s'arrête.
      Ensuite, la fonction ouvre le fichier en mode écriture et utilise un flux de sortie pour écrire les données dans le fichier.
      Elle utilise l'encodage UTF-8 pour écrire les données.
      Elle utilise ensuite une requête SQL pour récupérer toutes les données de la table "profil" de la base de données.
      Pour chaque ligne de résultat, elle écrit les valeurs de chaque colonne dans le fichier en utilisant une boucle.
      Elle utilise également une méthode pour séparer les éléments d'une colonne qui contient une liste.
      Elle répète le processus pour la table "professionnel".
      Enfin, elle utilise une autre requête SQL pour compter le nombre de lignes dans la table "competences" et écrit ce nombre dans le fichier à la suite de la zone "CPT:",
      suivi des competences lues dans la base de données.

*/



    DbConnection connection;
    if(!connection.open())
    {
       qDebug() << "Echec d'ouverture de la Base de données";
       QMessageBox::critical(this,tr("Exportation interrompue"), tr("Nous n'avons pas pu nous connecter à la database"));
    }
     else
    {
                // Demande à l'utilisateur de choisir un nom et un emplacement pour le fichier texte
                QString fileName = QFileDialog::getSaveFileName(this, tr("Exporter la base de données"), "", tr("Fichiers cpt (*.cpt)"));

                if (!fileName.isEmpty())
                {
                    QFile file(fileName);
                    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
                    {
                        QTextStream out(&file);
                        out.setCodec("UTF-8");

                            QSqlQuery query1("SELECT nom, nbrCpt, ListCpt FROM profil"); // Exécution de la requête pour récupérer toutes les données de la table "profil"

                           while (query1.next())
                        {
                            QSqlRecord record = query1.record();

                            // Boucle pour récupérer les valeurs de chaque colonne de la ligne actuelle et les écrire dans le fichier texte
                            for (int i = 0; i < record.count(); i++)
                            {
                                if(i==0)
                                {
                                    out<<"PFL:"<<endl;
                                }
                                if(i==2)
                                {
                                    QString valeurChamp = record.value(i).toString(); // Récupération de la valeur du champ
                                    QList<QString> liste = valeurChamp.split(","); // Séparation des éléments de la valeur du champ en utilisant la méthode QString::split
                                    for (int j = 0; j < liste.size(); j++)
                                    {
                                            out << "\t"<< liste[j] << endl; // Ajout de chaque élément de la liste à la sortie de fichier, suivi d'un saut de ligne
                                    }
                                }
                                if(i!=2)
                                {
                                    out << "\t"<<record.value(i).toString() << endl;
                                }


                            }
                            out << endl;

                         }


                            QSqlQuery query2("SELECT nom, prenom, nbrCpt, ListCpt FROM professionnel"); // Exécution de la requête pour récupérer toutes les données de la table "professionnel"

                            while (query2.next())
                            {
                                QSqlRecord record = query2.record();

                                // Boucle pour récupérer les valeurs de chaque colonne de la ligne actuelle et les écrire dans le fichier texte
                                for (int i = 0; i < record.count(); i++)
                                {
                                    if(i==0)
                                    {
                                        out<<"PRO:"<<endl;
                                    }
                                    if(i==3)
                                    {
                                        QString valeurChamp = record.value(i).toString(); // Récupération de la valeur du champ
                                        QList<QString> liste = valeurChamp.split(","); // Séparation des éléments de la valeur du champ en utilisant la méthode QString::split
                                        for (int j = 0; j < liste.size(); j++)
                                        {
                                                out << "\t"<< liste[j] << endl; // Ajout de chaque élément de la liste à la sortie de fichier, suivi d'un saut de ligne
                                        }
                                    }
                                    if(i!=3)
                                    {
                                        out << "\t"<<record.value(i).toString() << endl;
                                    }

                                }
                                out << endl;
                               }


                                QSqlQuery qry;
                                qry.prepare("SELECT COUNT(*) AS total FROM competences ");
                                int count=0 ;
                                if(qry.exec())
                                {

                                    while(qry.next())
                                    {
                                         count = qry.value(0).toInt();//on compte le nombre de ligne retourné de la table competences
                                    }
                                }


                                out<<"CPT:"<<endl;
                                out<<"\t"<<count<<endl;
                                QSqlQuery query3("SELECT nom FROM competences"); // Exécution de la requête pour récupérer toutes les données de la table "competences"

                                while (query3.next())
                                {
                                    QSqlRecord record = query3.record();

                                    // Boucle pour récupérer les valeurs de chaque colonne de la ligne actuelle et les écrire dans le fichier texte
                                    for (int i = 0; i < record.count(); i++)
                                    {
                                                out << "\t"<<record.value(i).toString() << endl; // Ajout de chaque élément de la liste à la sortie de fichier, suivi d'un saut de ligne
                                    }

                                }
                                  out << endl;
                                  file.close();
                                  QMessageBox::information(this,tr("Reussite"), tr("L'exportation a été un succès"));
                      }
                   }
           }

           connection.close();

}


void menu::on_Reinitialiser_clicked() //Réinitialiser la bdd
{
    QMessageBox msgBox;

      msgBox.setWindowTitle("ATTENTION"); //Titre de la boîte de dialogue
      msgBox.setText("Cette action supprimera définitivement le contenu de la base de données"); //Texte de la boîte de dialogue
      msgBox.setIcon(QMessageBox::Critical); //Icone de la boîte de dialogue
      msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);//Boutons de la boîte de dialogue

      int ret = msgBox.exec();//exécution de la boîte de dialogue

      if (ret == QMessageBox::Ok)//Si l'utilisateur clique sur "OK"
      {
                  DbConnection connection;
                  if(!connection.open())//Ouverture de la connexion à la base de données
                  {
                     qDebug() << "Echec d'ouverture de la Base de données";
                     QMessageBox::critical(this,tr("Importation interrompue"), tr("Nous n'avons pas pu nous connecter à la database"));
                  }
                  else
                  {

                       int erreur=0;


                       QSqlQuery qry;

                       // Suppression des données de la table "profil"
                       qry.prepare("DELETE FROM profil;");
                       if(!qry.exec())
                       {
                           qDebug() << "Erreur lors de la suppression des données de la table 'profil' : " << qry.lastError().text();
                            erreur++;
                       }

                       // Suppression des données de la table "competences"
                       qry.prepare("DELETE FROM competences;");
                       if(!qry.exec())
                       {
                           qDebug() << "Erreur lors de la suppression des données de la table 'competences' : " << qry.lastError().text();
                            erreur++;
                       }

                       // Suppression des données de la table "professionnel"
                       qry.prepare("DELETE FROM professionnel;");
                       if(!qry.exec())
                       {
                           qDebug() << "Erreur lors de la suppression des données de la table 'professionnel' : " << qry.lastError().text();
                           erreur++;
                       }



                          if(erreur==0)//Si aucune erreur n'a été détectée lors de la suppression des données
                          {
                              QMessageBox::information(this,tr("Information"),tr("Le contenu de la base a été vidée en attendant de recevoir de nouvelles données"));//boite de dialogue pour informer de la reussite de l'operation

                          }
                  }
                  connection.close();
      }
      else if (ret == QMessageBox::Cancel)
      {
         msgBox.close();
      }
}


