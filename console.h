#ifndef CONSOLE_H
#define CONSOLE_H
#define WRITE_TO_CONSOLE true
#define WRITE_TO_PROGRAMEDITOR false

#include "serialportmanager.h"

class Console
{
public:
    Console();
    void ConvertConsoleLineToSingleACLCommand(QString);

private:
    SerialPortManager *singleton_SerialPortManager;
    void SendSingleACLCommandToController(QString);
};

#endif // CONSOLE_H
