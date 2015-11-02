#include "userinterface.h"
#include "ui_userinterface.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QLineEdit>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QTextCodec>
#include <QFileDialog>
#include <QScrollBar>
#include <ios>
#include <stdio.h>

UserInterface::UserInterface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserInterface)
{
    ui->setupUi(this);
    enterprogramname = new EnterProgramName();
    m_CreateCoordinates = new CreateCoordinates();

    //connect voor koppeling EnterProgramName GUI aan UserInterface GUI
    connect(enterprogramname, SIGNAL(accepted()), this, SLOT(ProgramNameReceived()));

    //Methode voor het `onbruikbaar` maken van bepaalde GUI functionaliteiten aan de hand
    //van de status van het systeem. Zie userinface.h voor de verschillende define's (statussen).
    GreyOutMenuItems(STARTUP);

    //Methode voor het scannen van de COM porten die aanwezig
    UserInterface::ScanCOMPorts();
    erronOnOpen = true;
    consoleHistoryposition = 0;
}

UserInterface::~UserInterface()
{
    delete ui;
}

//BUTTON-CLICK Connect
void UserInterface::on_Connect_clicked()
{
    if (ui->ScanPorts->currentText() != "Scan ports...")
    {
        //Maak verbinding met de seriele poort, eerst wordt de verbinding geinitialiseerd en vervolgens geopend.
        singleton_SerialPortManager = SerialPortManager::GetInstance();
        singleton_SerialPortManager->InitializeSerialConnection(ui->ScanPorts->currentText());
        erronOnOpen = singleton_SerialPortManager->OpenSerialConnection();
        if(erronOnOpen == false)
        {
            singleton_SerialPortManager->CloseSerialConnection();
            ui->ConnectionStatus->setText("Wrong COM port!");
            ui->ConnectionStatus->setStyleSheet("QLineEdit{background: red;}");
            GreyOutMenuItems(DISCONNECT);

        }
        else
        {
            //Laat de gebruiker zien dat de verbinding tot stand is gekomen
            ui->ConnectionStatus->setText("Connected to: " + ui->ScanPorts->currentText());
            ui->ConnectionStatus->setStyleSheet("QLineEdit{background: lightgreen;}");
            //Maak bepaalde GUI functionaliteiten weer beschikbaar voor de gebruiker
            GreyOutMenuItems(CONNECT);
            GreyOutMenuItems(CHOISE_IS_CONSOLE);
            ui->ChoiseConsole->animateClick(0);
        }
    }
}

//BUTTON-CLICK Disconnect
void UserInterface::on_Disconnect_clicked()
{
    if (ui->ScanPorts->currentText() != "Scan ports...")
    {
        //Verbreek de seriele connectie.
        singleton_SerialPortManager = SerialPortManager::GetInstance();
        singleton_SerialPortManager->CloseSerialConnection();

        //Laat de gebruiker zien dat de verbinding gesloten is.
        ui->ConnectionStatus->setText("Disconnected from: " + ui->ScanPorts->currentText());
        ui->ConnectionStatus->setStyleSheet("QLineEdit{background: red;}");

        //De functionaliteiten van de GUI (run program, load program etc.) zijn nu niet meer beschikbaar.
        GreyOutMenuItems(DISCONNECT);
    }
}

//BUTTON-CLICK Load Program
void UserInterface::on_LoadProgram_clicked()
{
    //Naam in titelbalk van pop-up scherm aanpassen
    enterprogramname->setWindowTitle("Load Program");
    //Pop-up scherm voor programmanaam openen
    enterprogramname->show();
    //Integer useProgramNameForAction houdt bij op welke knop er is gedrukt (LOAD, RUN, READ of EDIT)
    useProgramNameForAction = LOAD;
}

//BUTTON-CLICK Run Program
void UserInterface::on_RunProgram_clicked()
{   
    //Naam in titelbalk van pop-up scherm aanpassen
    enterprogramname->setWindowTitle("Run Program");
    //Pop-up scherm voor programmanaam openen
    enterprogramname->show();
   //Integer useProgramNameForAction houdt bij op welke knop er is gedrukt (LOAD, RUN, READ of EDIT)
    useProgramNameForAction = RUN;
}

//BUTTON-CLICK Read Program
void UserInterface::on_ReadProgram_clicked()
{
    //Naam in titelbalk van pop-up scherm aanpassen
    enterprogramname->setWindowTitle("Read Program");
    //Pop-up scherm voor programmanaam openen
    enterprogramname->show();
    //Integer useProgramNameForAction houdt bij op welke knop er is gedrukt (LOAD, RUN, READ of EDIT)
    useProgramNameForAction = READ;
}

//BUTTON-CLICK Edit Program
void UserInterface::on_EditProgram_clicked()
{   
    //Integer useProgramNameForAction houdt bij op welke knop er is gedrukt (LOAD, RUN, READ of EDIT)
    useProgramNameForAction = EDIT;
    ProgramNameReceived();
}

