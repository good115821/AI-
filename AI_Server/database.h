#ifndef DATABASE_H
#define DATABASE_H

#include <QTcpServer>   // 통신
#include <QTcpSocket>   // 통신

#include <QtSql>        // DB
#include <QSqlDatabase> // DB
#include <QSqlQuery>    // DB
#include <QDateTime>
#include <QFile>

class Database
{
public:
    Database();

protected:
    QMap<QTcpSocket *, QString> Qmap_User_Socket_ID;           // QMap 소켓 - ID 묶음                   // 임호진이 추가함
    QMap<QString, QString>  Qmap_User_ID_Type;                 // QMap ID - Type 묶음(환자, 의사)        // 임호진이 추가함


    void Login(QTcpSocket * senderSKT, QStringList DATA_LIST);              // 로그인 함수
    void Join(QTcpSocket * senderSKT, QStringList DATA_LIST);               // 회원가입 함수
    void ID_CHECK(QTcpSocket * senderSKT, QStringList DATA_LIST);           // 중복확인 함수
    void Hospital_List(QTcpSocket * senderSKT);                             // 병원 리스트 보내기 함수
    void Doctor_List(QTcpSocket * senderSKT, QStringList DATA_LIST);        // 의사 리스트 보내기 함수
    void Find_ID_PW(QTcpSocket * senderSKT, QStringList DATA_LIST);         // ID / PW 찾기 함수
    void Schedule_Search(QTcpSocket * senderSKT, QStringList DATA_LIST);    // 등록된 일정 찾기
    void Schedule_Insert(QTcpSocket * senderSKT, QStringList DATA_LIST);    // 일정 등록하기
    void Schedule_Detail(QTcpSocket * senderSKT, QStringList DATA_LIST);    // 일정 상세보기
    void Schedule_Update(QTcpSocket * senderSKT, QStringList DATA_LIST);    // 일정 업데이트
    void Schedule_Delete(QTcpSocket * senderSKT, QStringList DATA_LIST);    // 일정 삭제
    void Patient_Question_Check(QTcpSocket * senderSKT, QStringList DATA_LIST); // 환자 상담 확인하기
    QString Patient_Question_ADD(QTcpSocket * senderSKT, QStringList DATA_LIST);   // 환자 상담 등록하기
    void Patient_Question(QTcpSocket * senderSKT, QStringList DATA_LIST);   // 담당 환자 답변 보기
    void Patient_List(QTcpSocket * senderSKT, QStringList DATA_LIST);       // 환자 리스트
    void Patient_Detail(QTcpSocket * senderSKT, QStringList DATA_LIST);     // 환자 분석 자료 상세보기
    void Analyze_Result_Save(QString patient_num, QString DATA_LIST);   // 환자 분석 결과값 DB에 저장
    void Setting(QTcpSocket * senderSKT, QStringList DATA_LIST);            // 셋팅 정보 보내기기
    void User_Logout(QStringList DATA_LIST);                                // 로그아웃
    void Secession(QTcpSocket * senderSKT, QStringList DATA_LIST);          // 회원탈퇴
    void DB_Disconnected(QTcpSocket * leftSKT);                             // 연결 종료시 DB 값 수정
    void Send_PYTHON(QTcpSocket * SKT, QString TEXT);  // 파이썬 서버로 데이터 전송

public:
};

#endif // DATABASE_H
