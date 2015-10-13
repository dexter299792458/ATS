#ifndef USERINTERFACE_H
#define USERINTERFACE_H
#define STARTUP 0
#define CONNECT 1
#define DISCONNECT 0
#define CHOISE_IS_PROGRAMEDITOR 2
#define CHOISE_IS_CONSOLE 3
#define RUN 10
#define LOAD 20
#define READ 30
#define EDIT 40
#define WRITE_TO_CONSOLE true
#define WRITE_TO_PROGRAMEDITOR false

#include "serialportmanager.h"
#include "console.h"
#include "programeditor.h"
#include <QMainWindow>
#include <QListWidget>
#include "enterprogramname.h"
#include "createcoordinates.h"

namespace Ui {
class UserInterface;
}

class UserInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserInterface(QWidget *parent = 0);
    ~UserInterface();

public slots:
    void SerialReceived(QByteArray& s, bool &consoleOrProgramEditor);
    void ProgramNameReceived();
    void CreateCoordinate();

private slots:
    void on_Connect_clicked();

    void on_Disconnect_clicked();

    void on_LoadProgram_clicked();

    void on_RunProgram_clicked();

    void on_ReadProgram_clicked();

    void on_ClearScreenProgramEditor_clicked();

    void on_SaveProgram_clicked();

    void on_OpenProgram_clicked();

    void on_ClearScreenConsole_clicked();

    void on_ScanPorts_activated(const QString &arg1); 

    void on_ConsoleLine_returnPressed();

    void on_Stop_clicked();

    void on_ChoiseProgramEditor_clicked();

    void on_ChoiseConsole_clicked();

    void on_EditProgram_clicked();

    void on_ExpandProgram_clicked();

    void on_QuitProgram_clicked();

    void on_CreateCoordinates_clicked();

private:
    Ui::UserInterface *ui;
    SerialPortManager *singleton_SerialPortManager;
    Console m_Console;
    ProgramEditor m_ProgramEditor;
    void GreyOutMenuItems(int);
    void ConvertProgramToEditable();
    void ScanCOMPorts();
    EnterProgramName *enterprogramname;
    QString programName;
    int useProgramNameForAction;
    QStringList splitAllTextByColon;
    QStringList onlyPrintACLCommands;
    bool isNumeric;
    QString readAllTextToConvert;
    QStringList splitAllTextBySpaces;
    CreateCoordinates *m_CreateCoordinates;

};

#endif // USERINTERFACE_H
