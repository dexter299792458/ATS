#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QFileDialog>
#include <QTextStream>
#include <ios>
#include <stdio.h>
#include <time.h>
#include <dos.h>
#include <windows.h>

QSerialPort *serial;
int serialDef = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit->setStyleSheet("QLineEdit{background: red;}");
    ui->lineEdit->setText("Please connect...");
}

void MainWindow::serialReveiced()
{
    ui->plainTextEdit_2->appendPlainText(serial->readAll());
}

MainWindow::~MainWindow()
{
    delete ui;
    if(serialDef == 1)
    {
        serial->close();
    }
}

void MainWindow::on_openButton_clicked()
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

void MainWindow::on_saveButton_clicked()
{
    if (serialDef == 1)
    {
        QString tekst = ui->plainTextEdit->toPlainText();
        QStringList regels = tekst.split("\n");
        for(int i = 0; i < regels.count(); i++)
        {
             QByteArray naampie = (regels[i].toLatin1() + "\x00D");
             serial->write(naampie);
             //Sleep(1000);
        }
        ui->plainTextEdit->clear();
    }
    else
    {
        ui->plainTextEdit_2->appendPlainText("Please connect... \n");
    }
}

void MainWindow::on_connectButton_clicked()
{
    if (serialDef == 1)
    {
        if (ui->dropdownList->currentText() == serial->portName())
        {
            ui->plainTextEdit_2->appendPlainText("COM port already connected \n");
        }
        else
        {
            ui->plainTextEdit_2->appendPlainText("Disconnect with current COM first \n");
        }
    }
    else
    {
        serial = new QSerialPort(this);
        serial->setPortName(ui->dropdownList->currentText());
        serial->setBaudRate(QSerialPort::Baud9600);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);
        serial->open(QIODevice::ReadWrite);
        connect(serial,SIGNAL(readyRead()),this,SLOT(serialReveiced()));
        if (serial->isOpen())
        {
            ui->lineEdit->setStyleSheet("QLineEdit{background: green;}");
            ui->lineEdit->setText("Connected");
            serialDef = 1;
        }
        else
        {
            ui->lineEdit->setStyleSheet("QLineEdit{background: red;}");
            ui->lineEdit->setText("Connecting failed...");
            serial->close();
        }
    }
}

void MainWindow::on_disconnectButton_clicked()
{
    if (serialDef == 1)
    {
        serial->close();
        ui->lineEdit->setStyleSheet("QLineEdit{background: red;}");
        ui->lineEdit->setText("Disconnected");
        serialDef = 0;
    }
}

void MainWindow::on_clear_editorButton_clicked()
{
    ui->plainTextEdit->clear();
}

void MainWindow::on_clear_consoleButton_clicked()
{
    ui->plainTextEdit_2->clear();
}
