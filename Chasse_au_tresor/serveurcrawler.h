/**
  @file serveurcrawler.h
 * @brief The ServeruCrawler class
 * @details Déclartion de la classe ServeurCrawler héritant de la classe QWidget
 * @author Maëva Soulard
 * @date 18/12/2020
 */

#ifndef SERVEURCRAWLER_H
#define SERVEURCRAWLER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QGridLayout>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QRandomGenerator>
#include <QEventLoop>
#include <QBuffer>
#include <QToolButton>

const int TAILLE = 20;

QT_BEGIN_NAMESPACE
namespace Ui { class ServeurCrawler; }
QT_END_NAMESPACE

class ServeurCrawler : public QWidget
{
    Q_OBJECT

public:
    explicit ServeurCrawler(QWidget *parent = nullptr);
    ~ServeurCrawler();
private slots:
    void onQTcpServer_newConnection();
    void onQTcpSocket_readyRead();
    void onQTcpSocket_disconnected();
    void on_pushButton_Lancement_clicked();

private:
    Ui::ServeurCrawler *ui;
    QTcpServer *socketEcoute;
    QList<QTcpSocket *> listeSocketsClient;
    QList<QPoint> listePositions;
    QPoint tresor;
    QGridLayout *grille;

    void EnvoyerDonnees(QTcpSocket *_client, QPoint _pt, QString _message);
    void AfficherGrille();
    void ViderGrille();
    QPoint DonnerPositionUnique();
    double CalculerDistance(QPoint _pos);
};

#endif // SERVEURCRAWLER_H
