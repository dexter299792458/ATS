#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H
#include <QMainWindow>

class SerialPortManager : public QObject
{
     Q_OBJECT
private:    
    static SerialPortManager* instance;    
protected:
    SerialPortManager();
public:
    static SerialPortManager* GetInstance();
    void OpenSerialConnection();
    void CloseSerialConnection();
    void WriteSingleACLCommand(QString);
    void WriteMultipleACLCommands();

public slots:
    void GiveReceivedDataToUI();
signals:
    void Send(QByteArray& s);

};
#endif // SERIALPORTMANAGER_H
