#include "TcpClientSocket.h"
#include <qmessagebox.h>
TcpClientSocket::TcpClientSocket(QObject* parent) {
	QMessageBox::information(nullptr, tr("info"), tr("Listen socket created!"));

	connect(this, SIGNAL(readyRead()), this, SLOT(dataReceived()));
	connect(this, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));

}

void TcpClientSocket::dataReceived() {
	QMessageBox::information(nullptr, tr("info"), tr("Receiving data!"));

	while (bytesAvailable() > 0) {
		int length = bytesAvailable();
		char buf[1024] = {};
		read(buf, length);
		QString msg = buf;
		emit updateClients(msg, length);
	}
}

void TcpClientSocket::slotDisconnected() {
	emit disconnected(this->socketDescriptor());
}