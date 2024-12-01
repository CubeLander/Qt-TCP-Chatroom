#pragma once
#include "ClientSocket.h"
#include <qmessagebox.h>

ClientSocket::ClientSocket(QTcpSocket* sock) :
	status(0),
	QObject(),
	socket(sock)
{
	socket->setParent(this);
	connect(socket, &QTcpSocket::readyRead, this, &ClientSocket::readyRead);
	connect(socket, &QTcpSocket::disconnected, this, &ClientSocket::disconnected);

	//qDebug()<< connect(socket, &QTcpSocket::disconnected, this, &ClientSocket::slotDisconnected, Qt::QueuedConnection);
	qDebug() << connect(socket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
	//qDebug() << connect(this, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));

	connect(socket, &QObject::destroyed, this, []() {
		qDebug() << "Socket has been destroyed";
	});
	//这个信号传不过去
	//这个信号传得过去
}

ClientSocket::~ClientSocket(){}


void ClientSocket::slotDisconnected() {
	//QMessageBox::information(nullptr, tr("info"), tr("disconnecting!"));
}
