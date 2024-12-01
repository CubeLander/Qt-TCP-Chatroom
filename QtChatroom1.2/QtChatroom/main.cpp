#include "TCPServer.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	//a.setFont(QFont("Microsoft Yahei", 9));
	TcpServer w;
	w.show();
	return a.exec();
	//return a.exec();
}
