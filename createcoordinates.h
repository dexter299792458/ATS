#ifndef CREATECOORDINATES_H
#define CREATECOORDINATES_H
#include <QDialog>

namespace Ui {
class CreateCoordinates;
}

class CreateCoordinates : public QDialog
{
    Q_OBJECT

public:
    explicit CreateCoordinates(QWidget *parent = 0);
    ~CreateCoordinates();
    void ReturnCoordinatesToUI();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::CreateCoordinates *ui;

};

#endif // CREATECOORDINATES_H
