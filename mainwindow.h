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

private:

    QLabel *statusLabel;
    QVBoxLayout * _mainLayout;
    std::vector<QLabel*> _connectedClients;

    QTcpServer *tcpServer;
    QStringList fortunes;
    QNetworkSession *networkSession;

    std::vector<namedConnection> _establishedConnections;
    std::vector<identifiedStreams> _dataStreams;

    int _currentBuggedSocket = 0;


};

#endif // MAINWINDOW_H
