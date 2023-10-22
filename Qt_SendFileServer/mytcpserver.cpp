#include "mytcpserver.h"

MyTcpServer::MyTcpServer(QObject *parent) : QTcpServer(parent)
{

}

void MyTcpServer::incomingConnection(qintptr handle)
{
    emit newDescriptor(handle);
}
