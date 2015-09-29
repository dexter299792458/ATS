#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QMainWindow>

namespace Ui {
class UserInterface;
}

class UserInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserInterface(QWidget *parent = 0);
    ~UserInterface();

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

private:
    Ui::UserInterface *ui;
};

#endif // USERINTERFACE_H
