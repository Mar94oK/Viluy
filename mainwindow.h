#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QLayout>
#include <QtWidgets>
#include <QtNetwork>
#include <QString>
#include <stdlib.h>
#include <QNetworkSession>
#include <QTcpServer>

typedef std::pair <QTcpSocket*, QString> namedConnection;
typedef std::pair <QDataStream*, int> identifiedStreams;


namespace Ui {
class MainWindow;
}

class ServerMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerMainWindow(QWidget *parent = 0);
    ~ServerMainWindow();

private:
    Ui::MainWindow *ui;

private slots:

    void sessionOpened();
    void sendFortune(int socketDescriptor);

    void setUpNewConnection();
    void readTheClientName(int socketDescriptor);
    void displayError(QAbstractSocket::SocketError);


signals:

    void signal_sendFortune(int socketDescriptor);

public slots:

    void slot_showServerLogMessage(QString message);
    void slot_showServerErrorMessage(QString message);
    void slot_showServerInfoMessage(QString message);

private:


    std::vector<QLabel*> _connectedClients;

    QTcpServer *tcpServer;
    QStringList fortunes;
    QNetworkSession *networkSession;

    int _currentBuggedSocket = 0;

    std::vector<namedConnection> _establishedConnections;
    std::vector<identifiedStreams> _dataStreams;




};

#endif // MAINWINDOW_H
