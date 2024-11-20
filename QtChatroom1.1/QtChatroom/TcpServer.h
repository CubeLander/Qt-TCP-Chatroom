#pragma once

#include <QtWidgets/QMainWindow>
#include <qdialog.h>
#include <qlistwidget.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qgridlayout.h>
#include "ui_TCPServer.h"
#include "server.h"

class TcpServer : public QDialog
{
    Q_OBJECT

public:
    TcpServer(QWidget *parent = nullptr, Qt::WindowFlags f = (Qt::WindowFlags)0);
    ~TcpServer();

private:
    QListWidget* ContentListWidget;
    QLabel* PortLabel;
    QLineEdit* PortLineEdit;
    QPushButton* CreateBtn;
    QGridLayout* mainLayout;
    //Ui::TCPServerClass ui;
private:
    int port;
    Server* server;
public slots:
    void slotCreateServer();
    void updateServer(QString);
};

