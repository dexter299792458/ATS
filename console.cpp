#include "console.h"


Console::Console()
{             
}


//Methode plakt een hexadecimale `enter` achter de huidige data in de QString
//anders wordt het commando niet verzonden!
void Console::ConvertConsoleLineToSingleACLCommand(QString ACLCommand)
{
    SendSingleACLCommandToController(ACLCommand + "\x00D");
}

//Stuurt het te verzenden commando naar de SerialPortManager, de functie
//WriteSingleACLCommand is verantwoordelijk voor het daadwerkelijk versturen van
//de data naar de controller. Aanvraag wordt gedaan vanuit de Console (define WRITE_TO_CONSOLE)
void Console::SendSingleACLCommandToController(QString ACLCommand)
{
    singleton_SerialPortManager = SerialPortManager::GetInstance();
    singleton_SerialPortManager->WriteSingleACLCommand(ACLCommand, WRITE_TO_CONSOLE);
}
