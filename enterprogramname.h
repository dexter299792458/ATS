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
    ~EnterProgramName();

private:
    Ui::EnterProgramName *ui;
};

#endif // ENTERPROGRAMNAME_H
