#ifndef SERVER_H
#define SERVER_H

#include <QObject>

#include <QtNetwork>
#include <QString>
#include <QNetworkSession>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkSession>
#include <serversettings.h>
//since it is necessary to report everything during server init, the Window should already be created.
#include <mainwindow.h>
#include <room.h>

typedef QPair<QString, QString> serverSettings;


class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server( QObject *parent = 0);

private:

    QTcpServer *tcpServer;
    QStringList fortunes;
    QNetworkSession *networkSession;

    std::vector<Room> _rooms;

    std::vector<QTcpSocket*> _establishedConnections;

    ServerSettings _settings;

signals:

    void sig_sendFortune(int socketDescriptor);
    void sig_serverErrorReport(QString);
    void sig_serverLogReport(QString);
    void sig_serverInfoReport(QString);


private slots:

    int slot_sessionOpened();
    void slot_sendFortune(int socketDescriptor);
    void slot_setUpNewConnection();
    void slot_readIncomingData(int socketDescriptor);
    void slot_reportError(QAbstractSocket::SocketError);

private:

    void setFortunes();
    void slot_serverInitializaion();

};

#endif // SERVER_H
