#pragma once
#include <QtNetwork/qtcpserver.h>
#include <qobject.h>
#include "TcpClientSocket.h"

class Server :public QTcpServer {
	Q_OBJECT
public:
	Server(QObject* parent = 0, int port = 0);
private:
	const qsizetype headerLength = 4;
	QVector<QTcpSocket*> tcpClientSocketList;
	QByteArray bigBuffer;
	void extractMessages();
	void updateClients(QString);

signals:
	void updateServer(QString);
public slots:
	void clientDisconnected();
	void connectionCame();
	void dataReceived();
	//void dataReceived(QTcpSocket*);

};

