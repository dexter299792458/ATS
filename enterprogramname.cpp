#include "enterprogramname.h"
#include "ui_enterprogramname.h"
#include <QDebug>
#include <QMessageBox>

EnterProgramName::EnterProgramName(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EnterProgramName)
{
    ui->setupUi(this);
    ui->lineEdit->setFocus();
}

EnterProgramName::~EnterProgramName()
{
    delete ui;
}

//*** implementeert een apart GUI scherm voor het invoeren van een programmanaam. ***//


//SIGNAL accepted() ---> Wanneer gebruiker op OK klikt wordt dit opgevangen in de UserInterface
//hiervoor is een connect(); gebruikt, zie hiervoor UserInterface() constructor.
void EnterProgramName::on_buttonBox_accepted()
{

}

//Het receiver SLOT gedefinieerd in de UserInterface klasse roept onderstaande methode
//ReturnEnteredProgramName() aan. Methode stuurt een QString terug met daarin
//de ingevoerde programmanaam. Zie UserInterface constructor voor de `connect`
//tussen de UserInterface GUI en de EnterProgramName GUI.
QString EnterProgramName::ReturnEnteredProgramName()
{
    //Check of er een programmanaam is ingevuld, laat anders opnieuw het scherm zien.
    if(ui->lineEdit->text().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText("Please enter a programname!");
        msgBox.exec();
        this->show();
        return nullptr;
    }
    else
    {
         return ui->lineEdit->text();
    }
}
