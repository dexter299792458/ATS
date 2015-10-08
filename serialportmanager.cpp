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

//Singleton constructor zorgt voor de RS232 initialisatie
SerialPortManager::SerialPortManager()
{
    serialport = new QSerialPort();
    DataToConsoleOrProgramEditor = false;
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

//Initialisatie van de seriele connectie
void SerialPortManager::InitializeSerialConnection(QString connectToPort)
{
    serialport->setPortName(connectToPort);
    serialport->setBaudRate(QSerialPort::Baud9600);
    serialport->setDataBits(QSerialPort::Data8);
    serialport->setParity(QSerialPort::NoParity);
    serialport->setStopBits(QSerialPort::OneStop);
    serialport->setFlowControl(QSerialPort::NoFlowControl);
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

void SerialPortManager::WriteMultipleACLCommands()
{

}

void SerialPortManager::WriteSingleACLCommand(QString ACLCommand, bool RequestFromConsoleOrProgramEditor)
{
    //****!!! IF DataToConsoleOrProgramEditor == TRUE -> Write to Console ****!!!!

    DataToConsoleOrProgramEditor = RequestFromConsoleOrProgramEditor;
    serialport->write(ACLCommand.toLatin1());
}

void SerialPortManager::GiveReceivedDataToUI()
{
    QByteArray s;
   // bool b = true;

    //QByteArray loading = "Loading.....";
    //emit Send(loading);
   // while(serialport->bytesAvailable() > 0 && b == true )
    //{
       // b = serialport->waitForReadyRead();
        s.append(serialport->readAll());
   // }
    emit Send(s, DataToConsoleOrProgramEditor);
    //QByteArray ss = "\n";
    //emit Send(ss);
}


