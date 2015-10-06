#include "userinterface.h"
#include <QApplication>
#include "serialportmanager.h"
#include "userinterface.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UserInterface w;
    w.show();
    //UserInterface* m_interface = new UserInterface();
    SerialPortManager *singleton_SerialPortManager = SerialPortManager::GetInstance();
    QWidget::connect(singleton_SerialPortManager, SIGNAL(Send(QByteArray&)),
                     &w, SLOT(SerialReceived(QByteArray&)));
    return a.exec();
}
