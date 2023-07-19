#include "accueil.h" //inclusion de l'en-tête de la classe Accueil
#include "ui_accueil.h" //inclusion de l'en-tête de l'interface graphique de la classe Accueil

Accueil::Accueil(QWidget *parent):  //définition du constructeur de la classe Accueil
      QDialog(parent),
      ui(new Ui::Accueil)
{
    ui->setupUi(this);//initialisation de l'interface graphique

}

Accueil::~Accueil() //définition du destructeur de la classe Accueil
{
    delete ui; //suppression de l'interface graphique
}



void Accueil::on_Connection_clicked()  //méthode qui est appelée lorsque l'utilisateur clique sur le bouton "Connection"
{
    QString username,password; //déclaration de variables pour stocker le nom d'utilisateur et le mot de passe
    //récupération du nom d'utilisateur et du mot de passe saisis par l'utilisateur à partir de l'interface graphique
    username = ui -> user -> text();
    password = ui -> mdp -> text();

    if(username.isEmpty() || password.isEmpty())  //vérification si les champs nom d'utilisateur et mot de passe sont vides
    {
        ui -> statut -> setText("Le nom d'utilisateur et le mot de passe doivent être renseignés");//affichage d'un message d'erreur dans l'interface graphique
        QMessageBox::critical(this,"Remplissage","Le nom d'utilisateur et le mot de passe doivent être renseignés");//affichage d'une boîte de dialogue d'erreur
        return;//sortie de la méthode
    }

    DbConnection connection;//création d'un objet de la classe DbConnection pour gérer la connexion à la base de données
    if(!connection.open())//ouverture de la connexion à la base de données
    {
       qDebug() << "Echec d'ouverture de la Base de données";//sortie de la méthode en cas d'échec d'ouverture de la base de données
       return;
    }
    else
    {
        QSqlQuery qry;//création d'un objet de la classe QSqlQuery pour exécuter des requêtes SQL
        qry.prepare("SELECT COUNT(*) AS total FROM admin WHERE username='"+username+"' and mdp='"+password+"'"); //préparation de la requête pour vérifier si les informations de connexion saisies par l'utilisateur existent dans la table "admin" de la base de données

        if(qry.exec())
        {
            int count = 0;
            while(qry.next())
            {
                count = qry.value(0).toInt();//On compte le nombre de personnes ayant ces informations de connexion
            }
            if(count==1)
            {
                connection.close();//fermeture de la connexion à la base de donnéees
                ui -> statut -> setText("Nom d'utilisateur et Mot de Passe corrects");//affichage d'un message indiquant que les informations de connexion sont correctes
                this -> hide();//masquage de la fenêtre de connexion
                menu men;
                men.exec();//affichage du menu principal
            }
            else if (count>1)
            {

                      ui -> statut -> setText("Utilisateur en doublon dans la base");//affichage d'un message indiquant qu'il y a un doublon d'utilisateur dans la base de données
            }
            else
            {
                ui -> statut -> setText("Nom d'utilisateur ou Mot de Passe incorrects");

            }
        }
        else//Si on a pas pu exécuter la requete
        {
            qDebug() << "Exécution de la requette impossible: " << qry.lastError();
             ui -> statut -> setText("Une erreur fatale s'est produite.");
        }
        connection.close();//fermeture de la connexion à la base de donnéees
    }
}





void Accueil::on_apropos_clicked()
{
    QString aboutText = "Nom de l'application : CPT Manager\n"
                           "Version : 2.0\n"
                           "Développeur : SONON Charbel\n"
                           "GitHub : github.com/AsKing07\n"
                           "Email : charbelsnn@gmail.com\n\n"
                           "Cette application est une application de gestion de compétences à l’attention de jeunes professionnels.\n"
                           "Il s'agit de la version évoluée de CPT Manager 1.0\n Une interface graphique est mise à votre disposition pour une expérience utilisateur plus aisée contrairement à la version précédente en mode console.\n"
                           "Pour tout besoin, veuillez vous référer au manuel d'utilisation!";

     QMessageBox::about(nullptr, "A propos de  l' application", aboutText);


}

