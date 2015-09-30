#include "userinterface.h"
#include "ui_userinterface.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QLineEdit>
#include "serialportmanager.h"



//Class member
SerialPortManager *m_SerialPortManager;


UserInterface::UserInterface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserInterface)
{
    ui->setupUi(this);
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
        m_SerialPortManager = SerialPortManager::GetInstance();
        m_SerialPortManager->OpenSerialConnection();
        ui->ConnectionStatus->setText("Connected to: " + ui->ScanPorts->currentText());
        ui->ConnectionStatus->setStyleSheet("QLineEdit{background: lightgreen;}");
    }
}

void UserInterface::on_Disconnect_clicked()
{
    if (ui->ScanPorts->currentText() != "Scan ports...")
    {
        m_SerialPortManager = SerialPortManager::GetInstance();
        m_SerialPortManager->CloseSerialConnection();
        ui->ConnectionStatus->setText("Disconnected from: " + ui->ScanPorts->currentText());
        ui->ConnectionStatus->setStyleSheet("QLineEdit{background: red;}");
    }
}

void UserInterface::on_LoadProgram_clicked()
{

}

void UserInterface::on_RunProgram_clicked()
{
   // QString tekst = ui->plainTextEdit->toPlainText();
   // QStringList regels = tekst.split("\n");
   // QByteArray naampie = (regels[i].toLatin1() + "\x00D");
   // serial->write(naampie);
}

void UserInterface::on_ReadProgram_clicked()
{

}

void UserInterface::on_ClearScreenProgramEditor_clicked()
{

}

void UserInterface::on_SaveProgram_clicked()
{

}

void UserInterface::on_OpenProgram_clicked()
{

}

void UserInterface::on_ClearScreenConsole_clicked()
{

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
