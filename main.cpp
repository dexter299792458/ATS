#include "userinterface.h"
#include <QApplication>
#include "serialportmanager.h"
#include "userinterface.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UserInterface w;
    w.show();

    //Voor een goed Object Georienteerd ontwerp is het van belang dat de klassen
    //`Console`, `ProgramEditor`, `SerialPortManager` niks weten van de UserInterface klasse (geen relatie.)
    //Door middel van SIGNALS en SLOTS kunnnen deze klassen toch op een nette manier data versturen
    //naar de UserInterface klasse.
    SerialPortManager *singleton_SerialPortManager = SerialPortManager::GetInstance();
    QWidget::connect(singleton_SerialPortManager, SIGNAL(Send(QByteArray&)),
                     &w, SLOT(SerialReceived(QByteArray&)));

    return a.exec();
}
