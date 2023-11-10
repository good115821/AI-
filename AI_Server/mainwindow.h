#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "server.h"

#include <QMainWindow>
#include <QNetworkInterface>
#include <QHostAddress>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QTcpServer;
class QNetworkSession;
class Server;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Server *server;

private:
    Ui::MainWindow *ui;

    // 중앙 서버 IP
    const QString ServerIP = "10.10.21.118";
    // 중앙 서버 Port
    const QString ServerPort = "25000";

    QHostAddress m_hostAddress;
    qint16 m_hostPort;
    QString m_title;
    QHostAddress getMyIP();

private slots:
    void slot_showMSG(QString msg);
};
#endif // MAINWINDOW_H
