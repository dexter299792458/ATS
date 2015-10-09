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
    serialport->setFlowControl(QSerialPort::HardwareControl);
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

void SerialPortManager::WriteMultipleACLCommands(QStringList receivedACLCommands)
{
    //QString enter = "PRLNCOM COM1 edit homo @coff @con exit";
    QString receive = "receive\x20homo\x00D";
    QByteArray ss = receive.toLatin1();
    serialport->write(ss);
    serialport->waitForReadyRead(5000);
    QByteArray s;
    //char *colon = ":";
    //char *jemoeder = ";";
    //QByteArray jemoer = serialport->readAll();
    for(int i = 0; i < receivedACLCommands.count(); i++ )
    {
        s = receivedACLCommands[i].toLatin1();
        serialport->write(s);



        //serialport->waitForReadyRead(500);
        //while(jemoeder != ":")
        //{
        //    serialport->getChar(jemoeder);
        //}
    }
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
    s.append(serialport->readAll());
    qDebug() << s;
    //qDebug() << serialport->ClearToSendSignal;
    emit Send(s, DataToConsoleOrProgramEditor);
}

void SerialPortManager::WaitForCommandIsProcessed()
{
    serialport->waitForReadyRead(5000);
}