//SLOT voor SIGNAL EnterProgramName is accepted(). Zie constructor.
void UserInterface::ProgramNameReceived()
{
    //Vraag de programmanaam op d.m.v een return (QString)
    programName = enterprogramname->ReturnEnteredProgramName();
    //Aan de hand van de waarde van de integer useProgramNameForAction wordt er een actie uitgevoerd:
    // LOAD, RUN, READ of EDIT

    //Als er geen programmanaam is ingevuld, sla dan de switch over.
    //Er moet geen actie worden uitgevoerd als er geen programmanaam is ingevuld.
    if(programName == nullptr)
    {
        //Sla de switch over.
    }
    else
    {
        switch (useProgramNameForAction)
        {
            case LOAD:
                //Geef aan de methode LoadProgramIntoController het gehele tekstveld van de ProgramEditor mee
                //In dit tekstveld staam alle losse ACL commando's die samen een programma vormen.
                m_ProgramEditor.LoadProgramIntoController(ui->ProgramEditorBox->toPlainText());
                break;
            case RUN:
                //De methode RunProgram krijgt de opgegeven programmanaam mee.
                m_ProgramEditor.RunProgram(programName);
                break;
            case READ:
                //Clear screen voordat er een nieuw programma gelezen wordt.
                ui->ProgramEditorBox->clear();
                //De methode DisplayProgramFromMemory krijgt de opgegeven programmanaam mee.
                m_ProgramEditor.DisplayProgramFromMemory(programName);
                break;
            case EDIT:
                //**EDIT functie werkt alleen wanneer er eerst een READ is uitgevoerd **//
                //**De methode ConvertProgramToEditable converteert een programma     **//
                //**uit het geheugen zodat deze aangepast kan worden                  **//
                UserInterface::ConvertProgramToEditable();
                break;
            default:
                break;
        }
    }
}

//BUTTON-CLICK Clear Screen (Program Editor)
void UserInterface::on_ClearScreenProgramEditor_clicked()
{
    //Clear screen
    ui->ProgramEditorBox->clear();
}

//BUTTON-CLICK Save
void UserInterface::on_SaveProgram_clicked()
{
    //Lees alle tekst in het ProgramEditor veld in. Deel deze tekst op in enkele regels (QStringList).
    QString saveProgramText = ui->ProgramEditorBox->toPlainText();
    QStringList listOfCommands = saveProgramText.split("\n");

    //Opent een file dialog, gebruiker kan een locatie kiezen waar hij/zij het programma wil opslaan
    QString fileName = QFileDialog::getSaveFileName(this, tr("Open File"),
        "",tr("Files *.txt"));
    QFile outputFile(fileName + ".txt");
    {
        if(outputFile.open(QIODevice::ReadWrite))
        {
            QTextStream out(&outputFile);

            //Loop door de lijst van commando's heen en schrijft deze naar
            //de .txt file. De "\r\n" is nodig om een nieuwe regel toe te voegen
            //in de txt file, een "\n" of endl; werkt niet!!!
            for(int i = 0; i < listOfCommands.count(); i++)
            {
               out << listOfCommands[i] << "\r\n";
           }
        }
    }

}

//BUTTON-CLICK Open
void UserInterface::on_OpenProgram_clicked()
{
    //Opent een file dialog, gebruiker selecteert een .txt bestand.
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
        "",tr("Files *.txt"));
       ui->ProgramEditorBox->clear();
       QFile inputFile(fileName);
       if (inputFile.open(QIODevice::ReadOnly))
       {
          QTextStream in(&inputFile);
          //Zolang het eind van de .txt file niet bereikt is: lees een regel in en print deze in
          //het ProgramEditor veld.
          while (!in.atEnd())
          {
             QString line = in.readLine();
             ui->ProgramEditorBox->appendPlainText(line);
          }
          inputFile.close();
       }
}

//BUTTON-CLICK Clear Screen (Console)
void UserInterface::on_ClearScreenConsole_clicked()
{
    //Clear screen
    ui->ConsoleBox->clear();
}

//Inlezen van de beschikbare COM porten en deze toevoegen aan het drop down menu in de UserInterface.
void UserInterface::on_ScanPorts_activated(const QString &arg1)
{
    if(arg1 == "Scan ports...")
    {
         UserInterface::ScanCOMPorts();
    }
}

