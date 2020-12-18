/********************************************************************************
** Form generated from reading UI file 'serveurcrawler.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVEURCRAWLER_H
#define UI_SERVEURCRAWLER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ServeurCrawler
{
public:
    QLabel *label_Port;
    QSpinBox *spinBox_Port;
    QPushButton *pushButton_Lancement;
    QPushButton *pushButton_Quitter;

    void setupUi(QWidget *ServeurCrawler)
    {
        if (ServeurCrawler->objectName().isEmpty())
            ServeurCrawler->setObjectName(QString::fromUtf8("ServeurCrawler"));
        ServeurCrawler->resize(800, 642);
        label_Port = new QLabel(ServeurCrawler);
        label_Port->setObjectName(QString::fromUtf8("label_Port"));
        label_Port->setGeometry(QRect(10, 490, 312, 25));
        spinBox_Port = new QSpinBox(ServeurCrawler);
        spinBox_Port->setObjectName(QString::fromUtf8("spinBox_Port"));
        spinBox_Port->setGeometry(QRect(140, 480, 153, 25));
        spinBox_Port->setMinimum(8888);
        spinBox_Port->setMaximum(9999);
        pushButton_Lancement = new QPushButton(ServeurCrawler);
        pushButton_Lancement->setObjectName(QString::fromUtf8("pushButton_Lancement"));
        pushButton_Lancement->setGeometry(QRect(250, 500, 100, 24));
        pushButton_Quitter = new QPushButton(ServeurCrawler);
        pushButton_Quitter->setObjectName(QString::fromUtf8("pushButton_Quitter"));
        pushButton_Quitter->setGeometry(QRect(410, 540, 80, 24));

        retranslateUi(ServeurCrawler);
        QObject::connect(pushButton_Quitter, SIGNAL(clicked()), ServeurCrawler, SLOT(close()));

        QMetaObject::connectSlotsByName(ServeurCrawler);
    } // setupUi

    void retranslateUi(QWidget *ServeurCrawler)
    {
        ServeurCrawler->setWindowTitle(QCoreApplication::translate("ServeurCrawler", "ServeurCrawler", nullptr));
        label_Port->setText(QCoreApplication::translate("ServeurCrawler", "Port d'\303\251coute", nullptr));
        pushButton_Lancement->setText(QCoreApplication::translate("ServeurCrawler", "Lancement", nullptr));
        pushButton_Quitter->setText(QCoreApplication::translate("ServeurCrawler", "Quitter", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ServeurCrawler: public Ui_ServeurCrawler {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVEURCRAWLER_H
