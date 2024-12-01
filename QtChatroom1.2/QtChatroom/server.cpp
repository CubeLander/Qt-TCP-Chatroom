#include "server.h"
#include <qmessagebox.h>

Server::Server(QObject* parent, int port) :QTcpServer(parent) {
	if (!listen(QHostAddress::LocalHost, port)) {
		QMessageBox::information(nullptr, tr("error"), tr("Server socket failed!"));
	}

	connect(this, SIGNAL(newConnection()), this, SLOT(connectionCame()));
}

void Server::connectionCame() {
	//QTcpSocket* connection = this->nextPendingConnection();
	
	ClientSocket* tcpClientSocket = new ClientSocket(this->nextPendingConnection());
	//QMessageBox::information(nullptr, tr("info"), tr("connection came!"));
	//QMessageBox::information(nullptr, tr("info"), tr("connection sd:%1!").arg(QString::number(connection->socketDescriptor())));
	//QMessageBox::information(nullptr, tr("info"), tr("connection sd:%1!").arg(QString::number(tcpClientSocket->socketDescriptor())));

	//tcpClientSocket->socketDescriptor();
	tcpClientSocketList.append(tcpClientSocket);

	connect(tcpClientSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
	connect(tcpClientSocket, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
}

void Server::extractMessages(ClientSocket* recvSocket) {
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
		if (recvSocket->status == 1) {
			updateClients(msgStr);

		} else {
			updateClientName(msg, recvSocket);
			//QMessageBox::information(nullptr, tr("info"), tr("received client name!"));

		}
		bigBuffer = bigBuffer.right(bigBuffer.length() - messageLength - headerLength);
	}

}

void Server::dataReceived() {
	//QMessageBox::information(nullptr, tr("info"), tr("data received!"));

	ClientSocket* clientSocket = dynamic_cast<ClientSocket*>(sender());

	if (clientSocket->socket->bytesAvailable() <= 0)
		return;

	QByteArray buffer = clientSocket->socket->readAll();
	bigBuffer.append(buffer);
	extractMessages(clientSocket);

}

void Server::updateClients(QString msg) {

	emit updateServer(msg);
	for (auto i : tcpClientSocketList) {
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_6_8);
		out << msg.toUtf8();
		i->socket->write(block);
	}
}

void Server::updateClientName(QString name, ClientSocket* clientSocket) {
	clientSocket->name = name;
	clientSocket->status = 1;
	QString msg = tr("%1 enters chatroom.").arg(name);
	updateClients(msg);
}

void Server::clientDisconnected() {
	ClientSocket* clientSocket = dynamic_cast<ClientSocket*>(sender());
	QString msg = tr("%1 leaves chatroom.").arg(clientSocket->name);
	updateClients(msg);
	clientSocket->socket->disconnect();
	return;
}