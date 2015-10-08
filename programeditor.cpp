#include "programeditor.h"

ProgramEditor::ProgramEditor()
{
}

void ProgramEditor::DisplayProgramFromMemory(QString showProgam)
{
    singleton_SerialPortManager = SerialPortManager::GetInstance();
    singleton_SerialPortManager->WriteSingleACLCommand("list\x20" + showProgam + "\x00D");
}

void ProgramEditor::LoadProgramIntoController(QString){}
void ProgramEditor::RunProgram(QString){}
