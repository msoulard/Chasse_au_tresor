/**
 * @file serveurcrawler.cpp
 * @details Définition et déclaration des méthodes de la classe ServeurCrawler
 * @author Maëva Soulard
 * @date 18/12/2020
 */

#include "serveurcrawler.h"
#include "ui_serveurcrawler.h"

/**
 * @brief qtPause
 * @param millisecondes
 */
void qtPause(int millisecondes)
{
    QTimer timer;
    timer.start(millisecondes);
    QEventLoop loop;
    QObject::connect(&timer,&QTimer::timeout,&loop,&QEventLoop::quit);
    loop.exec();
}

/**
 * @brief ServeurCrawler::ServeurCrawler
 * @param parent
 * @details Constructeur de la classe ServeurCrawler
 */
ServeurCrawler::ServeurCrawler(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ServeurCrawler)
{
    ui->setupUi(this);
    socketEcoute =  new QTcpServer;
    if(!connect(socketEcoute, &QTcpServer::newConnection, this, &ServeurCrawler::onQTcpServer_newConnection)){
        qDebug() << "Problème lors de la nouvelle connection";
    }
    grille = new QGridLayout;
    AfficherGrille();
    //ajouter la case du trésor
    tresor = DonnerPositionUnique();
    grille->itemAtPosition(tresor.y(), tresor.x())->widget()->setStyleSheet("background-color : red");
}

/**
 * @brief ServeurCrawler::~ServeurCrawler
 * @details Destructeur de la classe ServeurCrawler
 */
ServeurCrawler::~ServeurCrawler()
{
    delete ui;
    delete socketEcoute;
}

/**
 * @brief ServeurCrawler::onQTcpServer_newConnection
 * @details Méthode qui permet d'écouter les nouvelles connections et de les prendre
 */
void ServeurCrawler::onQTcpServer_newConnection()
{
    QTcpSocket *client;
    client = socketEcoute->nextPendingConnection();
    connect(client,&QTcpSocket::readyRead,this,&ServeurCrawler::onQTcpSocket_readyRead);
    connect(client, &QTcpSocket::disconnected, this, &ServeurCrawler::onQTcpSocket_disconnected);
    listeSocketsClient.append(client);
    QPoint position = DonnerPositionUnique();
    listePositions.append(position);
    EnvoyerDonnees(client, position, "start");
    grille->itemAtPosition(position.y(), position.x())->widget()->setStyleSheet("background-color : black");
    qDebug() << "un client est connecté";
}

/**
 * @brief ServeurCrawler::onQTcpSocket_readyRead
 * @details Méthode qui permet d'écouter ce que le client dit et de lui répondre
 */
void ServeurCrawler::onQTcpSocket_readyRead()
{
    //Ecouter ce que le client envoie
    QTcpSocket *client=qobject_cast<QTcpSocket*>(sender());
    //récupérer la position du client dans la liste des connexions
    int index=listeSocketsClient.indexOf(client);
    quint16 taille = 0;
    QChar commande;
    //récupérer ce que le client envoie
    //si le nombre d'octets reçu est au moins égal à celui de la taille de ce que l'on doit recevoir
    if(client->bytesAvailable() >= (qint64)sizeof (taille)){
        //association de la socket au flux d'entrée
        QDataStream in(client);
        //extraire la taille de ce que l'on doit recevoir
        in >> taille;
        //si le nombre d'octets reçu est au moins égal à celui de ce que l'on doit recevoir
        if(client->bytesAvailable() >= (qint64)taille){
            //extraire la demande du client
            in >> commande;
        }
    }
    //Etudier la demande du client
    QPoint position = listePositions.at(index);
    grille->itemAtPosition(position.y(), position.x())->widget()->setStyleSheet("background-color : white");
    QPoint positionTest;
    switch (commande.toLatin1()) {
    //vers le haut
    case 'U' :
        positionTest.setX(position.x());
        positionTest.setY(position.y()-1);
        if(positionTest.y() <= -1){
            position.setY(position.y()+1);
        }
        else{
            position.setY(position.y()-1);
        }
        break;
        //vers le bas
    case 'D' :
        positionTest.setX(position.x());
        positionTest.setY(position.y()+1);
        if(positionTest.y() >= 20){
            position.setY(position.y()-1);
        }
        else{
            position.setY(position.y()+1);
        }
        break;
        //vers la gauche
    case 'L' :
        positionTest.setX(position.x()-1);
        positionTest.setY(position.y());
        if(positionTest.x() <= -1){
            position.setX(position.x()+1);
        }
        else{
            position.setX(position.x()-1);
        }
        break;
        //vers la droite
    case 'R' :
        positionTest.setX(position.x()+1);
        positionTest.setY(position.y());
        if(positionTest.x() >= 20){
            position.setX(position.x()-1);
        }
        else{
            position.setX(position.x()+1);
        }
        break;
    }
    //si le joueur a trouvé le trésor
    if(position == tresor){
        QPoint fin;
        fin.setX(-1);
        fin.setY(-1);
        foreach (QTcpSocket *joueur, listeSocketsClient) {
            EnvoyerDonnees(joueur, fin, QString("Victoire de ")+QString(client->peerAddress().toString()));
        }
        foreach (QTcpSocket *joueur, listeSocketsClient) {
            joueur->close();
        }
        ViderGrille();
    }
    else{
        //si il y a une collision
        if(listePositions.contains(position)){
            int indexCollision = listePositions.indexOf(position);
            position = DonnerPositionUnique();
            EnvoyerDonnees(client, position, "Collision");
            grille->itemAtPosition(position.y(), position.x())->widget()->setStyleSheet("background-color : black");
            listePositions.replace(index, position);
            QPoint positionAutreJoueur = listePositions.at(indexCollision);
            grille->itemAtPosition(positionAutreJoueur.y(), positionAutreJoueur.x())->widget()->setStyleSheet("background-color : white");
            positionAutreJoueur = DonnerPositionUnique();
            listePositions.replace(indexCollision,positionAutreJoueur);
            QTcpSocket *autreJoueur = listeSocketsClient.at(indexCollision);
            EnvoyerDonnees(autreJoueur, positionAutreJoueur, "Collision");
            grille->itemAtPosition(positionAutreJoueur.y(), positionAutreJoueur.x())->widget()->setStyleSheet("background-color : black");
        }
        else{
            //si il n'y a rien dans la case
            if(!listePositions.contains(position) && !(position == tresor)){
                EnvoyerDonnees(client, position, "vide");
                listePositions.replace(index, position);
                grille->itemAtPosition(position.y(), position.x())->widget()->setStyleSheet("background-color : black");
            }
        }
    }
}

