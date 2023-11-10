#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_hostAddress = getMyIP();
    m_hostPort = qint16(25000);
    m_title = "C++_AI_서버 (" + m_hostAddress.toString() + " " + QString::number(m_hostPort) + ")";
    setWindowTitle(m_title);
    server = new Server(this);
    server->listen(m_hostAddress, m_hostPort);
    connect(server, SIGNAL(signal_showNewMSG(QString)), this, SLOT(slot_showMSG(QString)));
    connect(ui->btn_AI_server_connect, SIGNAL(clicked()), server, SLOT(initialize()));
    connect(ui->btn_AI_server_disconnect, SIGNAL(clicked()), server, SLOT(PYTHON_Disconnected()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

QHostAddress MainWindow::getMyIP()
{
    QHostAddress myAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address())
        {
            myAddress = ipAddressesList.at(i);
            break;
        }
    }

    if (myAddress.toString().isEmpty())
        myAddress = QHostAddress(QHostAddress::LocalHost);

    return myAddress;
}

void MainWindow::slot_showMSG(QString msg)
{
    ui->textEdit_server_log->append(msg);
}
