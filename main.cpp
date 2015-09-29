#include "userinterface.h"
#include <QApplication>
#include "serialportmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UserInterface w;
    w.show();    
    return a.exec();
}
