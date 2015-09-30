#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H
#include <QMainWindow>

class SerialPortManager : QMainWindow
{
private:
    static SerialPortManager* instance;
     Q_OBJECT
protected:
    SerialPortManager();
public:
    static SerialPortManager* GetInstance();
    void OpenSerialConnection();
    void CloseSerialConnection();
    void WriteSingleACLCommand();
    void WriteMultipleACLCommands();
    void SerialReveiced();

};
#endif // SERIALPORTMANAGER_H