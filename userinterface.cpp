#include "userinterface.h"
#include "ui_userinterface.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QLineEdit>
#include <QFileDialog>

UserInterface::UserInterface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserInterface)
{
    ui->setupUi(this);

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

void UserInterface::on_Connect_clicked()
{
    if (ui->ScanPorts->currentText() != "Scan ports...")
    {
        singleton_SerialPortManager = SerialPortManager::GetInstance();
        singleton_SerialPortManager->OpenSerialConnection();
        ui->ConnectionStatus->setText("Connected to: " + ui->ScanPorts->currentText());
        ui->ConnectionStatus->setStyleSheet("QLineEdit{background: lightgreen;}");
    }
}

void UserInterface::on_Disconnect_clicked()
{
    if (ui->ScanPorts->currentText() != "Scan ports...")
    {
        singleton_SerialPortManager = SerialPortManager::GetInstance();
        singleton_SerialPortManager->CloseSerialConnection();
        ui->ConnectionStatus->setText("Disconnected from: " + ui->ScanPorts->currentText());
        ui->ConnectionStatus->setStyleSheet("QLineEdit{background: red;}");
    }
}

void UserInterface::on_LoadProgram_clicked()
{

}

void UserInterface::on_RunProgram_clicked()
{
   //QString tekst = ui->plainTextEdit->toPlainText();
   //QStringList regels = tekst.split("\n");
   //QByteArray naampie = (regels[i].toLatin1() + "\x00D");
   //serial->write(naampie);
}

void UserInterface::on_ReadProgram_clicked()
{

}

void UserInterface::on_ClearScreenProgramEditor_clicked()
{
    ui->ProgramEditorBox->clear();
}

void UserInterface::on_SaveProgram_clicked()
{

}

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

void UserInterface::on_ClearScreenConsole_clicked()
{
    ui->ConsoleBox->clear();
}

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

void UserInterface::on_SendConsole_clicked()
{
    QString allText = ui->ConsoleBox->toPlainText();
    QStringList splitAllText = allText.split("\n");
    QString GetLastLine = splitAllText.last();
    ui->ConsoleBox->appendPlainText(GetLastLine);
    m_Console.ConvertConsoleLineToSingleACLCommand(GetLastLine);
}

void UserInterface::SerialReceived(QByteArray received)
{
    singleton_SerialPortManager = SerialPortManager::GetInstance();
    received = singleton_SerialPortManager->GiveReceivedDataToUI();
    ui->ConsoleBox->insertPlainText(received);
}
