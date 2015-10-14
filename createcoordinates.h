#ifndef CREATECOORDINATES_H
#define CREATECOORDINATES_H
#define WRITE_TO_CONSOLE true
#define WRITE_TO_PROGRAMEDITOR false

#include <QDialog>
#include "serialportmanager.h"

namespace Ui {
class CreateCoordinates;
}

class CreateCoordinates : public QDialog
{
    Q_OBJECT

public:
    explicit CreateCoordinates(QWidget *parent = 0);
    ~CreateCoordinates();


private slots:
    void on_buttonBox_accepted();

private:
    Ui::CreateCoordinates *ui;
    void SendCoordinatesToController(QStringList);
    SerialPortManager *singleton_SerialPortManager;
    QStringList coordinate;
};

#endif // CREATECOORDINATES_H
