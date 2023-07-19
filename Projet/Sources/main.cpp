#include "accueil.h" //Inclusion de la bibliothèque "accueil.h"

//Inclusion de différentes bibliothèques nécessaires pour l'application
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QSplashScreen>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); //Création de l'application

    QTranslator translator;//Création d'un traducteur pour permettre la traduction de l'application

    const QStringList uiLanguages = QLocale::system().uiLanguages();//Récupération de la liste des langues préférées de l'utilisateur
    for (const QString &locale : uiLanguages)//Parcours de la liste pour charger le fichier de traduction correspondant à la langue préférée
    {
        const QString baseName = "Projet_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName))
        {
            a.installTranslator(&translator);
            break;
        }
    }
    QPixmap image("Images/img_accueil.png");//Création d'une image pour l'écran de démarrage
    QSplashScreen *splash = new QSplashScreen;//Création de l'écran de démarrage
    splash-> setPixmap(image.scaled(862,500, Qt::KeepAspectRatio));//Affectation de l'image à l'écran de démarrage
    splash-> show();//Affichage de l'écran de démarrage
    Accueil w;//Création de la fenêtre d'accueil
    QTimer::singleShot(3500, splash,SLOT(close()));//Fermeture de l'écran de démarrage après 3.5 secondes
    QTimer::singleShot(3500, &w,SLOT(show()));//Affichage de la fenêtre d'accueil après 3.5 secondes
    return a.exec();//Exécution de l'application
}



