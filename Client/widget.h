#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket> // QTcpSocket을 포함합니다.
#include <QHostAddress>
#include <QByteArray>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QScreen>//화면중앙이동함수적용
#include <QFileDialog>//의사면허증 사진파일 선택
#include <QFrame>
#include <QApplication>
#include <QScrollArea>
#include <QComboBox>
#include <QVector>
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QMovie>
#include <QTimer>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QThread>



QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    int number_cnt = 0;
    QString user_num = "";
    int select_num = 0;
    QString select_human;
    QString select_patient_num;


private:
    Ui::Widget *ui;
    int beforePage = 0;
    int totalcheck_patient;
    int totalcheck_doctor = 0;
    int pwcheck_patient = 0;
    int secesstion_num = 0;
    int result;
    QTimer timer;                                                        //현재 시간 저장
    QString day_date = today_Tm();                                       //오늘 날짜 저장
    QTcpSocket *m_socket;                                                //통신소켓
    QVector<QString> doctor_list;                                        //의사 리스트
    QVector<QString> hospital_list;                                      //병원 리스트
    QVector<QString> patient_num_list;                                   //환자 번호 리스트
    QVector<QString> name_list;                                          //이름 리스트
    QVector<QString> age_list;                                           //나이 리스트
    QVector<QString> phone_list;                                         //전화번호 리스트
    QVector<QString> gender_list;                                        //성별 리스트
    QByteArray imageData;                                                //이미지 데이터 저장
    QStringList split;                                                   //X, Y, Z축 값 저장
    QPushButton *myButton;                                               //UI button
    QFrame *myFrame;                                                     //UI Frame
    QScrollArea *scroll;                                                 //UI scroll
    QComboBox *comboBox;                                                 //UI comboBox
    QVBoxLayout *layout;                                                 //UI layout
    QHBoxLayout *box;                                                    //UI layout
    QLabel *label;                                                       //UI label
    QDialog *dialog;                                                     //UI dailog
    QTextEdit *edit;                                                     //UI textEdit

signals:
    void signal_newMessage(QString);

private slots:
    void initialize();                                                   //시작 서버 연결
    void connectIP();                                                    //서버 연결 IP설정
    void Loding_Screen(int Num);                                         //로딩화면
    void on_pushButton_4_clicked();                                      //이동 버튼(홈, 일기, 설정)
    void on_center_home_menu_clicked();                                  //이동버튼(가운데 메뉴에서 홈버튼)
    void readMessage();                                                  //서버로부터 데이터 받아오기
    void on_pushButton_33_clicked();                                     //로그아웃 버튼
    void on_join_patient_Btn_clicked();                                  //회원가입-환자선택
    void on_join_doctor_Btn_clicked();                                   //회원가입-의사선택
    void on_app_ver_clicked();                                           //앱버전
    QString today_Tm();                                                  //오늘 날짜 받아오기

public:
    void delete_schedule();                                              // 스케줄 삭제
    void calender_going();                                               //캘린더 클릭 후 이동 및 정보 받아오기
    void calender_update();                                              //일정 업데이트
    void calender_update_success();                                      //일정 업데이트
    void Login_Start();                                                  //로그인
    void Move_Page(int page);                                            //페이지 이동
    void Back();                                                         //뒤로가기
    void CenterOnScreen();                                               //창위치 조절
    void idCheck_Doctor();                                               //아이디 중복확인(의사)
    void idCheck_Patient();                                              //아이디 중복확인(환자)
    void idSearch();                                                     //아이디 찾기
    void pwSearch();                                                     //비밀번호 찾기
    void Patient_PW_Text_Changed();                                      //입력한 비밀번호 일치여부 확인하여 텍스트 출력 및 색깔변경함수(환자)
    void Doctor_PW_Text_Changed();                                       //입력한 비밀번호 일치여부 확인하여 텍스트 출력 및 색깔변경함수(의사)
    void Patient_Join_Complete();                                        //회원가입 완료버튼(환자용)
    void Doctor_Join_Complete();                                         //회원가입 완료버튼(의사용)
    void Doctor_image();                                                 //의사 면허증 openfiledialog
    void Check_DoctorLiscense();                                         //의사면허증 6자리 여부 확인 함수
    void Join_Btn_Update_Patient();                                      //회원가입-환자선택
    void Join_Btn_Update_Doctor();                                       //회원가입-의사선택
    void on_calendarWidget_clicked(const QDate &date);                   //캘린터 클릭
    void close_myday();                                                  //환자의 일지 추가
    void save_myschedule();                                              //의사, 환자 일정 추가
    void join_doctor_check();                                            //회원가입 의사 체크
    void join_patient_check();                                           //회원가입 환자 체크
    void see_my_patient();                                               //환자의 일지 확인
    void give_doctor_list();                                             //의사리스트 받아오기
    void give_doctor_list1();                                            // 의사 리스트 받아오기
    void select_hospital_list();                                         // 회원가입에서 병원 리스트 중에 선택한 것
    void give_hospital_list();                                           //서버에서 병원 리스트 받아오기
    void select_home();                                                  //일정 확인하고 확인버튼
    void select_setting();                                               //setting 이동함수
    void select_center();                                                //center 이동함수
    void secession_detail();                                             //회원탈퇴
    void go_to_message();                                                //알림보내기
    void see_mypatient_detail();                                         //분석하기
};
#endif // WIDGET_H
