#ifndef USERINTERFACE_H
#define USERINTERFACE_H
#include "serialportmanager.h"
#include "console.h"
#include "programeditor.h"
#include <QMainWindow>

namespace Ui {
class UserInterface;
}

class UserInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserInterface(QWidget *parent = 0);
    QByteArray testje;
    ~UserInterface();

public slots:
    void SerialReceived(QByteArray& s);

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
    void on_SendConsole_clicked();

private:
    Ui::UserInterface *ui;
    SerialPortManager *singleton_SerialPortManager;
    Console m_Console;
    ProgramEditor m_ProgramEditor;
};

#endif // USERINTERFACE_H
