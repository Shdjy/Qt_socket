#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sendfile.h"
#include <QMessageBox>
#include <QFileDialog>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //初始化
    ui->ip->setText("127.0.0.1");
    ui->port->setText("9999");
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);
    //创建线程
    QThread *t = new QThread;
    //创建任务对象
    SendFile *worker = new SendFile;

    worker->moveToThread(t);
    connect(this,&MainWindow::sendPath,worker,&SendFile::sendFile);

    connect(this,&MainWindow::startConnect,worker,&SendFile::connectServer);
    //处理线程信号
    connect(worker,&SendFile::connectok,this,[=]()
    {
       QMessageBox::information(this,"连接服务器","连接服务器成功！");
    });

    connect(worker,&SendFile::connectover,this,[=]()
    {
        //资源释放
        t->quit();
        t->wait();
        worker->deleteLater();
        t->deleteLater();
    });

    connect(worker,&SendFile::curpercent,ui->progressBar,&QProgressBar::setValue);
    t->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connect_clicked()
{
    QString ip = ui->ip->text();
    unsigned short port = ui->port->text().toUShort();
    emit startConnect(port, ip);
}

void MainWindow::on_chooseFile_clicked()
{
    QString path = QFileDialog::getOpenFileName();
    if(path.isEmpty())
    {
        return;
    }
    ui->pathFile->setText(path);

}

void MainWindow::on_send_clicked()
{
    emit sendPath(ui->pathFile->text());
}
