#include "userinterface.h"
#include "ui_userinterface.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QLineEdit>
#include <QString>
#include <QTextCodec>
#include <QFileDialog>
#include <QScrollBar>
#include "enterprogramname.h"

UserInterface::UserInterface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserInterface)
{
    ui->setupUi(this);
    enterprogramname = new EnterProgramName();
    connect(enterprogramname, SIGNAL(accepted()), this, SLOT(ProgramNameReceived()));
    GreyOutMenuItems(STARTUP);
    UserInterface::ScanCOMPorts();
}

UserInterface::~UserInterface()
{
    delete ui;
}

//Maak een verbinding met de seriele poort.
//Laat de gebruiker zien dat de verbinding tot stand is gekomen.
//Maak de functionaliteiten van de GUI beschikbaar (GreyOutMenuItems)
void UserInterface::on_Connect_clicked()
{
    if (ui->ScanPorts->currentText() != "Scan ports...")
    {
        ui->ConnectionStatus->setText("Connected to: " + ui->ScanPorts->currentText());
        ui->ConnectionStatus->setStyleSheet("QLineEdit{background: lightgreen;}");
        singleton_SerialPortManager = SerialPortManager::GetInstance();
        singleton_SerialPortManager->InitializeSerialConnection(ui->ScanPorts->currentText());
        singleton_SerialPortManager->OpenSerialConnection();
        GreyOutMenuItems(CONNECT);
        GreyOutMenuItems(CHOISE_IS_CONSOLE);
        ui->ChoiseConsole->animateClick(0);
    }
}

//Verbreek de verbinding met de seriele poort.
//Laat de gebruiker zien dat de verbinding verbroken is.
//Geef de gebruiker nu alleen de mogelijkheid om weer verbinding te maken (GreyOutMenuItems).
void UserInterface::on_Disconnect_clicked()
{
    if (ui->ScanPorts->currentText() != "Scan ports...")
    {
        singleton_SerialPortManager = SerialPortManager::GetInstance();
        singleton_SerialPortManager->CloseSerialConnection();
        ui->ConnectionStatus->setText("Disconnected from: " + ui->ScanPorts->currentText());
        ui->ConnectionStatus->setStyleSheet("QLineEdit{background: red;}");
        GreyOutMenuItems(DISCONNECT);
    }
}

void UserInterface::on_LoadProgram_clicked()
{
    enterprogramname->show();
    useProgramNameForAction = LOAD;
}

void UserInterface::on_RunProgram_clicked()
{   
    enterprogramname->show();   
    useProgramNameForAction = RUN;
}

void UserInterface::on_ReadProgram_clicked()
{
    enterprogramname->show();  
    useProgramNameForAction = READ;
}

void UserInterface::on_EditProgram_clicked()
{   
    useProgramNameForAction = EDIT;
    ProgramNameReceived();
}

void UserInterface::ProgramNameReceived()
{
    programName = enterprogramname->ReturnEnteredProgramName();
    switch (useProgramNameForAction)
    {
        case LOAD:
            m_ProgramEditor.LoadProgramIntoController(programName, ui->ProgramEditorBox->toPlainText());          
            break;
        case RUN:        
            m_ProgramEditor.RunProgram(programName);
            break;
        case READ:
            m_ProgramEditor.DisplayProgramFromMemory(programName);
            break;
        case EDIT:             
            UserInterface::ConvertProgramToEditable();
            break;
        default:
            break;
    }
}

//Clear de program editor.
void UserInterface::on_ClearScreenProgramEditor_clicked()
{
    ui->ProgramEditorBox->clear();
}

void UserInterface::on_SaveProgram_clicked()
{

}

//Openen van een programma, opgeslagen in een .txt bestand.
//Locatie van de .txt bestanden moet door de gebruiker zelf worden bijgehouden.
void UserInterface::on_OpenProgram_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
        "",tr("Files *.txt"));
       ui->ProgramEditorBox->clear();
       QFile inputFile(fileName);
       if (inputFile.open(QIODevice::ReadOnly))
       {
          QTextStream in(&inputFile);
          while (!in.atEnd())
          {
             QString line = in.readLine();
             ui->ProgramEditorBox->appendPlainText(line);
          }
          inputFile.close();
       }
}

//Clear de console.
void UserInterface::on_ClearScreenConsole_clicked()
{
    ui->ConsoleBox->clear();
}

//Inlezen van de beschikbare COM porten en deze toevoegen aan het drop down menu.
void UserInterface::on_ScanPorts_activated(const QString &arg1)
{
    if(arg1 == "Scan ports...")
    {
         UserInterface::ScanCOMPorts();
    }
}

//Wegschrijven van de ontvangen data van de seriele connectie in de Console of Program Editor
void UserInterface::SerialReceived(QByteArray& s, bool& consoleOrProgramEditor)
{        
    if(consoleOrProgramEditor == true)
    {      
        ui->ConsoleBox->insertPlainText(s);
        ui->ConsoleBox->verticalScrollBar()->setValue(ui->ConsoleBox->verticalScrollBar()->maximum());
    }
    else
    {       
        ui->ProgramEditorBox->insertPlainText(s);
        ui->ProgramEditorBox->verticalScrollBar()->setValue(ui->ProgramEditorBox->verticalScrollBar()->maximum());
    }
}

