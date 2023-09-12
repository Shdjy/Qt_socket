#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void startConnect(unsigned short , QString);

    void sendPath(QString);
private slots:
    void on_connect_clicked();

    void on_chooseFile_clicked();

    void on_send_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
