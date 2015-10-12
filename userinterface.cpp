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

    //Lezen van beschikbare COM porten en wegschrijven in drop-down box.
    QList<QSerialPortInfo> com_ports = QSerialPortInfo::availablePorts();
    QSerialPortInfo port;
    ui->ScanPorts->clear();
    foreach(port, com_ports)
    {
        ui->ScanPorts->addItem(port.portName());
    }
    ui->ScanPorts->addItem("Scan ports...");
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
   enterprogramname->show();
   useProgramNameForAction = EDIT;
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
            MakeProgramFromMemoryEditable = true;
            m_ProgramEditor.DisplayProgramFromMemoryToEdit(programName);
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
         QList<QSerialPortInfo> com_ports = QSerialPortInfo::availablePorts();
         QSerialPortInfo port;
         ui->ScanPorts->clear();
         foreach(port, com_ports)
         {
             ui->ScanPorts->addItem(port.portName());
         }
         ui->ScanPorts->addItem("Scan ports...");
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
    else if (consoleOrProgramEditor == false && MakeProgramFromMemoryEditable == false)
    {       
        ui->ProgramEditorBox->insertPlainText(s);
        ui->ProgramEditorBox->verticalScrollBar()->setValue(ui->ProgramEditorBox->verticalScrollBar()->maximum());
    }
    else if(consoleOrProgramEditor == false && MakeProgramFromMemoryEditable == true )
    {
        //UserInterface::ConvertProgramToEditable(s);
        ui->ProgramEditorBox->insertPlainText(s);
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
    singleton_SerialPortManager->WriteSingleACLCommand("\x03", true);
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

void UserInterface::ConvertProgramToEditable(QByteArray& programFromMemory)
{
        QStringList print;
        QStringList final;
        bool isNumeric;
        QString testje = ui->ProgramEditorBox->toPlainText();
        QStringList testlist = testje.split("\n");
        for(QString strings : testlist)
        {
            print.append(strings.split(":"));
            //print = strings.split(":");
        }
        for(int i = 0; i < print.count(); i++)
        {

            print[i].toDouble(&isNumeric);
            if(isNumeric == true)
            {
                    final.append(print[i+1]);
            }
        }
         ui->ProgramEditorBox->insertPlainText("\n");\
        for(QString s : final)
        {

            ui->ProgramEditorBox->insertPlainText(s + "\n");
        }
}
