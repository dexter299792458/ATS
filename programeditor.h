#ifndef PROGRAMEDITOR_H
#define PROGRAMEDITOR_H
#define WRITE_TO_CONSOLE true
#define WRITE_TO_PROGRAMEDITOR false

#include "serialportmanager.h"


class ProgramEditor
{
public:
    ProgramEditor();
    void ConvertProgramToSingleACLCommands();
    void LoadProgramIntoController(QString);
    void RunProgram(QString);
    void DisplayProgramFromMemory(QString);
    void DisplayProgramFromMemoryToEdit(QString);

private:
    SerialPortManager *singleton_SerialPortManager;
    QStringList listConversion;
};

#endif // PROGRAMEDITOR_H