//SLOT voor SIGNAL Send() -> zie main.cpp
//Wegschrijven van de ontvangen data van de seriele connectie in de Console of Program Editor.
//Keuze wordt gemaakt aan de hand van de bool consoleOrProgramEditor.
void UserInterface::SerialReceived(QByteArray& s, bool& consoleOrProgramEditor)
{        
    if(consoleOrProgramEditor == WRITE_TO_CONSOLE)
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
//STARTUP                       = GUI wordt geladen
//DISCONNECT                    = Gebruiker drukt op disconnect knop
//CONNECT                       = Gebruiker drukt op connect knop
//CHOISE_IS_PROGRAMEDITOR       = radiobutton voor ProgramEditor is gekozen
//CHOISE_IS_CONSOLE             = radiobutton voor console is gekozen
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

void UserInterface::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Up:
        if(ui->ConsoleLine->hasFocus())
        {
            if(!consoleHistory.empty())
            {
                if(ui->ConsoleLine->text() == consoleHistory[consoleHistoryposition] && consoleHistoryposition < consoleHistory.count()-1)
                {
                        consoleHistoryposition++;
                }
                ui->ConsoleLine->setText(consoleHistory[consoleHistoryposition]);
            }
        }
            break ;
    case Qt::Key_Down:
        if(ui->ConsoleLine->hasFocus())
        {
            if(!consoleHistory.empty())
            {
                if(ui->ConsoleLine->text() == consoleHistory[consoleHistoryposition] && consoleHistoryposition > 0)
                {
                    consoleHistoryposition--;
                }
                ui->ConsoleLine->setText(consoleHistory[consoleHistoryposition]);
            }
        }
            break ;
    default:
        break ;
    }
}

//Wanneer er in de Console op `enter` wordt gedrukt:
void UserInterface::on_ConsoleLine_returnPressed()
{
    //Lees de tekst in een verstuur deze naar de Console klasse
    m_Console.ConvertConsoleLineToSingleACLCommand(ui->ConsoleLine->text());
    consoleHistory.push_front(ui->ConsoleLine->text());
    ui->ConsoleLine->clear();
    consoleHistoryposition = 0;
}

//BUTTON-CLICK Stop
void UserInterface::on_Stop_clicked()
{
    //Verstuurt CTRL-C over de verbinding om de controller te kunnen onderbreken (stoppen)
    singleton_SerialPortManager = SerialPortManager::GetInstance();
    singleton_SerialPortManager->WriteSingleACLCommand("\x03", WRITE_TO_CONSOLE);
    ui->ConsoleLine->clear();
}

//RADIOBUTTON ProgramEditor is clicked
void UserInterface::on_ChoiseProgramEditor_clicked()
{
    //Alleen de ProgramEditor kan gebruikt worden, geen Console.
    GreyOutMenuItems(CHOISE_IS_PROGRAMEDITOR);
    ui->ChoiseConsole->clearMask();
}

//RADIOBUTTON Console is clicked
void UserInterface::on_ChoiseConsole_clicked()
{
    //Aleen de Console kan gebruikt worden, geen ProgramEditor
    GreyOutMenuItems(CHOISE_IS_CONSOLE);
    ui->ChoiseProgramEditor->clearMask();
}

//**!!!! Geldt alleen wanneer er eerst een programma is gelezen met de Read Program knop **!!!!//
//BUTTON-CLICK Edit Program
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

//BUTTON-CLICK Send <CR>
void UserInterface::on_ExpandProgram_clicked()
{
    //Waneer de gebruiker in de ProgramEditor aan het werk is en een programma opvraagd (Read Program)
    //kan het zo zijn dat het programa zo groot is dat de controller vraag om een `enter` om de rest
    //van het programma te kunnen laten zien. Hiervoor is een knop in de ProgramEditor toegevoegd.
    //Hetzelfde geldt voor Quit, het lezen van het programma moet ook onderbroken kunnen worden.
    //Hiervoor dient de knop `Send <Q>`
    QString findCROrQuitCommando = ui->ProgramEditorBox->toPlainText();
    bool checkIfCommandoIsPresence = findCROrQuitCommando.contains("Type <cr> for more");
    if (checkIfCommandoIsPresence == true)
    {
        singleton_SerialPortManager = SerialPortManager::GetInstance();
        singleton_SerialPortManager->WriteSingleACLCommand("cr\x00D", WRITE_TO_PROGRAMEDITOR);
    }
}

//BUTTON-CLICK Send <Q>
//Zie commentaar bij BUTTON-CLICK Send <CR>
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

//Scan welke COM porten er aanwezig zijn op het systeem en plaats deze in een dropdown menu.
void UserInterface::ScanCOMPorts()
{
    QList<QSerialPortInfo> com_ports = QSerialPortInfo::availablePorts();
    QSerialPortInfo port;
    ui->ScanPorts->clear();
    foreach(port, com_ports)
    {
        ui->ScanPorts->addItem(port.portName());
    }
    //Geef de gebruiker de mogelijkheid om opnieuw te scannen.
    //Dit wordt afgevangen in on_ScanPorts_activated() in de UserInterface klasse.
    ui->ScanPorts->addItem("Scan ports...");
}

//BUTTON-CLICK Create coördinates
void UserInterface::on_CreateCoordinates_clicked()
{
    //Laat het scherm voor het aanmaken van een coördinaat zien.
    m_CreateCoordinates->show();
}


