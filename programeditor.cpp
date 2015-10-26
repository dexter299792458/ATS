#include "programeditor.h"
#include <QDebug>

ProgramEditor::ProgramEditor()
{
}

//Stuurt een enkel ACL commando naar de SerialPortManager klasse
//De aanvraag komt vanaf de ProgramEditor en moet een programma
//uit het geheugen laten zien (list [naam])
void ProgramEditor::DisplayProgramFromMemory(QString showProgam)
{
    singleton_SerialPortManager = SerialPortManager::GetInstance();
    singleton_SerialPortManager->WriteSingleACLCommand
            ("list\x20" + showProgam + "\x00D", WRITE_TO_PROGRAMEDITOR);
}

//Methode krijgt vanuit de UserInterface alle tekst uit het ProgamEdior veld mee
//eerst wordt dit hele tekstveld omgezet naar een lijst van enkele ACL commando's
//zie hiervoor de functie ConvertProgramToSingleACLCommands().
//Vervolgens zal de methode WriteMultipleACLCommands het versturen van de ACL commando's afhandelen.
//Let op dat hier een QStringList wordt meegegeven en geen QByteArray!
void ProgramEditor::LoadProgramIntoController(QString programData)
{
    ProgramEditor::ConvertProgramToSingleACLCommands(programData);
    singleton_SerialPortManager = SerialPortManager::GetInstance();
    singleton_SerialPortManager->WriteMultipleACLCommands(listConversion, WRITE_TO_PROGRAMEDITOR);
}

//Stuurt een enkel ACL commando naar de SerialPortManager, aanvraag van ProgramEditor.
//Methode stuurt een verzoek voor het uitvoeren van een programma (run + naam).
void ProgramEditor::RunProgram(QString runProgram)
{
    singleton_SerialPortManager = SerialPortManager::GetInstance();
    singleton_SerialPortManager->WriteSingleACLCommand
            ("run\x20" + runProgram + "\x00D", WRITE_TO_PROGRAMEDITOR);
}

//Alle tekst uit het ProgramEditor veld in de UserInterface wordt ontvangen in een QString.
//Deze tekst wordt opgedeeld in enkele QStrings's, vandaar de QStringList.
//De tekst wordt door middel van het split("\n") commando opgedeeld in enkele QString's.
//Elke individuele QString krijgt een hexadecimale `enter` achter zijn huidige data geplakt
//anders worden de commando's niet verzonden.
void ProgramEditor::ConvertProgramToSingleACLCommands(QString convertedProgramData)
{
    listConversion = convertedProgramData.split("\n");
    for(int i = 0; i < listConversion.count(); i++)
    {
        listConversion[i] = listConversion[i] + "\x00D";
    }
}

