#pragma once
#include <QtNetwork/qtcpserver.h>
#include <QtNetwork/qtcpsocket.h>
#include <qobject.h>

class ClientSocket: public QObject{
	Q_OBJECT
public:
	QTcpSocket* socket;
	int status;
	QString name;
	ClientSocket(QTcpSocket*);
	//ClientSocket(QTcpSocket);
	~ClientSocket();
signals:
	void disconnected();

	void readyRead();
public slots:
	void slotDisconnected();


};