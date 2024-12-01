#pragma once
#include <QtNetwork/qtcpserver.h>
#include <qobject.h>
#include "ClientSocket.h"

class Server :public QTcpServer {
	Q_OBJECT
public:
	Server(QObject* parent = 0, int port = 0);
private:
	const qsizetype headerLength = 4;
	QVector<ClientSocket*> tcpClientSocketList;
	//QVector<>
	QByteArray bigBuffer;



	void extractMessages(ClientSocket*);
	void updateClients(QString);
	void updateClientName(QString, ClientSocket*);

signals:
	void updateServer(QString);
public slots:
	void clientDisconnected();
	void connectionCame();
	void dataReceived();
	//void dataReceived(QTcpSocket*);

};

