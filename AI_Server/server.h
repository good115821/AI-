#ifndef SERVER_H
#define SERVER_H

#include "database.h"

#include <QFileSystemWatcher>
#include <QMessageBox>
#include <QDataStream>

#include <QTcpServer>
#include <QTcpSocket>
#include <QFileInfo>

#include <QList>        //
#include <QFile>        //
#include <QSet>         //
#include <QMap>         // QMap
#include <QDir>         //

#include <QtSql>        // DB
#include <QSqlDatabase> // DB
#include <QSqlQuery>    // DB

class MainWindow;

class Server : public QTcpServer, protected Database
{
    Q_OBJECT

public:
    Server(QObject *parent=0);
    ~Server();


protected:
    void incomingConnection(qintptr socketfd);

private:
    QSet<QTcpSocket *> qset_clntSKTList;                // 서버에 접속된 모든 온라인 클라이언트 (소켓)리스트

    QString last_saveing_file;
    // 파일 저장 경로
    const QString savePath = "C:/Users/LMS23/Desktop/server_file/";

    // 파일 변동을 자동으로 감시할 폴더경로
    const QString auto_Path = "C:/Users/LMS23/Desktop/server_file/";

    // QFileSystemWatcher 객체 생성
    QFileSystemWatcher watcher;

    // 파이썬 서버 연결
    QTcpSocket *tcpSocket;

    // Mysql_DB 세팅
    QSqlDatabase db;
    QString hostName = "10.10.21.128";
    int port = 3306;
    QString dbName = "6team";
    QString userName = "6team";
    QString passWord = "1234";

    // Mysql_DB 연결 함수
    void Mysql_DB_Connection();

    // 파이썬 서버 IP
    const QString py_ServerIP = "10.10.21.118";

    // 파이썬 서버 Port
    const QString py_ServerPort = "30000";

    // 파이썬 서버 연결 성공 / 실패 확인 함수
    void Py_Server_Connected();

signals:
    void signal_updateClntCNT(int No_Users);
    void signal_showNewMSG(QString msg);

private slots:
    void slot_readMSG();                     // 서버에 도착한 메세지 출력 함수
    void initialize();                       // 파이썬 서버 연결 관련 함수
    void PYTHON_Disconnected();              //
    void Read_file(QString filepath);
    void slot_disconnected();                // 연결 종료 처리 함수
};

#endif // SERVER_H
