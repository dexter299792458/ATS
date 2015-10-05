#ifndef PROGRAMEDITOR_H
#define PROGRAMEDITOR_H
#include "serialportmanager.h"


class ProgramEditor
{
public:
    ProgramEditor();
    void ConvertProgramToSingleACLCommands();
    void LoadProgramIntoController();
    void RunProgram();
    void OpenProgram();
    void SaveProgram();
    void DisplayProgramFromMemory();

private:
    SerialPortManager *singleton_SerialPortManager;
    QByteArray listOfSingleACLCommands;
};

#endif // PROGRAMEDITOR_H
