#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H
#include <QMainWindow>
#include <QSerialPort>

class SerialPortManager : public QObject
{
     Q_OBJECT
private:    
    static SerialPortManager* instance;
    QSerialPort *serialport;
    bool DataToConsoleOrProgramEditor;
protected:
    SerialPortManager();
public:
    static SerialPortManager* GetInstance();
    void OpenSerialConnection();
    void CloseSerialConnection();
    void WriteSingleACLCommand(QString, bool);
    void WriteMultipleACLCommands(QStringList);
    void InitializeSerialConnection(QString);
    void WaitForCommandIsProcessed();

public slots:
    void GiveReceivedDataToUI();

signals:
    void Send(QByteArray& s, bool& consoleOrProgramEditor);

};
#endif // SERIALPORTMANAGER_H
