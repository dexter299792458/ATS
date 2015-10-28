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
#include <QMessageBox>
#include "userinterface.h"

//** Set de instance_flag van de singleton op 0! **//
SerialPortManager* SerialPortManager::instance = 0;

//**Singleton constructor zorgt voor het eenmalig aanmaken van een QSerialPort! **//
SerialPortManager::SerialPortManager()
{
    serialport = new QSerialPort();
    DataToConsoleOrProgramEditor = false;
}

//Singleton implementatie -> Nog geen instantie van SerialPortManager is nieuw object aanmaken.
//Wanneer er op de eerste aanvraag van GetInstance() een instantie gemaakt is, krijgen alle
//volgende aanvragen diezelfde instantie terug.
SerialPortManager* SerialPortManager::GetInstance()
{
    if(instance == 0)
    {
       instance = new SerialPortManager;
    }
    return instance;
}

//Initialisatie van de seriele connectie
//Methode krijgt de gekozen COM poort uit het dropdown menu van de UserInterface mee in een QString.
void SerialPortManager::InitializeSerialConnection(QString connectToPort)
{
    serialport->setPortName(connectToPort);
    serialport->setBaudRate(QSerialPort::Baud9600);
    serialport->setDataBits(QSerialPort::Data8);
    serialport->setParity(QSerialPort::NoParity);
    serialport->setStopBits(QSerialPort::OneStop);
    serialport->setFlowControl(QSerialPort::SoftwareControl);
}

//Opent de seriele connectie.
//Het connect commando verbindt het readyRead() signaal aan een zelf gedefinieerd SLOT.
//readyRead() signaal wordt true (emit) wanneer er nieuwe data aanwezig is op de verbinding.
//data komt binnen op het gedefinieerde SLOT.
//** LET OP: readyRead() signaal emit meerdere keren!! bijvoorbeeld bij het commando `list` **//
//** Er is geen eind aan de datastroom te onderscheiden!! **//
bool SerialPortManager::OpenSerialConnection()
{
    serialport->open(QIODevice::ReadWrite);
    QSerialPort::connect(serialport,SIGNAL(readyRead()),this,SLOT(GiveReceivedDataToUI()));

    //Check for DSR serial signal (Data Set Ready)
    //DSR signaal = TRUE juiste COM port gekozen
    if(DATA_SET_READY != (DATA_SET_READY & serialport->pinoutSignals()))
    {
        QMessageBox msgBox;
        msgBox.setText("The SCOTBOT controller is not connected to this COM port!");
        msgBox.exec();
        return false;
    }

    //Verkeerde COM port gekozen
    else
    {
        return true;
    }
}

//Sluiten van de seriele connectie.
void SerialPortManager::CloseSerialConnection()
{
    serialport->close();
}

//Methode verstuurt meerdere ACL commando's achter elkaar.
//waitForReadyRead(500) is nodig om de controller de tijd te geven om de commando's te verwerken
//voordat het volgende commando verstuurd kan worden.
//** De bool DataToConsoleOrProgramEditor slaat op vanaf waar er een ACL verstuurd is. **//
//** Dit kan zijn vanaf de Console of vanaf de ProgramEditor                           **//
//** Wanneer er vanaf de Console een ACL verstuurd wordt, wil je dat de onvangen data  **//
//** ook in het Console veld geprint wordt. Zelfde geldt voor de ProgramEditor.        **//
void SerialPortManager::WriteMultipleACLCommands(QStringList receivedACLCommands, bool RequestFromConsoleOrProgramEditor)
{
    //Bool DataToConsoleOrProgramEditor wordt bijgewerkt aan de hand van het verzoek!
    DataToConsoleOrProgramEditor = RequestFromConsoleOrProgramEditor;
    QByteArray s;
    for(int i = 0; i < receivedACLCommands.count(); i++ )
    {
        s = receivedACLCommands[i].toLatin1();
        serialport->write(s);
        serialport->waitForReadyRead(500);
    }
}

//Het ontvangen (enkele) ACL commando wordt verzonden over de seriele connectie.
//De conversie toLatin1() is nodig om het juiste dataformaat te krijgen wat over de seriele verbinding mag
void SerialPortManager::WriteSingleACLCommand(QString ACLCommand, bool RequestFromConsoleOrProgramEditor)
{
    //Bool DataToConsoleOrProgramEditor wordt bijgewerkt aan de hand van het verzoek!
    DataToConsoleOrProgramEditor = RequestFromConsoleOrProgramEditor;
    serialport->write(ACLCommand.toLatin1());
}

//public SLOT --> zie connect in OpenSerialConnection in SerialPortManager.
//Als er nieuwe data op de verbinding staat die ingelezen kan worden wordt deze methode aangeroepen.
//Vervolgens is er zelf een SIGNAL gedefinieerd namelijk Send();
//Dit signaal wordt gebruikt om de ontvangen data terug te sturen naar de UserInterface en deze data
//vervolgens in de Console of ProgramEditor te printen (bool DataToConsoleOrProgramEditor)
//De connect tussen de SerialPortManager en UserInterface is terug te vinden in main.cpp
void SerialPortManager::GiveReceivedDataToUI()
{
    QByteArray s;
    s.append(serialport->readAll());
    emit Send(s, DataToConsoleOrProgramEditor);
}


