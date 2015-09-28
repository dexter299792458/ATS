#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_openButton_clicked();

    void on_saveButton_clicked();

    void on_connectButton_clicked();

    void serialReveiced();

    void on_disconnectButton_clicked();

    void on_clear_editorButton_clicked();

    void on_clear_consoleButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
