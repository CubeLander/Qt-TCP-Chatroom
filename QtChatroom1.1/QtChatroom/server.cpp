#include "server.h"
#include <qmessagebox.h>

Server::Server(QObject* parent, int port) :QTcpServer(parent) {
	if (!listen(QHostAddress::LocalHost, port)) {
		QMessageBox::information(nullptr, tr("error"), tr("Server socket failed!"));
	}

	connect(this, SIGNAL(newConnection()), this, SLOT(connectionCame()));
}

void Server::connectionCame() {
	QTcpSocket* tcpClientSocket = this->nextPendingConnection();
	tcpClientSocketList.append(tcpClientSocket);

	connect(tcpClientSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
	connect(tcpClientSocket, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
}

void Server::extractMessages() {
	qint32 messageLength;
	while (bigBuffer.size()) {
		QDataStream recv_data(&bigBuffer, QIODevice::ReadOnly);
		recv_data.setVersion(QDataStream::Qt_6_8);
		if (bigBuffer.size() < headerLength)
			break;
		recv_data >> messageLength;

		if (bigBuffer.size() < headerLength + messageLength)
			break;
		QByteArray msg;
		for (int i = 0; i < messageLength; i++) {
			char c;
			recv_data >> c;
			msg.append(c);
		}
		QString msgStr = QString::fromUtf8(msg);
		updateClients(msgStr);
		bigBuffer = bigBuffer.right(bigBuffer.length() - messageLength - headerLength);
	}

}

void Server::dataReceived() {
	QTcpSocket* recvSocket = qobject_cast<QTcpSocket*>(sender());

	if (recvSocket->bytesAvailable() <= 0)
		return;

	QByteArray buffer = recvSocket->readAll();
	bigBuffer.append(buffer);
	extractMessages();

}

void Server::updateClients(QString msg) {

	emit updateServer(msg);
	for (auto i : tcpClientSocketList) {
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_6_8);
		out << msg.toUtf8();
		i->write(block);
	}
}

void Server::clientDisconnected() {
	QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
	clientSocket->disconnect();
	return;
}