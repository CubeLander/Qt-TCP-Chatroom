#include "TCPServer.h"
#include <qmessagebox.h>

TcpServer::TcpServer(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent,f) 
{

    ContentListWidget = new(QListWidget);
    PortLabel = new QLabel(tr("端口："));
    PortLineEdit = new QLineEdit(this);
    PortLineEdit->setText(QString::number(port));

    CreateBtn = new QPushButton(tr("创建聊天室"));

    port = 8010;

    connect(CreateBtn, SIGNAL(clicked()), this, SLOT(slotCreateServer()));

    setWindowTitle(tr("TCP Server"));

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(ContentListWidget, 0, 0, 1, 2);
    mainLayout->addWidget(PortLabel,  1, 0);
    mainLayout->addWidget(PortLineEdit, 1, 1);
    mainLayout->addWidget(CreateBtn, 2, 0, 1, 2);
}

TcpServer::~TcpServer()
{}

void TcpServer::updateServer(QString msg) {
    ContentListWidget->addItem(msg);
}

void TcpServer::slotCreateServer() {
    server = new Server(this, port);
    connect(server, SIGNAL(updateServer(QString)), this, SLOT(updateServer(QString)));
    CreateBtn->setEnabled(false);
    QString msg = "Server successfully created";

    ContentListWidget->addItem(msg);
}