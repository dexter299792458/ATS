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
protected:
    SerialPortManager();
public:
    static SerialPortManager* GetInstance();
    void OpenSerialConnection();
    void CloseSerialConnection();
    void WriteSingleACLCommand(QString);
    void WriteMultipleACLCommands();
    void InitializeSerialConnection(QString);

public slots:
    void GiveReceivedDataToUI();
signals:
    void Send(QByteArray& s);

};
#endif // SERIALPORTMANAGER_H
