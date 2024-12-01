#pragma once
#include <QtNetwork/qtcpserver.h>
#include <QtNetwork/qtcpsocket.h>
#include <qobject.h>

class ClientSocket : public QTcpSocket {
	
public:
	int status;
	QString clientName;
	ClientSocket();
	~ClientSocket();




};