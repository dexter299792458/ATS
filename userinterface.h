#ifndef USERINTERFACE_H
#define USERINTERFACE_H
#define STARTUP 0
#define CONNECT 1
#define DISCONNECT 0
#define RUN 10
#define LOAD 20
#define READ 30
#include "serialportmanager.h"
#include "console.h"
#include "programeditor.h"
#include <QMainWindow>
#include <QListWidget>
#include "enterprogramname.h"

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

private:
    Ui::UserInterface *ui;
    SerialPortManager *singleton_SerialPortManager;
    Console m_Console;
    ProgramEditor m_ProgramEditor;
    void GreyOutMenuItems(int);
    EnterProgramName *enterprogramname;
    QString programName;
    int useProgramNameForAction;
};

#endif // USERINTERFACE_H
