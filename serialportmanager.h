#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H
#include <QMainWindow>
#include <QSerialPort>
#define DATA_SET_READY 0x10
#define REMOVE_PROGRAM_ACL 1

class SerialPortManager : public QObject
{
     Q_OBJECT
private:    
    static SerialPortManager* instance;
    QSerialPort *serialport;
    bool DataToConsoleOrProgramEditor;
    int checkForDataSetReady;
protected:
    SerialPortManager();
public:
    static SerialPortManager* GetInstance();
    bool OpenSerialConnection();
    void CloseSerialConnection();
    void WriteSingleACLCommand(QString, bool);
    void WriteMultipleACLCommands(QStringList, bool);
    void InitializeSerialConnection(QString);

public slots:
    void GiveReceivedDataToUI();

signals:
    void Send(QByteArray& s, bool& consoleOrProgramEditor);

};
#endif // SERIALPORTMANAGER_H
