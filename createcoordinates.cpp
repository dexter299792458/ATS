#include "createcoordinates.h"
#include "ui_createcoordinates.h"
#include <QMessageBox>

CreateCoordinates::CreateCoordinates(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateCoordinates)
{
    ui->setupUi(this);
}

CreateCoordinates::~CreateCoordinates()
{
    delete ui;
}

//** implementeert een apart GUI scherm voor het aanmaken van een coordinaat d.m.v meerdere ACL's. **//


//Wanneer de gebruiker in het pop-up scherm voor het aanmaken van coordinaten op 'OK' drukt:
//De ingevulde data wordt in een QStringList geplaatst en alle enkele QString's krijgen
//een hexadecimale `enter` achter hun huidige data geplakt voor het verzenden.
//Vervolgens handelt de methode SendCoordinatedToController()  het verzenden van een QStringList
//naar de SerialPortManager af.
void CreateCoordinates::on_buttonBox_accepted()
{
    if(ui->PosName->text().isEmpty() || ui->X->text().isEmpty() || ui->Y->text().isEmpty()
            || ui->Z->text().isEmpty() || ui->P->text().isEmpty() || ui->R->text().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText("Not all the parameters are assigned a value!");
        msgBox.exec();
        this->show();
    }
    else
    {
        coordinate.append("defp\x20" + ui->PosName->text() + "\x00D");
        coordinate.append("teach\x20" + ui->PosName->text() + "\x00D" );
        coordinate.append(ui->X->text() + "\x00D");
        coordinate.append(ui->Y->text() + "\x00D");
        coordinate.append(ui->Z->text() + "\x00D");
        coordinate.append(ui->P->text() + "\x00D");
        coordinate.append(ui->R->text() + "\x00D");
        CreateCoordinates::SendCoordinatesToController(coordinate);
    }
}

//Verzenden van de QStringList met daarin de ACL Commando's die nodig zijn voor het aanmaken
//van een coordinaat. Verzoek komt vanaf de ProgramEditor (WRITE_TO_PROGRAMEDITOR)
void CreateCoordinates::SendCoordinatesToController(QStringList sendCoordinate)
{
    singleton_SerialPortManager = SerialPortManager::GetInstance();
    singleton_SerialPortManager->WriteMultipleACLCommands(sendCoordinate, WRITE_TO_PROGRAMEDITOR);
    coordinate.clear();
}
