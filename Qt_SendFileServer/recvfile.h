#ifndef RECVFILE_H
#define RECVFILE_H

#include <QThread>
#include <QTcpSocket>

class RecvFile : public QThread
{
    Q_OBJECT
public:
    explicit RecvFile(qintptr sock, QObject *parent = nullptr);

signals:
    void over();

protected:
    void run() override;


private:
    QTcpSocket *m_tcp;
    qintptr m_sock;
};

#endif // RECVFILE_H
