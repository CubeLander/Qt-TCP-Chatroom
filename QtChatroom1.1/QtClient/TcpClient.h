#pragma once
#include <qdialog.h>
#include <qlistwidget.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qgridlayout.h>
#include <qhostaddress.h>
#include <qtcpsocket.h>
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
public slots:
	void slotEnter();
	void slotConnected();
	void clientDisconnected();
	void dataReceived();
	void slotSend();
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