//Menu items worden onbruikbaar gemaakt voor een bepaalde status van het programma.
void UserInterface::GreyOutMenuItems(int greyOutChoice)
{
    switch ( greyOutChoice )
          {
             case STARTUP || DISCONNECT:
                ui->LoadProgram->setEnabled(false);
                ui->RunProgram->setEnabled(false);
                ui->ReadProgram->setEnabled(false);
                ui->EditProgram->setEnabled(false);
                ui->Disconnect->setEnabled(false);
                ui->ProgramEditorUIGroup->setEnabled(false);
                ui->ConsoleUIGroup->setEnabled(false);
                ui->Connect->setEnabled(true);
                ui->ScanPorts->setEnabled(true);
                ui->ChoiseConsole->setEnabled(false);
                ui->ChoiseProgramEditor->setEnabled(false);
                break;
             case CONNECT:
                ui->LoadProgram->setEnabled(true);
                ui->RunProgram->setEnabled(true);
                ui->ReadProgram->setEnabled(true);
                ui->EditProgram->setEnabled(true);
                ui->Disconnect->setEnabled(true);
                ui->Connect->setEnabled(false);
                ui->ScanPorts->setEnabled(false);
                ui->ProgramEditorUIGroup->setEnabled(true);
                ui->ConsoleUIGroup->setEnabled(true);
                ui->ChoiseConsole->setEnabled(true);
                ui->ChoiseProgramEditor->setEnabled(true);
                break;
            case CHOISE_IS_PROGRAMEDITOR:
                ui->ConsoleUIGroup->setEnabled(false);
                ui->ProgramEditorUIGroup->setEnabled(true);
                ui->LoadProgram->setEnabled(true);
                ui->RunProgram->setEnabled(true);
                ui->ReadProgram->setEnabled(true);
                ui->EditProgram->setEnabled(true);
                break;
            case CHOISE_IS_CONSOLE:
                ui->ProgramEditorUIGroup->setEnabled(false);
                ui->ConsoleUIGroup->setEnabled(true);
                ui->LoadProgram->setEnabled(false);
                ui->RunProgram->setEnabled(false);
                ui->ReadProgram->setEnabled(false);
                ui->EditProgram->setEnabled(false);
             default:
                true;
          }
}

void UserInterface::on_ConsoleLine_returnPressed()
{
    m_Console.ConvertConsoleLineToSingleACLCommand(ui->ConsoleLine->text());
    ui->ConsoleLine->clear();
}

void UserInterface::on_Stop_clicked()
{
    singleton_SerialPortManager = SerialPortManager::GetInstance();
    singleton_SerialPortManager->WriteSingleACLCommand("\x03", WRITE_TO_CONSOLE);
    ui->ConsoleLine->clear();
}

void UserInterface::on_ChoiseProgramEditor_clicked()
{
    GreyOutMenuItems(CHOISE_IS_PROGRAMEDITOR);
    ui->ChoiseConsole->clearMask();
}

void UserInterface::on_ChoiseConsole_clicked()
{
    GreyOutMenuItems(CHOISE_IS_CONSOLE);
    ui->ChoiseProgramEditor->clearMask();
}

//**!!!! Geldt alleen wanneer er eerst een programma is gelezen met de Read Program knop **!!!!//
void UserInterface::ConvertProgramToEditable()
{             
        //lees alle teskt uit de ProgramEditor in
        readAllTextToConvert = ui->ProgramEditorBox->toPlainText();
        ui->ProgramEditorBox->clear();

        //Split de tekst op een enter en plaats dit in een nieuwe lijst.
        splitAllTextBySpaces = readAllTextToConvert.split("\n");

        //Voor alle regels in de lijst, split deze bij een ":"
        for(QString allStringsIn : splitAllTextBySpaces)
        {
            splitAllTextByColon.append(allStringsIn.split(":"));
        }
            //zoek door de hele lijst naar nummers, wanneer nummer gevonden
            //is de volgende cell in de lijst het ACL commando.
            //Voeg het gevonden commando toe aan een nieuwe lijst.
            for(int i = 0; i < splitAllTextByColon.count(); i++)
            {
                splitAllTextByColon[i].toDouble(&isNumeric);
                if(isNumeric == true)
                {
                    onlyPrintACLCommands.append(splitAllTextByColon[i+1]);
                }
            }
                //print de nieuwe lijst in de console.
                for(QString allStringsIn : onlyPrintACLCommands)
                {
                  ui->ProgramEditorBox->insertPlainText(allStringsIn + "\n");
                }
           splitAllTextByColon.clear();
           onlyPrintACLCommands.clear();
}

void UserInterface::on_ExpandProgram_clicked()
{
    QString findCROrQuitCommando = ui->ProgramEditorBox->toPlainText();
    bool checkIfCommandoIsPresence = findCROrQuitCommando.contains("Type <cr> for more");
    if (checkIfCommandoIsPresence == true)
    {
        singleton_SerialPortManager = SerialPortManager::GetInstance();
        singleton_SerialPortManager->WriteSingleACLCommand("cr\x00D", WRITE_TO_PROGRAMEDITOR);
    }
}

void UserInterface::on_QuitProgram_clicked()
{
    QString findCROrQuitCommando = ui->ProgramEditorBox->toPlainText();
    bool checkIfCommandoIsPresence = findCROrQuitCommando.contains("Type <cr> for more");
    if (checkIfCommandoIsPresence == true)
    {
        singleton_SerialPortManager = SerialPortManager::GetInstance();
        singleton_SerialPortManager->WriteSingleACLCommand("Q\x00D", WRITE_TO_PROGRAMEDITOR);
    }
}

void UserInterface::ScanCOMPorts()
{
    QList<QSerialPortInfo> com_ports = QSerialPortInfo::availablePorts();
    QSerialPortInfo port;
    ui->ScanPorts->clear();
    foreach(port, com_ports)
    {
        ui->ScanPorts->addItem(port.portName());
    }
    ui->ScanPorts->addItem("Scan ports...");
}
