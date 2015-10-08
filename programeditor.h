#ifndef PROGRAMEDITOR_H
#define PROGRAMEDITOR_H
#include "serialportmanager.h"


class ProgramEditor
{
public:
    ProgramEditor();
    void ConvertProgramToSingleACLCommands();
    void LoadProgramIntoController(QString);
    void RunProgram(QString);
    void DisplayProgramFromMemory(QString);

private:
    SerialPortManager *singleton_SerialPortManager;
    QByteArray listOfSingleACLCommands;
};

#endif // PROGRAMEDITOR_H
