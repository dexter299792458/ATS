#include "userinterface.h"
#include "ui_userinterface.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QLineEdit>
#include <QFileDialog>
#include <QScrollBar>

UserInterface::UserInterface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserInterface)
{
    ui->setupUi(this);
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

}

void UserInterface::on_RunProgram_clicked()
{

}

void UserInterface::on_ReadProgram_clicked()
{

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

//Versturen van het commando dat ingetypt is in de console.
void UserInterface::on_SendConsole_clicked()
{
    QString allText = ui->ConsoleBox->toPlainText();
    QStringList splitAllText = allText.split("\n");
    QString GetLastLine = splitAllText.last();   
    m_Console.ConvertConsoleLineToSingleACLCommand(GetLastLine);
    ui->ConsoleBox->clear();
}

//Wegschrijven van de ontvangen data van de seriele connectie in de Console of Program Editor
void UserInterface::SerialReceived(QByteArray& s)
{        
    ui->ConsoleBox->insertPlainText(s);
    ui->ConsoleBox->verticalScrollBar()->setValue(ui->ConsoleBox->verticalScrollBar()->maximum());
}

//Menu items worden onbruikbaar gemaakt voor een bepaalde status van het programma.
void UserInterface::GreyOutMenuItems(int greyOutChoice)
{
    switch ( greyOutChoice )
          {
             case 0:
                ui->LoadProgram->setEnabled(false);
                ui->RunProgram->setEnabled(false);
                ui->ReadProgram->setEnabled(false);
                ui->Disconnect->setEnabled(false);
                ui->ProgramEditorUIGroup->setEnabled(false);
                ui->ConsoleUIGroup->setEnabled(false);
                ui->Connect->setEnabled(true);
                ui->ScanPorts->setEnabled(true);
                break;
             case 1:
                ui->LoadProgram->setEnabled(true);
                ui->RunProgram->setEnabled(true);
                ui->ReadProgram->setEnabled(true);
                ui->Disconnect->setEnabled(true);
                ui->Connect->setEnabled(false);
                ui->ScanPorts->setEnabled(false);
                ui->ProgramEditorUIGroup->setEnabled(true);
                ui->ConsoleUIGroup->setEnabled(true);
                break;
             default:
                true;
          }
}