/**
 * @brief ServeurCrawler::onQTcpSocket_disconnected
 * @details Méthode qui permet de supprimer de la liste un client qui est déconnecté
 */
void ServeurCrawler::onQTcpSocket_disconnected()
{
    QTcpSocket *client=qobject_cast<QTcpSocket*>(sender());
    //récupérer la position du client dans la liste des connexions
    int index=listeSocketsClient.indexOf(client);
    QPoint position = listePositions.at(index);
    grille->itemAtPosition(position.y(), position.x())->widget()->setStyleSheet("background-color : white");
    listePositions.removeAt(index);
    //suppression du client
    listeSocketsClient.removeOne(client);
    qDebug() << "un client est déconnecté";
}

/**
 * @brief ServeurCrawler::on_pushButton_Lancement_clicked
 * @details Méthode qui permet de lancer le serveur
 */
void ServeurCrawler::on_pushButton_Lancement_clicked()
{
    if(socketEcoute->listen(QHostAddress::Any, ui->spinBox_Port->text().toInt())){
        qDebug() << "Le serveur est lancé";
        qDebug() << "coordonnées trésor : " << tresor.x() << ", " << tresor.y();
    }
    ui->pushButton_Lancement->setEnabled(false);
}

/**
 * @brief ServeurCrawler::EnvoyerDonnees
 * @param _client
 * @param _pt
 * @param _message
 * @details Méthode qui permet d'envoyer un message au client
 */
void ServeurCrawler::EnvoyerDonnees(QTcpSocket *_client, QPoint _pt, QString _message)
{
    quint16 taille = 0;
    QBuffer tampon;
    double distance = CalculerDistance(_pt);
    tampon.open(QIODevice::WriteOnly);
    //association du tampon au flux de sortie
    QDataStream out(&tampon);
    //construction de la trame
    out<<taille<<_pt<<_message<<distance;
    //calcul de la taille de la trame
    taille = tampon.size()-sizeof (taille);
    //placement sur la premiere position du flux pour pouvoir modifier la taille
    tampon.seek(0);
    //modification de la taille de la trame
    out << taille;
    //envoie du QByteArray du tampon via la socket
    _client->write(tampon.buffer());
}

/**
 * @brief ServeurCrawler::AfficherGrille
 * @details Méthode qui permet d'afficher la grille de jeu
 */
void ServeurCrawler::AfficherGrille()
{
    for(int ligne=0; ligne<TAILLE; ligne++)
    {
        for (int colonne=0; colonne<TAILLE; colonne++)
        {
            QToolButton *b=new QToolButton();
            b->setEnabled(false);
            grille->addWidget(b,ligne,colonne,1,1);
        }
    }
    //Placement sur la grille des objets
    grille->addWidget(ui->label_Port, TAILLE, 0, 1, 5);
    grille->addWidget(ui->spinBox_Port, TAILLE, 3, 1, 5);
    grille->addWidget(ui->pushButton_Lancement, TAILLE, 9, 1, 5);
    grille->addWidget(ui->pushButton_Quitter, TAILLE, 15, 1, 5);
    this->setLayout(grille);
}

/**
 * @brief ServeurCrawler::ViderGrille
 * @details Méthode qui permet de vider la grille de jeu
 */
void ServeurCrawler::ViderGrille()
{
    for(int ligne=0; ligne<TAILLE; ligne++)
    {
        for (int colonne=0; colonne<TAILLE; colonne++)
        {
            grille->itemAtPosition(ligne, colonne)->widget()->setStyleSheet("background-color : white");
        }
    }
    tresor = DonnerPositionUnique();
    grille->itemAtPosition(tresor.y(), tresor.x())->widget()->setStyleSheet("background-color : red");
}

/**
 * @brief ServeurCrawler::DonnerPositionUnique
 * @return
 * @details Méthode qui permet de donner sa prosition sur la grille à chaque client
 */
QPoint ServeurCrawler::DonnerPositionUnique()
{
    QRandomGenerator gen;
    QPoint pt;
    gen.seed(QDateTime::currentMSecsSinceEpoch());
    int ligne;
    int colonne;
    do
    {
        ligne = gen.bounded(TAILLE);
        qtPause(20);
        colonne = gen.bounded(TAILLE);
        qtPause(20);
        pt = QPoint(colonne,ligne);
    }while (listePositions.contains(pt));

    return pt;
}

/**
 * @brief ServeurCrawler::CalculerDistance
 * @param _pos
 * @return
 * @details Méthode qui permet de donner la distance séparant le trésor du client
 */
double ServeurCrawler::CalculerDistance(QPoint _pos)
{
    double distance;
    int xVecteur = tresor.x()-_pos.x();
    int yVecteur = tresor.y()-_pos.y();
    distance = sqrt((xVecteur*xVecteur + yVecteur*yVecteur));
    return distance;
}
