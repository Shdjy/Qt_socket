#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->port->setText("9999");
    ui->ip->setText("127.0.0.1");
    this->setWindowTitle("TCP-客户端");
    this->setWindowIcon(QIcon(":/image/main"));


    //创建套接字对象
    m_tcp = new QTcpSocket(this);
    //检测服务器是否回复了数据
    connect(m_tcp,&QTcpSocket::readyRead,this,[=]()
    {
        QByteArray data = m_tcp->readAll();
        ui->record->append("服务器:" + data);
    });

    //断开连接
    connect(m_tcp,&QTcpSocket::disconnected, this,[=]()
    {
        m_tcp->close();
        m_status->setPixmap(QPixmap(":/image/no.png").scaled(20,20));
        ui->record->append("服务器已经和客户端断开！");
        ui->connect->setEnabled(true);
        ui->disconnect->setEnabled(false);
    });

    //连接
    connect(m_tcp,&QTcpSocket::connected,this,[=]()
    {
        m_status->setPixmap(QPixmap(":/image/yes.png").scaled(20,20));
        ui->record->append("服务器已经和客户端连接！");
    });

    //状态栏
    m_status = new QLabel(this);
    m_status->setPixmap(QPixmap(":/image/no.png").scaled(20,20));
    ui->statusbar->addWidget(new QLabel("连接状态:"));
    ui->statusbar->addWidget(m_status);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connect_clicked()
{
    QString ip = ui->ip->text();
    unsigned short port = ui->port->text().toUShort();
    m_tcp->connectToHost(QHostAddress(ip),port);
    ui->connect->setEnabled(false);
    ui->disconnect->setEnabled(true);
}

void MainWindow::on_disconnect_clicked()
{
    m_tcp->close();
    ui->connect->setEnabled(true);
    ui->disconnect->setEnabled(false);
}

void MainWindow::on_send_clicked()
{
    QString msg = ui->msg->toPlainText();
    m_tcp->write(msg.toUtf8());
    ui->record->append("客户端："+msg);
    ui->msg->clear();
}
