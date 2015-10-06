#include "serialportmanager.h"
#include <QSerialPort>
#include <QFileDialog>
#include <QTextStream>
#include <ios>
#include <stdio.h>
#include <QStringList>
#include <QSerialPortInfo>
#include <QString>
#include <QDebug>
#include "userinterface.h"

//Class members
SerialPortManager* SerialPortManager::instance = 0;
QSerialPort *serialport;

//Singleton constructor zorgt voor de RS232 initialisatie
SerialPortManager::SerialPortManager()
{
    serialport = new QSerialPort();
    serialport->setPortName("COM1");
    serialport->setBaudRate(QSerialPort::Baud9600);
    serialport->setDataBits(QSerialPort::Data8);
    serialport->setParity(QSerialPort::NoParity);
    serialport->setStopBits(QSerialPort::OneStop);
    serialport->setFlowControl(QSerialPort::NoFlowControl);
}

//Singleton implementatie
SerialPortManager* SerialPortManager::GetInstance()
{
    if(instance == 0)
    {
       instance = new SerialPortManager;
    }
    return instance;
}

//Open de seriele connectie en connect
void SerialPortManager::OpenSerialConnection()
{
    serialport->open(QIODevice::ReadWrite);
    QSerialPort::connect(serialport,SIGNAL(readyRead()),this,SLOT(GiveReceivedDataToUI()));
}

void SerialPortManager::CloseSerialConnection()
{
    serialport->close();
}

void SerialPortManager::WriteMultipleACLCommands(){}
void SerialPortManager::WriteSingleACLCommand(QString ACLCommand)
{
    serialport->write(ACLCommand.toLatin1());
}

void SerialPortManager::GiveReceivedDataToUI()
{
    QByteArray s = serialport->readAll();
    emit Send(s);
}


