#ifndef ENTERPROGRAMNAME_H
#define ENTERPROGRAMNAME_H

#include <QDialog>

namespace Ui {
class EnterProgramName;
}

class EnterProgramName : public QDialog
{
    Q_OBJECT

public:
    explicit EnterProgramName(QWidget *parent = 0);
    QString ReturnEnteredProgramName();
    ~EnterProgramName();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::EnterProgramName *ui;
signals:

};

#endif // ENTERPROGRAMNAME_H
