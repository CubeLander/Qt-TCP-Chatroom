#include "Tcpclient.h"  // 引入TcpClient类的头文件
#include <qmessagebox.h>
#include <qhostinfo.h>


// TcpClient类的构造函数，接受一个QWidget指针作为父窗口和Qt的窗口标志
TcpClient::TcpClient(QWidget* parent, Qt::WindowFlags f)
    : QDialog(parent, f)  // 调用QDialog的构造函数初始化TcpClient
{
    setWindowTitle(tr("TCP Client"));

    mainLayout = new QGridLayout(this);

    contentListWidget = new QListWidget;
    sendLineEdit = new QLineEdit;
    sendBtn = new QPushButton(tr("发送"));

    userNameLabel = new QLabel(tr("用户名:"));
    userNameLineEdit = new QLineEdit;
    userNameLineEdit->setText(QString("Username"));

    serverIpLabel = new QLabel(tr("服务器地址:"));
    serverIpLineEdit = new QLineEdit;
    serverIpLineEdit->setText(QString::fromUtf8("127.0.0.1"));

    port = 8010;

    portLabel = new QLabel(tr("端口:"));
    portLineEdit = new QLineEdit;
    portLineEdit->setText(QString::number(port));

    enterBtn = new QPushButton(tr("进入聊天室"));

    serverIP = new QHostAddress();

    status = false;

    connect(enterBtn, SIGNAL(clicked()), this, SLOT(slotEnter()));
    connect(sendBtn, SIGNAL(clicked()), this, SLOT(slotSend()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(clientDisconnected));
    sendBtn->setEnabled(false);


    // 使用mainLayout将控件添加到布局中
    mainLayout->addWidget(contentListWidget, 0, 0, 1, 2);
    mainLayout->addWidget(sendLineEdit, 1, 0);
    mainLayout->addWidget(sendBtn, 1, 1);
    mainLayout->addWidget(userNameLabel, 2, 0);
    mainLayout->addWidget(userNameLineEdit, 2, 1);
    mainLayout->addWidget(serverIpLabel, 3, 0);
    mainLayout->addWidget(serverIpLineEdit, 3, 1);
    mainLayout->addWidget(portLabel, 4, 0);
    mainLayout->addWidget(portLineEdit, 4, 1);
    mainLayout->addWidget(enterBtn, 5, 0, 1, 2);  // 这个按钮跨越两列
}

TcpClient::~TcpClient() {

}

void TcpClient::slotEnter() {
    if (!status) {
        QString ip = serverIpLineEdit->text();
        if (!serverIP->setAddress(ip)) {
            QMessageBox::information(this, tr("error"), tr("server ip address error!"));
            return;
        }
        if (userNameLineEdit->text() == "") {
            QMessageBox::information(this, tr("error"), tr("User name error!"));
            return;
        }
        userName = userNameLineEdit->text();

        tcpSocket = new QTcpSocket(this);
        connect(tcpSocket, SIGNAL(connected()), this, SLOT(slotConnected()));
        connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
        connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
        tcpSocket->connectToHost(*serverIP, port);
        status = true;
    } else {
        QString msg = tr("%1 leaves chat room").arg(userName);
        Send(msg);
        tcpSocket->disconnectFromHost(); 
        status = false;
    
    
    }
}

void TcpClient::Send(QString msg) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_8);
    //out << (msg.length() + 1)/2;
    //out << msg.length();
    out << msg.toUtf8();
    tcpSocket->write(block);
    QString hexString = block.toHex();
    //QMessageBox::information(this, tr("info"), tr("Message Sent,Data:%1").arg(hexString));

}

void TcpClient::slotConnected() {
    sendBtn->setEnabled(true);
    enterBtn->setText(tr("离开"));
    QString msg = tr("%1 enters chat Room\0").arg(userName);
    Send(msg);
}

void TcpClient::slotSend() {
    if (sendLineEdit->text() == "") {
        return;
    }
    QString msg = tr("%1 : %2").arg(userName).arg(sendLineEdit->text());
    Send(msg);
    sendLineEdit->clear();
}

void TcpClient::clientDisconnected() {
    sendBtn->setEnabled(false);
    enterBtn->setText(tr("进入聊天室"));
}

void TcpClient::dataReceived() {
    if (tcpSocket->bytesAvailable() < 0) {
        return;
    }
    QByteArray buffer = tcpSocket->readAll();
    //QMessageBox::information(nullptr, tr("event"), tr("Buffer is %1").arg(buffer));

    bigBuffer.append(buffer);
    //int total_length = bigBuffer->size();
    qint32 messageLength;
    while (bigBuffer.size()) {
        QDataStream recv_data(&bigBuffer, QIODevice::ReadOnly);
        recv_data.setVersion(QDataStream::Qt_6_8);
        if (bigBuffer.size() < headerLength)
            break;
        recv_data >> messageLength;
        //QMessageBox::information(nullptr, tr("event"), tr("messageLength:%1").arg(messageLength));

        if (bigBuffer.size() < headerLength + messageLength)
            break;
        QByteArray msg;
        for (int i = 0; i < messageLength; i++) {
            char c;
            recv_data >> c;
            msg.append(c);
        }
        //QMessageBox::information(nullptr, tr("event"), tr("msg:%1").arg(msg));
        //QMessageBox::information(nullptr, tr("event"), tr("recv_data:%1").arg(recv_data.));

        //recv_data.readBytes(msg.data(),messageLength);
        QString msgStr = QString::fromUtf8(msg);
        //QMessageBox::information(nullptr, tr("event"), tr("msgstr:%1").arg(msgStr));

        //QMessageBox::information(nullptr, tr("event"), tr("Message received!") + msgStr);
        contentListWidget->addItem(msgStr);
        //updateClients(msgStr);
        bigBuffer = bigBuffer.right(bigBuffer.length() - messageLength - headerLength);
    }
}