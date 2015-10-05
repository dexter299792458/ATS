#ifndef CONSOLE_H
#define CONSOLE_H
#include "serialportmanager.h"

class Console
{
public:
    Console();
    void ConvertConsoleLineToSingleACLCommand(QString);
    void SendSingleACLCommandToController(QString);
private:
    SerialPortManager *singleton_SerialPortManager;
};

#endif // CONSOLE_H
