#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->port->setText("9999");
    this->setWindowTitle("TCP-服务端");
    this->setWindowIcon(QIcon(":/image/main.png"));
    //状态栏
    m_status = new QLabel;

    ui->statusbar->addWidget(new QLabel("连接状态:"));
    ui->statusbar->addWidget(m_status);

    QPixmap *pix = new QPixmap(":/image/no.png");
    m_status->setPixmap(pix->scaled(20,20));

    //创建监听服务器对象
    m_s = new QTcpServer(this);

    connect(m_s,&QTcpServer::newConnection, this, [=]()
    {
        m_tcp = m_s->nextPendingConnection();

        m_status->setPixmap(QPixmap(":/image/yes.png").scaled(20,20));

        //检测是否可以接受数据
        connect(m_tcp, &QTcpSocket::readyRead,this,[=]()
        {
            QByteArray data = m_tcp->readAll();
            ui->record_2->appendPlainText("客户端:"+data);
        });

        //断开
        connect(m_tcp,&QTcpSocket::disconnected,this,[=]()
        {
            m_tcp->close();
            m_tcp->deleteLater();

            m_status->setPixmap(pix->scaled(20,20));
        });
    });




}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_startSever_clicked()
{
    unsigned short port = ui->port->text().toUShort();
    m_s->listen(QHostAddress::Any,port);
    ui->startSever->setDisabled(true);
}

void MainWindow::on_send_clicked()
{
    QString msg = ui->sendMsg_2->toPlainText();
    m_tcp->write(msg.toUtf8());
    ui->record_2->appendPlainText("服务器："+msg);
    ui->sendMsg_2->clear();
}
