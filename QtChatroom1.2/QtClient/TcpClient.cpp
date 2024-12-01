#include "Tcpclient.h"  // 引入TcpClient类的头文件


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

    connectTimer = new QTimer(this);


    enterBtn = new QPushButton(tr("进入聊天室"));

    serverIP = new QHostAddress();

    status = false;

    connect(enterBtn, SIGNAL(clicked()), this, SLOT(slotEnter()));
    connect(sendBtn, SIGNAL(clicked()), this, SLOT(slotSend()));
    //connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(clientDisconnected));
    connect(connectTimer, &QTimer::timeout, this, &TcpClient::slotTimeout);

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
        //connect(connectTimer, &QTimer::timeout, this, QOverload<>::of(&AnalogClock::update));
        connectTimer->start(1000);
        connect(tcpSocket, SIGNAL(connected()), this, SLOT(slotConnected()));
        connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
        connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));

        tcpSocket->connectToHost(*serverIP, port);
        //status = 1;
    } else {
        //QString msg = tr("%1 leaves chat room").arg(userName);
        //Send(msg);
        tcpSocket->disconnectFromHost(); 
    
    
    }
}

void TcpClient::slotTimeout() {
    tcpSocket->abort();
    connectTimer->stop();
    QMessageBox::information(this, tr("error"), tr("Connection failed!"));
}





void TcpClient::Send(QString msg) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_8);
    out << msg.toUtf8();
    tcpSocket->write(block);
}

void TcpClient::slotConnected() {
    connectTimer->stop();
    sendBtn->setEnabled(true);
    status = 1;
    enterBtn->setText(tr("离开"));
    //QString msg = tr("%1 enters chat Room\0").arg(userName);
    QString msg = userName;
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
    status = false;
    sendBtn->setEnabled(false);
    enterBtn->setText(tr("进入聊天室"));
}

void TcpClient::dataReceived() {
    if (tcpSocket->bytesAvailable() < 0) {
        return;
    }
    QByteArray buffer = tcpSocket->readAll();

    bigBuffer.append(buffer);
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
        QString msgStr = QString::fromUtf8(msg);
        contentListWidget->addItem(msgStr);
        bigBuffer = bigBuffer.right(bigBuffer.length() - messageLength - headerLength);
    }
}