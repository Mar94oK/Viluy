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

typedef std::pair <QTcpSocket*, QString> namedConnection;
typedef std::pair <QDataStream*, int> identifiedStreams;


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

    std::vector<namedConnection> _establishedConnections;
    std::vector<identifiedStreams> _dataStreams;

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
    void slot_readTheClientName(int socketDescriptor);
    void slot_reportError(QAbstractSocket::SocketError);

private:

    void setFortunes();
    void slot_serverInitializaion();

};

#endif // SERVER_H
