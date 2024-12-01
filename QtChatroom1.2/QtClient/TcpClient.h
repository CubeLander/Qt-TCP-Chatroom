#pragma once
#include <qdialog.h>
#include <qlistwidget.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qgridlayout.h>
#include <qhostinfo.h>
#include <qhostaddress.h>
#include <qtcpsocket.h>
#include <qtimer.h>
#include <qmessagebox.h>

class TcpClient :public QDialog {
	Q_OBJECT
private:
	const int headerLength = 4;
	bool status;
	int port;
	QHostAddress* serverIP;
	QString userName;
	QTcpSocket* tcpSocket;
	QByteArray bigBuffer;
	QTimer* connectTimer;
public slots:
	void slotEnter();
	void slotConnected();
	void clientDisconnected();
	void dataReceived();
	void slotSend();
	void slotTimeout();
	//void slotError(QAbstractSocket::SocketError socketError);
private:
	void Send(QString);


public:
	TcpClient(QWidget* parent = 0, Qt::WindowFlags f = (Qt::WindowFlags)0);
	~TcpClient();
private:
	QGridLayout* mainLayout;

	QListWidget* contentListWidget;
	QLineEdit* sendLineEdit;
	QPushButton* sendBtn;
	QLabel* userNameLabel;
	QLineEdit* userNameLineEdit;
	QLabel* serverIpLabel;
	QLineEdit* serverIpLineEdit;
	QLabel* portLabel;
	QLineEdit* portLineEdit;
	QPushButton* enterBtn;
};