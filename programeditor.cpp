#include "programeditor.h"
#include <QDebug>

ProgramEditor::ProgramEditor()
{
}

void ProgramEditor::DisplayProgramFromMemory(QString showProgam)
{
    singleton_SerialPortManager = SerialPortManager::GetInstance();
    singleton_SerialPortManager->WriteSingleACLCommand("list\x20" + showProgam + "\x00D", false);
}

void ProgramEditor::LoadProgramIntoController(QString loadProgram, QString programData)
{
    listConversion = programData.split("\n");
    for(int i = 0; i < listConversion.count(); i++)
    {
        listConversion[i] = listConversion[i] + "\x00D";
        qDebug() << listConversion[i];
    }
    qDebug() << listConversion;
    singleton_SerialPortManager = SerialPortManager::GetInstance();
    singleton_SerialPortManager->WriteMultipleACLCommands(listConversion);

}

void ProgramEditor::RunProgram(QString runProgram)
{
    singleton_SerialPortManager = SerialPortManager::GetInstance();
    singleton_SerialPortManager->WriteSingleACLCommand("run\x20" + runProgram + "\x00D", false);
}

void ProgramEditor::ConvertProgramToSingleACLCommands(){}

void ProgramEditor::DisplayProgramFromMemoryToEdit(QString editProgram)
{
    singleton_SerialPortManager = SerialPortManager::GetInstance();
    singleton_SerialPortManager->WriteSingleACLCommand("list\x20" + editProgram + "\x00D", false);
}
