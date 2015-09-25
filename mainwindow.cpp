#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTextStream>
#include <ios>
#include <stdio.h>
#include <QSerialPort>

QSerialPort *serial;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serial = new QSerialPort(this);
    serial->setPortName("COM1");
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->open(QIODevice::ReadWrite);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::serialRecieved()
{

}

void MainWindow::on_pushButton_3_clicked()
{
   QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
    "",tr("Files *.txt"));
   ui->plainTextEdit->clear();
   QFile inputFile(fileName);
   if (inputFile.open(QIODevice::ReadOnly))
   {
      QTextStream in(&inputFile);
      while (!in.atEnd())
      {
         QString line = in.readLine();
         ui->plainTextEdit->appendPlainText(line);
      }
      inputFile.close();
   }
}

void MainWindow::on_pushButton_2_clicked()
{
    connect(serial,SIGNAL(readyRead()),this,SLOT(serialReveiced()));

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
     "",tr("Files .txt"));
    ui->plainTextEdit_2->appendPlainText(ui->plainTextEdit->toPlainText() + "hoi");

    QString tekst = ui->plainTextEdit->toPlainText();
    QStringList regels = tekst.split("\n");

    for(int i = 0; i < regels.count(); i++)
     {

        //std::string naampie = regels[i].toUtf8().constData();
        QByteArray naampie = regels[i].toLatin1();
        serial->write(naampie);
    }

     serial->close();

}

void MainWindow::on_pushButton_clicked()
{

}
