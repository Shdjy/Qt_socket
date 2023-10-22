#include "recvfile.h"
#include <QFile>
RecvFile::RecvFile(qintptr sock,QObject *parent) : QThread(parent)
{
   m_sock = sock;
}

void RecvFile::run()
{
    m_tcp = new QTcpSocket(this);
    m_tcp->setSocketDescriptor(m_sock);

    QFile *file = new QFile("recv.txt");
    file->open(QFile::WriteOnly);

    //接受数据
    connect(m_tcp, &QTcpSocket::readyRead,this,[=]()
    {
        static int count = 0;
        static int total = 0;
        if(count == 0)
        {
            m_tcp->read((char*)&total,4);
        }
        //读出剩余数据
        QByteArray data = m_tcp->readAll();
        count += data.size();
        file->write(data);
        //
        if(count == total)
        {
            m_tcp->close();
            m_tcp->deleteLater();
            file->close();
            file->deleteLater();
            emit over();
        }

        //事件循环
        exec();

    });
}
