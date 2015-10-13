#include "console.h"


Console::Console()
{             
}

void Console::ConvertConsoleLineToSingleACLCommand(QString ACLCommand)
{
    SendSingleACLCommandToController(ACLCommand + "\x00D");
}

void Console::SendSingleACLCommandToController(QString ACLCommand)
{
    singleton_SerialPortManager = SerialPortManager::GetInstance();
    singleton_SerialPortManager->WriteSingleACLCommand(ACLCommand, WRITE_TO_CONSOLE);
}
