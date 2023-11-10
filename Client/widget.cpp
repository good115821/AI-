#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0); //1번째 페이지부터 시작

    beforePage = 0;
    totalcheck_patient = 0;

    this->setFixedSize(430, 900);           // 기본크기 넓이 430 / 높이 900
    ui->chat_log->setReadOnly(true);

    // 로딩화면
    Loding_Screen(0);

    // 메인화면
    connect(ui->login_Btn, &QPushButton::clicked,this,&Widget::Login_Start);                                // 로그인 버튼
    connect(ui->join_Btn,&QPushButton::clicked,this,[this](){Move_Page(1);});                               // 회원가입 이동버튼
    connect(ui->join_patient_Btn,&QPushButton::clicked,this,&Widget::give_hospital_list);                   // 회원가입 이동버튼
    connect(ui->join_doctor_Btn,&QPushButton::clicked,this,&Widget::give_hospital_list);                    // 회원가입 이동버튼
    connect(ui->idsearch_Btn,&QPushButton::clicked,[this](){Move_Page(4);});                                // 아이디 찾기 이동버튼
    connect(ui->pwsearch_Btn,&QPushButton::clicked,[this](){Move_Page(5);});                                // 비밀번호 찾기 이동버튼
    connect(ui->btn_delete_schedule,&QPushButton::clicked,this,&Widget::delete_schedule);                   // 스케줄 삭제버튼
    connect(ui->Back_1,&QPushButton::clicked,this,&Widget::Back);                                           // 뒤로가기 버튼
    connect(ui->Back_2,&QPushButton::clicked,this,&Widget::Back);                                           // 뒤로가기 버튼
    connect(ui->Back_3,&QPushButton::clicked,this,&Widget::Back);                                           // 뒤로가기 버튼
    connect(ui->Back_4,&QPushButton::clicked,this,&Widget::Back);                                           // 뒤로가기 버튼
    connect(ui->Back_5,&QPushButton::clicked,this,&Widget::Back);                                           // 뒤로가기 버튼
    connect(ui->iddoctor_chkBtn,&QPushButton::clicked,this,&Widget::idCheck_Doctor);                        // 의사 아이디 중복 체크
    connect(ui->idpatient_chkBtn,&QPushButton::clicked,this,&Widget::idCheck_Patient);                      // 환자 아이디 중복 체크
    connect(ui->idsearch_Btn2,&QPushButton::clicked,this,&Widget::idSearch);                                // 아이디 찾기
    connect(ui->pwsearch_Btn2,&QPushButton::clicked,this,&Widget::pwSearch);                                // 비밀번호 찾기
    connect(ui->pwconfirm_patient, &QLineEdit::textChanged,this,&Widget::Patient_PW_Text_Changed);          // 비밀번호 일치여부 확인 및 색깔변경(환자)
    connect(ui->pwconfirm_doctor, &QLineEdit::textChanged,this,&Widget::Doctor_PW_Text_Changed);            // 비밀번호 일치여부 확인 및 색깔변경(의사)
    connect(ui->Complete_join_patient_Btn, &QPushButton::clicked,this, &Widget::Patient_Join_Complete);     // 환자 회원가입 완료버튼
    connect(ui->Complete_join_doctor_Btn, &QPushButton::clicked,this, &Widget::Doctor_Join_Complete);       // 의사 회원가입 완료버튼
    connect(ui->doctor_license_Btn,&QPushButton::clicked,this,&Widget::Doctor_image);                       // 의사면허증 출력버튼
    connect(ui->doctornum_chkBtn,&QPushButton::clicked,this,&Widget::Check_DoctorLiscense);                 // 의사면허번호 6자리여부 확인
    connect(ui->patient_2,&QPushButton::clicked,this,[this](){Move_Page(11);});                             // 일지 작성 페이지 이동
    connect(ui->close_myday_Btn_2,&QPushButton::clicked,this,&Widget::see_mypatient_detail);                // 분석하기
    connect(ui->calendarWidget, &QCalendarWidget::clicked, this, &Widget::on_calendarWidget_clicked);       // 환자 캘린더 클릭
    connect(ui->calendarWidget_2, &QCalendarWidget::clicked, this, &Widget::on_calendarWidget_clicked);     // 의사 캘린더 클릭
    connect(ui->close_myday_Btn,&QPushButton::clicked,this,&Widget::close_myday);                           // 오늘 하루 마무리하기 버튼 클릭시 기능
    connect(ui->save_myschedule_patient,&QPushButton::clicked,this,&Widget::save_myschedule);               // 환자->내 일정 저장하기 버튼
    connect(ui->setting_patient,&QPushButton::clicked,this,&Widget::select_setting);                        // 환자 UI에서 setting버튼 클릭시 setting창 이동
    connect(ui->home_setting_menu,&QPushButton::clicked,this,&Widget::select_setting);                      // 의사 UI에서 setting버튼 클릭시 setting창 이동
    connect(ui->pushButton_2, &QPushButton::clicked,this,&Widget::select_home);                             // 일정 확인 버튼
    connect(ui->pushButton, &QPushButton::clicked,this, &Widget::calender_going);                           // 일정 추가 버튼(환자)
    connect(ui->pushButton_4, &QPushButton::clicked,this, &Widget::calender_going);                         // 일정 추가 버튼(의사)
    connect(ui->pushButton_3, &QPushButton::clicked,this, &Widget::calender_update);                        // 일정 편집 버튼
    connect(ui->update_schedule_patient, &QPushButton::clicked,this, &Widget::calender_update_success);     // 일정 업데이트 버튼
    connect(ui->pushButton_29, &QPushButton::clicked,this, &Widget::see_my_patient);                        // 환자 일지 받아오기
    connect(ui->doctor_back_home,&QPushButton::clicked,this,[this](){Move_Page(9);});                       // 의사 홈으로 이동
    connect(ui->hospital, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Widget::select_hospital_list); // 병원 리스트 받아오기
    connect(ui->setting_home_menu,&QPushButton::clicked,this,&Widget::select_home);                         // setting에서 홈으로 이동하는 버튼
    connect(ui->center_setting_menu,&QPushButton::clicked,this,&Widget::select_setting);                    // 환자 center에서 setting으로 이동하는 버튼
    connect(ui->center_setting_menu_2,&QPushButton::clicked,this,&Widget::select_setting);                  // 의사 center에서 setting으로 이동하는 버튼
    connect(ui->setting_center_menu,&QPushButton::clicked,this,&Widget::select_center);                     // setting에서 center로 이동
    connect(ui->center_home_menu,&QPushButton::clicked,this,&Widget::select_home);                          // 환자 center에서 home로 이동
    connect(ui->get_out_button,&QPushButton::clicked,this,&Widget::secession_detail);                       // 회원탈퇴 버튼
    connect(ui->send_button,&QPushButton::clicked,this,&Widget::go_to_message);                             // 관리하는 환자에게 알림 보내기 버튼
    connect(ui->close_myday_Btn_3,&QPushButton::clicked,this,[this](){Move_Page(10);});                     // 환자 일지 체크 후 확인 버튼

    CenterOnScreen();
    initialize();
}

Widget::~Widget()
{
    if(m_socket->isOpen())
    {
        m_socket->close();
    }
    delete ui;
}

//시작시 서버 연결
void Widget::initialize()
{

    m_socket = new QTcpSocket(this);
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    connectIP();
}
//서버 연결 IP설정
void Widget::connectIP()
{
    QString serverIP = "10.10.21.118";
    QString serverPort = "25000";

    QHostAddress serverAddress(serverIP);
    m_socket->connectToHost(serverAddress, serverPort.toUShort());
}
//오늘 날짜 받아오기
QString Widget::today_Tm()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString formattedDateTime = currentDateTime.toString("yyyy-MM-dd");
        return formattedDateTime;
}
// 로딩화면
void Widget::Loding_Screen(int Num)
{
    Move_Page(15);  // 로딩화면 인덱스가 15임
    QMovie *movie = new QMovie(":/new/prefix1/resources/Dream Whale.gif");
    ui->label_Loding_screen->setMovie(movie);
    movie->start();
    connect(&timer, &QTimer::timeout, this, [this, Num](){ Move_Page(Num); });
    timer.setSingleShot(true);
    timer.start(3500);
}
//이동 버튼(홈, 일기, 설정)
void Widget::on_pushButton_4_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(3);
}

//이동버튼(가운데 메뉴에서 홈버튼)
void Widget::on_center_home_menu_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(6);
}
// 로그인 확인
void Widget::Login_Start()
{
    QMessageBox msgbox;
    QString type = 0; //type 변수 초기화
    if(m_socket->state() != QAbstractSocket::ConnectedState)
    {
        msgbox.setWindowTitle("서버연결");
        msgbox.setText("서버와 연결이 되어 있는지 확인 후 다시 시도해주세요.");
        msgbox.exec();
    }
    if(ui->loginID->text().trimmed().isEmpty())
    {
        msgbox.setText("아이디를 입력해주세요.");
        msgbox.exec();
    }
    else if(ui->loginPW->text().trimmed().isEmpty())
    {
        msgbox.setText("비밀번호를 입력해주세요.");
        msgbox.exec();
    }
    else if(!ui->for_patient_Btn->isChecked() && !ui->for_doctor_Btn->isChecked())
    {
        msgbox.setText("로그인 타입을 선택해주세요.");
        msgbox.exec();
    }
    else
    {
        if(ui->for_patient_Btn->isChecked())
        {
            type = "1"; //환자
            select_human = "1";
        }
        else if(ui->for_doctor_Btn->isChecked())
        {
            type = "2"; //의사
            select_human = "2";
        }
        QString id = ui->loginID->text().trimmed();
        QString pw = ui->loginPW->text().trimmed();
        QString Type = type;
        QString SendMSG = "LOGIN|" + Type + "|" + id + "|" + pw;
        m_socket -> write(SendMSG.toUtf8());
        m_socket -> flush();
    }
}
//이동 함수
void Widget::Move_Page(int page)
{
    // 0 : 로그인 / 1 : 약관동의 / 2 : 환자용 회원가입 / 3 : 의사용 회원가입 / 4 : 아이디 찾기 / 5 : 비밀번호 찾기
    // 9 : 의사 메인 화면 / 12 : 환경설정 / 15 : 로딩화면
    beforePage = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(page);
    if(page == 0)
    {
        ui->answer1->clear();
        ui->answer2->clear();
        ui->answer3->clear();
        ui->answer1->setReadOnly(false);
        ui->answer2->setReadOnly(false);
        ui->answer3->setReadOnly(false);
        ui->close_myday_Btn->show();
        ui->user_list->clear();
        ui->loginID->clear();
        ui->loginPW->clear();
    }
    else if (page == 9)     // 의사 메인 화면
    {
        ui->user_list->clear();
    }
    else if (page == 11)
    {
        QString SendMSG = QString("Q&A|CHECK|%1").arg(user_num);
        m_socket->write(SendMSG.toUtf8());
        m_socket->flush();
    }
    else if (page == 12)    // 설정창
    {
        ui->user_list->clear();
    }
}
//뒤로가기 함수
void Widget::Back()
{
    // 0 : 로그인 / 1 : 약관동의 / 2 : 환자용 회원가입 / 3 : 의사용 회원가입 / 4 : 아이디 찾기 / 5 : 비밀번호 찾기
    int ui_Index = ui->stackedWidget->currentIndex();
    if(ui_Index == 1)
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
    else if(ui_Index == 2)
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
    else if(ui_Index == 3)
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
    else if(ui_Index == 4)
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
    else if(ui_Index == 5)
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

//창위치 조절
void Widget::CenterOnScreen()
{
    // 현재 화면의 크기 정보 가져오기
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    // 창의 크기 정보 가져오기
    int windowWidth = width();
    int windowHeight = height();
    // 화면 중앙 위치 계산
    int x = (screenGeometry.width() - windowWidth) / 2;
    int y = (screenGeometry.height() - windowHeight) / 2;
    // 창의 위치 설정
    move(x, y);
}
//아이디중복확인(의사용)
void Widget::idCheck_Doctor()
{
    QMessageBox msgbox;
    msgbox.setIcon(QMessageBox::NoIcon);
    msgbox.setWindowTitle("아이디중복확인");
    if(ui->id_doctor->text().trimmed().isEmpty())
    {
        msgbox.setText("아이디를 입력해주세요");
        msgbox.exec();
    }
    else if(ui->id_doctor->text().trimmed().size() <= 6)
    {
        msgbox.setText("영문과 숫자를 조합하여 6자 이상을 입력해주세요.");
        msgbox.exec();
    }
    else if (ui->id_doctor->text().trimmed().size() > 6)
    {
        QString id = ui->id_doctor->text().trimmed();
        QString SendTexts = "JOIN|IDDUPLICATE|DOCTOR|" + id ;
        m_socket->write(SendTexts.toUtf8());
        m_socket->flush();
    }
}
//아이디중복확인(환자용)
void Widget::idCheck_Patient()
{
    QMessageBox msgbox;
    msgbox.setIcon(QMessageBox::NoIcon);
    msgbox.setWindowTitle("아이디중복확인");
    if(ui->id_patient->text().trimmed().isEmpty())
    {
        msgbox.setText("아이디를 입력해주세요");
        msgbox.exec();
    }
    else if(ui->id_patient->text().trimmed().size() <= 6)
    {
        msgbox.setText("영문과 숫자를 조합하여 6자 이상을 입력해주세요.");
        msgbox.exec();
    }
    else if (ui->id_patient->text().trimmed().size() > 6)
    {
        QString id_data = "JOIN|IDDUPLICATE|PATIENT|" + ui->id_patient->text();
        m_socket -> write(id_data.toUtf8());
    }
}

//아이디찾기 버튼
void Widget::idSearch()
{
    QMessageBox msgbox;
    msgbox.setIcon(QMessageBox::NoIcon);
    msgbox.setWindowTitle("아이디 찾기");
    if(ui->find_name->text().trimmed().isEmpty())//만약 빈칸이라면
    {
        msgbox.setText("이름을 입력해주세요.");
        msgbox.exec();
    }
    else if(ui->jumin_first->text().trimmed().isEmpty()||ui->jumin_second->text().trimmed().isEmpty())//만약 빈칸이라면
    {
        msgbox.setText("주민번호를 입력해주세요.");
        msgbox.exec();
    }
    else
    {
        QString name = ui->find_name->text().trimmed();
        QString jumin_number = ui->jumin_first->text().trimmed() + "-" + ui->jumin_second->text().trimmed();
        QString SendTexts = "FIND|ID|" + name + "|" + jumin_number;
        m_socket -> write(SendTexts.toUtf8());
        m_socket -> flush();
    }
}

//비번찾기 버튼
void Widget::pwSearch()
{
    QMessageBox msgbox;
    msgbox.setIcon(QMessageBox::NoIcon);
    msgbox.setWindowTitle("비밀번호 찾기");
    if(ui->searchpw_ID->text().trimmed().isEmpty())
    {
        msgbox.setText("아이디를 입력해주세요.");
        msgbox.exec();
    }
    else if(ui->searchpw_name->text().trimmed().isEmpty())
    {
        msgbox.setText("이름을 입력해주세요.");
        msgbox.exec();
    }
    else if(ui->searchpw_jumin_first->text().trimmed().isEmpty()||ui->searchpw_jumin_second->text().trimmed().isEmpty())
    {
        msgbox.setText("주민번호를 입력해주세요.");
        msgbox.exec();
    }
    else
    {
        QString id = ui->searchpw_ID->text().trimmed();
        QString name = ui->searchpw_name->text().trimmed();
        QString jumin_number = ui-> searchpw_jumin_first->text().trimmed() + "-" + ui->searchpw_jumin_second->text().trimmed();
        QString SendTexts = "FIND|PW|" + id  + "|"+ name + "|" + jumin_number;
        m_socket ->write(SendTexts.toUtf8());
        m_socket -> flush();
    }
}
//입력한 비밀번호 일치여부 확인하여 텍스트 출력 및 색깔변경함수(환자)
void Widget::Patient_PW_Text_Changed()
{
    if(ui->pwconfirm_patient->text().trimmed().isEmpty())
    {
        ui->pwpatient_text->setText("");
    }
    else if(ui->pw_patient->text().trimmed()== ui->pwconfirm_patient->text().trimmed())
    {
        ui->pwpatient_text->setText("비밀번호일치");
        ui->pwpatient_text->setStyleSheet("color:blue");
    }
    else if(ui->pw_patient->text().trimmed() != ui->pwconfirm_patient->text().trimmed())
    {
        ui->pwpatient_text->setText("비밀번호 불일치");
        ui->pwpatient_text->setStyleSheet("color:red");
    }
}
//입력한 비밀번호 일치여부 확인하여 텍스트 출력 및 색깔변경함수(의사)
void Widget::Doctor_PW_Text_Changed()
{
    if(ui->pw_doctor->text().trimmed().isEmpty())
    {
        ui->pwdoctor_text->setText("");
    }
    else if(ui->pw_doctor->text().trimmed()== ui->pwconfirm_doctor->text().trimmed())
    {
        ui->pwdoctor_text->setText("비밀번호일치");
        ui->pwdoctor_text->setStyleSheet("color:blue");
    }
    else if(ui->pw_doctor->text().trimmed() != ui->pwconfirm_doctor->text().trimmed())
    {
        ui->pwdoctor_text->setText("비밀번호 불일치");
        ui->pwdoctor_text->setStyleSheet("color:red");
    }
}
//회원가입 완료버튼(환자용)
void Widget::Patient_Join_Complete()
{
    QMessageBox msgbox;
    QString gender = "1";
    msgbox.setWindowTitle("회원가입(환자용)");

    if(ui->id_patient->text().trimmed().isEmpty())
    {
        msgbox.setText("아이디를 입력해주세요.");
        msgbox.exec();
    }
    else if(ui->pw_patient->text().trimmed().size()<7)
    {
        msgbox.setText("비밀번호를 최소 8자리 이상 입력해주세요.");
        msgbox.exec();
    }
    else if (ui->pw_patient->text().trimmed().size()>7)
    {
        pwcheck_patient = 1;
    }
    else if(ui->pw_patient->text().trimmed() != ui->pwconfirm_patient->text().trimmed())
    {
        msgbox.setText("비밀번호를 확인해주세요.");
        msgbox.exec();
    }
    else if(ui->name_patient->text().trimmed().isEmpty())
    {
        msgbox.setText("이름을 입력해주세요.");
        msgbox.exec();
    }
    else if(ui->jumin_patient_first->text().trimmed().isEmpty()||ui->jumin_patient_second->text().trimmed().isEmpty())
    {
        msgbox.setText("주민번호를 입력해주세요.");
        msgbox.exec();
    }
    else if(ui->email_patient->text().trimmed().isEmpty())
    {
        msgbox.setText("이메일을 확인해주세요.");
        msgbox.exec();
    }
    else if(ui->hpmiddle_patient->text().trimmed().isEmpty()||ui->hplast_patient->text().trimmed().isEmpty())
    {
        msgbox.setText("핸드폰번호를 확인해주세요.");
        msgbox.exec();
    }
    else if(!ui->gender_female->isChecked() && !ui->gender_male->isChecked())
    {
        msgbox.setText("성별을 선택해주세요.");
        msgbox.exec();
    }
    else if(ui->gender_female->isChecked()|| ui->gender_male->isChecked())
    {
        if(ui->gender_male->isChecked())
        {
            gender = "1";
        }
        else if(ui->gender_female->isChecked())
        {
            gender = "2";
        }
    }
    if (totalcheck_patient == 0)
    {
        msgbox.setText("아이디 중복확인을 체크해주세요.");
    }
    else if (totalcheck_patient == 1 && pwcheck_patient == 1)
    {
        QString find_doctor_num;
        QString find_hospital_num;
        for (int i = 0; i < hospital_list.size(); i++)
        {
            if (ui -> hospital -> currentText() == hospital_list[i])
            {
                find_hospital_num = hospital_list[i-1];
            }
        }
        for (int i = 0; i < doctor_list.size(); i++)
        {
            if (ui -> mydoctor -> currentText() == doctor_list[i])
            {
                find_doctor_num = doctor_list[i-1];
            }
        }
        QString id = ui->id_patient->text().trimmed();
        QString pw = ui->pw_patient->text().trimmed();
        QString name = ui->name_patient->text().trimmed();
        QString jumin_number = ui->jumin_patient_first->text().trimmed() + ui->jumin_patient_second->text().trimmed();
        QString email = ui-> email_patient->text().trimmed() + "@" + ui->e_address_patient->currentText();
        QString phone= ui->hpfirst_patient->currentText() + "-" +ui->hpmiddle_patient->text().trimmed() + "-" + ui->hplast_patient->text().trimmed();
        QString Gender = gender;
        QString data = "JOIN|PATIENT|" + id + "|" + pw + "|" + name + "|" + jumin_number +"|" + email + "|" + phone +  "|" + Gender + "|" + find_doctor_num + "|" + find_hospital_num;
        m_socket -> write(data.toUtf8()); //서버에 데이터 보내기
        m_socket -> flush();
        Move_Page(0);
    }
}
//회원가입 완료버튼(의사용)
void Widget::Doctor_Join_Complete()
{
    QString gender = "1";
    QMessageBox msgbox;
    msgbox.setWindowTitle("회원가입(의사용)");
    if(ui->id_doctor->text().trimmed().isEmpty())
    {
    msgbox.setText("아이디를 입력해주세요.");
    msgbox.exec();
    }
    else if(ui->pw_doctor->text().trimmed().size()<7)
    {
        msgbox.setText("비밀번호를 최소 8자리 이상 입력해주세요.");
        msgbox.exec();
    }
    else if(ui->pw_doctor->text().trimmed() != ui->pwconfirm_doctor->text().trimmed())
    {
        msgbox.setText("비밀번호를 확인해주세요.");
        msgbox.exec();
    }
    else if(ui->name_doctor->text().trimmed().isEmpty())
    {
        msgbox.setText("이름을 입력해주세요.");
        msgbox.exec();
    }
    else if(ui->jumin_doctor_first->text().trimmed().isEmpty()||ui->jumin_doctor_second->text().trimmed().isEmpty())
    {
        msgbox.setText("주민번호를 입력해주세요.");
        msgbox.exec();
    }
    else if(ui->email_doctor->text().trimmed().isEmpty())
    {
        msgbox.setText("이메일을 확인해주세요.");
        msgbox.exec();
    }
    else if(ui->hpmiddle_doctor->text().trimmed().isEmpty()||ui->hplast_doctor->text().trimmed().isEmpty())
    {
        msgbox.setText("핸드폰번호를 확인해주세요.");
        msgbox.exec();
    }
    else if(ui->lineEdit_doctornum->text().trimmed().isEmpty())
    {
        msgbox.setText("의사면허번호를 입력해주세요.");
        msgbox.exec();
    }
    else if(ui->lineEdit_doctornum->text().size()!= 6)
    {
        msgbox.setText("의사면허번호는 6자리입니다. 다시 확인해주세요.");
        msgbox.exec();
    }
    //파일첨부를 안했다면 확인메시지 추가하기
    else if(ui->show_doctorimg->pixmap().isNull())
    {
        msgbox.setText("의사면허증 이미지를 첨부해주세요.");
        msgbox.exec();
    }
    else if(totalcheck_doctor < 3)
    {
        msgbox.setText("중복체크 및 면허확인을 확인해주세요.");
        msgbox.exec();
    }
    else
    {
        QString find_hospital_num;
        for (int i = 0; i < hospital_list.size(); i++)
        {
            if (ui -> comboBox -> currentText() == hospital_list[i])
            {
                find_hospital_num = hospital_list[i-1];
            }
        }
        QString id = ui->id_doctor->text().trimmed();
        QString pw = ui->pw_doctor->text().trimmed();
        QString name = ui->name_doctor->text().trimmed();
        QString jumin_number = ui->jumin_doctor_first->text().trimmed() + ui->jumin_doctor_second->text().trimmed();
        QString email = ui-> email_doctor->text().trimmed() + "@" + ui->e_address_doctor->currentText();
        QString hp = ui-> hpfirst_doctor->currentText() + ui->hpmiddle_doctor->text().trimmed() + ui->hplast_doctor->text().trimmed();
        QString data = "JOIN|DOCTOR|" + id + "|" + pw + "|" + name + "|" + jumin_number +"|" + email + "|" + hp + "|" + gender + "|0|" + find_hospital_num;
        m_socket -> write(data.toUtf8()); //서버에 데이터 보내기
        m_socket -> flush();
        Move_Page(0);
    }
}
//의사면허증 (open file Dialog)
void Widget::Doctor_image()
{
    totalcheck_doctor++;
    QString imagePath = QFileDialog::getOpenFileName(this, "Open Image", "", "Images (*.png *.jpg *.jpeg *.bmp)");
    if(!imagePath.isEmpty()){
        QPixmap pixmap(imagePath);
        ui->show_doctorimg->setPixmap(pixmap);
        ui->show_doctorimg->setScaledContents(true);
        ui->show_doctorimg->show();
     }
}
//의사면허증 6자리 번호 확인 함수
void Widget::Check_DoctorLiscense()
{
     QMessageBox msgbox;
     totalcheck_doctor++;
     if(ui->lineEdit_doctornum->text().size() == 6)
     {
        msgbox.setText("의사면허번호가 정상적으로 확인되었습니다.");
        msgbox.exec();
     }
     else
     {
        msgbox.setText("의사면허번호는 6자리입니다. 다시 확인해주세요.");
        msgbox.exec();
     }
}
//이게 메인 // 서버로부터 데이터 읽어오기
void Widget::readMessage()
{
    QMessageBox msgbox;
    QByteArray ReadData;
    QByteArray buffer;
    if (m_socket->bytesAvailable() > 1000)
    {
        while (true)
        {
            ReadData = m_socket->read(1024); // 1024바이트씩 데이터 수신
            buffer.append(ReadData);
            if (ReadData.isEmpty())
            {
                break;
            }
        }
        // 수신한 데이터가 최소한 헤더 크기 이상인지 확인
        if (buffer.size() >= 128)
        {
            QByteArray header = buffer.left(128); // 헤더 추출
            QString headerStr = QString::fromUtf8(header); // QByteArray를 QString으로 변환
            split = headerStr.split("|");
            QString Data_Size = split[10];  // 이미지 크기
            imageData = buffer.mid(128); // 이미지 데이터 추출
            QString str_int = QString::number(imageData.size());
            if(Data_Size == str_int)
            {
                m_socket->write(QString("FILE_COMPLETE").toUtf8());
                m_socket->flush();
            }
        }
    }
    else
    {
        ReadData = m_socket->readAll();
    }
    QString user_info = QString::fromUtf8(ReadData);
    QStringList result = user_info.split("|");
    if(user_info == "Start_next_task")
    {
        QDialog dialog;
        QImage image;
        dialog.setFixedSize(416,780);
        dialog.setStyleSheet("background-color: lightblue;");
        dialog.setWindowTitle("데이터 보기");

        if (image.loadFromData(imageData))
        {
            QDialog *dialog = new QDialog(this);
            dialog->setWindowTitle("분석결과");
            dialog->setFixedSize(400, 500);
            layout = new QVBoxLayout(dialog);
            QPixmap pixmap = QPixmap::fromImage(image);
            QLabel *label = new QLabel("사진", dialog);
            label->setStyleSheet(
                "color:black;"
                "background-color: rgb(255, 255, 255);"
                "border: 1px solid rgb(255, 255, 255);"
                "border-radius: 13px;"
                "padding: 1px 10px;"
                );
            label->setFixedSize(350, 350);
            label->setPixmap(pixmap.scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            label->show();
            // X, Y, Z 값에 대한 레이블 및 입력 필드
            QWidget *boxWidget = new QWidget(dialog);
            boxWidget->setLayout(new QHBoxLayout(boxWidget));
            QWidget *boxWidget1 = new QWidget(dialog);
            boxWidget1->setLayout(new QHBoxLayout(boxWidget1));
            QWidget *boxWidget2 = new QWidget(dialog);
            boxWidget2->setLayout(new QHBoxLayout(boxWidget2));
            QWidget *boxWidget3 = new QWidget(dialog);
            boxWidget3->setLayout(new QHBoxLayout(boxWidget3));
            QLabel *label1 = new QLabel("x1 : " + split[1], dialog);
            QLabel *label2 = new QLabel("x2 : " + split[4], dialog);
            QLabel *label3 = new QLabel("x3 : " + split[7], dialog);
            QLabel *label4 = new QLabel("y1 : " + split[2], dialog);
            QLabel *label5 = new QLabel("y2 : " + split[5], dialog);
            QLabel *label6 = new QLabel("y3 : " + split[8], dialog);
            QLabel *label7 = new QLabel("z1 : " + split[3], dialog);
            QLabel *label8 = new QLabel("z2 : " + split[6], dialog);
            QLabel *label9 = new QLabel("z3 : " + split[9], dialog);
            boxWidget->layout()->addWidget(label1);   //x축
            boxWidget->layout()->addWidget(label2);   //x축
            boxWidget->layout()->addWidget(label3);   //x축
            boxWidget1->layout()->addWidget(label4);  //y축
            boxWidget1->layout()->addWidget(label5);  //y축
            boxWidget1->layout()->addWidget(label6);  //y축
            boxWidget2->layout()->addWidget(label7);  //z축
            boxWidget2->layout()->addWidget(label8);  //z축
            boxWidget2->layout()->addWidget(label9);  //z축
            boxWidget3->layout()->addWidget(label);   //이미지
            layout->addWidget(boxWidget3);            //이미지
            layout->addWidget(boxWidget);             //x축
            layout->addWidget(boxWidget1);            //y축
            layout->addWidget(boxWidget2);            //z축
            // "확인" 버튼
            QPushButton *OkayButton = new QPushButton("확인", dialog);
            connect(OkayButton, &QPushButton::clicked, [this, dialog]() {
                dialog->reject();
            });
            OkayButton->setStyleSheet(
                "color: rgb(255, 255, 255);"
                "background-color: rgb(62, 164, 175);"
                "border: 3px solid rgb(255, 255, 255);"
                "border-radius: 13px; "
                "font: 700 15pt 'Gaegu';"
                "padding: 1px 10px;"
                );
            layout->addWidget(OkayButton);
            dialog->show();
        }
        imageData.isNull();
    }
    else if(result[0] =="LOGINO") //로그인 성공
        {
            if(result[1] =="PATIENT")//환자용 로그인 성공
            {
                user_num = result[2];
                ui -> label_48 -> setText(result[4] + "님 안녕하세요.");
                Move_Page(6); //환자 UI이동
                QString today = today_Tm();
                QString go_msg = "SCHEDULE|" + today + "|" + user_num;
                m_socket -> write(go_msg.toUtf8());
                if (result[3] == "O")
                {
                    ui -> label_49 -> setText("오늘의 일지 작성 완료!");
                }
                else if (result[3] == "X")
                {
                    ui -> label_49 -> setText("일지를 작성해주세요.");
                }
                QString quote = result[5];//원본 인용문
                QString author = result[6];//원본 저자
                if(quote.length()>32)
                {
                    quote.insert(32,'\n');
                }
                quote.prepend("오늘의 명언\n");
                quote.append(author);
                ui->quote_patient->setText(quote);
            }
            else if(result[1] =="DOCTOR") //의사용 로그인 성공
            {
                user_num = result[2];
                QString today = today_Tm();
                ui -> label_36 -> setText(result[4] + "님 안녕하세요.");
                QString go_msg = "SCHEDULE|" + today + "|" + user_num;
                m_socket -> write(go_msg.toUtf8());

                QString quote = result[5];//원본 인용문
                QString author = result[6];//원본 저자
                if(quote.length()>32)
                {
                    quote.insert(32,'\n');
                }
                quote.prepend("오늘의 명언\n");
                quote.append(author);
                ui->quote_doctor->setText(quote);

                Move_Page(9); //의사 UI이동
            }
        }
    else if(result[0] == "LOGINX") //로그인 실패
        {
            msgbox.setIcon(QMessageBox::NoIcon);
            msgbox.setWindowTitle("로그인");
            msgbox.setText("아이디나 비밀번호를 확인해주세요.");
            msgbox.exec();
        }
    else if (result[0] == "LOGIN_DOUBLE")
        {
            QMessageBox::information(this, "경고", "이미 로그인 되어있는 상태입니다.");
        }
    else if(result[0] == "IDDUPLICATE") //아이디 중복체크
        {
            if(result[1] == "PATIENT") //환자용 아이디 중복체크
            {
                if(result[2] == "IDCHECKO") //아이디 중복X, 사용가능
                {
                    totalcheck_patient = 1;
                    msgbox.setWindowTitle("아이디 중복확인");
                    msgbox.setText("사용가능한 아이디입니다.");
                    msgbox.exec();

                    ui->id_patient->setReadOnly(true);
                    ui->idpatient_chkBtn->setEnabled(false);
                    ui->idpatient_chkBtn->setStyleSheet("background-color:rgb(155, 155, 155); color:white;");
                }
                else if (result[2] == "IDCHECKX")
                {
                    msgbox.setWindowTitle("아이디 중복확인");
                    msgbox.setText("중복된 아이디입니다.");
                    msgbox.exec();
                }
            }
            else if(result[1] == "DOCTOR") //의사용 아이디 중복체크
            {
                if(result[2] == "IDCHEKCX") //아이디 중복 O, 사용불가
                {
                msgbox.setWindowTitle("아이디 중복확인");
                msgbox.setText("중복된 아이디가 있습니다.");
                msgbox.exec();
                }
                else if(result[2] == "IDCHEKCO") //아이디 중복 O, 사용불가
                {
                msgbox.setWindowTitle("아이디 중복확인");
                msgbox.setText("중복된 아이디가 있습니다.");
                msgbox.exec();
                }

            }
        }
    else if(result[0] == "IDO") // ID조회 가능
        {
            ui->find_id->setText("회원님의 아이디는 " + result[1] + "입니다.");
        }
    else if(result[0] == "IDX") //ID조회 불가능,ID 조회 불가
        {
            ui->find_id->setText("아이디가 존재하지 않습니다.");
        }
    else if(result[0] == "PWO") //PW조회 가능
        {
            ui->find_pw->setText("회원님의 비밀번호는 " + result[1] + "입니다.");
        }
    else if(result[0] == "PWX") //PW조회 불가
        {
            ui->find_pw->setText("검색하신 아이디나 이름, 주민번호를 다시 확인해주세요.");
        }
    else if(result[0] == "JOINO")
        {
            msgbox.setWindowTitle("회원가입");
            msgbox.setText("회원가입이 완료되었습니다.\n 로그인 화면으로 이동합니다.");
            msgbox.exec();
            Move_Page(0);
        }
    else if(result[0] == "JOINX")
        {
            msgbox.setWindowTitle("회원가입");
            msgbox.setText("회원가입 실패");
            msgbox.exec();
        }
    else if (result[0] == "SCHEDULE")
        {
            if (select_human == "1")
            {
                myFrame = findChild<QFrame*>("frame");
                myFrame->setStyleSheet("QFrame { background-color: transparent; border: none; }");
            }
            else if (select_human == "2")
            {
                myFrame = findChild<QFrame*>("frame_2");
                myFrame->setStyleSheet("QFrame { background-color: transparent; border: none; }");
            }
            int x = 9;
            int y = 9;
            int width = 320;
            int height = 40;
            int title = 1;
            int totalHeigt = 0;
            if (myFrame)
            {
                for (int i = 1; i <= (result.size()-1)/3; i++)
                {
                myButton = new QPushButton(result[title], myFrame);
                myButton->setObjectName("Button");
                // 임호진이 수정함 11.03 (모든 일정)
                myButton->setStyleSheet("QPushButton {"
                                        "color: black;"
                                        "font: 700 18pt 'Gaegu';"
                                        "background-color: rgb(255, 255, 255);"
                                        "border: 3px solid rgb(255, 255, 255);"
                                        "border-radius: 13px;"
                                        "padding: 1px 10px;"
                                        "}");
                myButton->setGeometry(x, y, width, height);  // x, y, width, height
                // 임호진이 수정 종료함
                connect(myButton, &QPushButton::clicked, this, [i, result, title, this]()
                {
                    QString msg1 = "SCHEDULE_DETAIL|" + result[title] + "|" + user_num + "|" + QString::number(i);
                    m_socket -> write(msg1.toUtf8());
                    ui -> stackedWidget -> setCurrentIndex(13);
                });
                totalHeigt += height + 10;
                y += 50;
                title += 3;
                myButton -> show();
                }
                myFrame -> setFixedHeight(totalHeigt);
            }
        }
    else if (result[0] == "SCHEDULEX")
        {
            if (select_human == "1")
            {
                myFrame = findChild<QFrame*>("frame");
                myFrame->setStyleSheet("QFrame { background-color: transparent; border: none; }");
            }
            else if (select_human == "2")
            {
                myFrame = findChild<QFrame*>("frame_2");
                myFrame->setStyleSheet("QFrame { background-color: transparent; border: none; }");
            }
            if (myFrame)
            {
                int x = 9;
                int y = 9;
                int width = 320;
                int height = 40;
                myButton = new QPushButton("등록된 일정이 없습니다.", myFrame);
                myButton->setStyleSheet("QPushButton {"
                                        "color: black;"
                                        "font: 700 18pt 'Gaegu';"
                                        "background-color: rgb(255, 255, 255);"
                                        "border: 3px solid rgb(255, 255, 255);"
                                        "border-radius: 13px;"
                                        "padding: 1px 10px;"
                                        "}");
                myButton->setGeometry(x, y, width, height);
                myButton -> show();
            }
        }
    else if (result[0] == "SCHEDULE_DETAIL")
        {
            ui -> label_5 -> setText(result[2] + "의 일정");
            ui -> textEdit -> setText(result[1]);
            ui -> textEdit_2 -> setText(result[3]);
            ui -> textEdit_3 -> setText(result[4]);
        }
    else if (result[0] == "SCHEDULE_INSERT")
        {
            if (result[1] == "SUCCESS")
            {
                QMessageBox::information(this, "일정 등록", "일정이 등록되었습니다.");
                QString today = today_Tm();
                QString go_msg = "SCHEDULE|" + today + "|" + user_num;
                m_socket -> write(go_msg.toUtf8());
                if (select_human == "1")
                {
                    ui -> stackedWidget -> setCurrentIndex(6);
                }
                else if (select_human == "2")
                {
                    Move_Page(9);
                }
            }
            else
            {
                QMessageBox::information(this, "일정 등록", "일정 등록에 실패했습니다.");
            }
        }
    else if (result[0] == "SCHEDULE_UPDATE")
        {
            if (result[1] == "SUCCESS")
            {
                QMessageBox::information(this, "일정 업데이트", "일정이 업데이트 되었습니다.");
                QString today = today_Tm();
                QString go_msg = "SCHEDULE|" + today + "|" + user_num;
                m_socket -> write(go_msg.toUtf8());
                if (select_human == "1")
                {
                    ui -> stackedWidget -> setCurrentIndex(6);
                }
                else if (select_human == "2")
                {
                    Move_Page(9);
                }
            }
            else
            {
                QMessageBox::information(this, "일정 업데이트", "일정 업데이트에 실패하였습니다.");
            }
        }
    else if (result[0] == "SCHEDULE_DELETE")
        {
            if (result[1] == "SUCCESS")
            {
                QMessageBox::information(this, "일정 삭제", "일정 삭제 성공");
                QString go_msg = "SCHEDULE|" + day_date + "|" + user_num;
                m_socket -> write(go_msg.toUtf8());

                if (select_human == "1")
                {
                    ui -> stackedWidget -> setCurrentIndex(6);
                }
                else if (select_human == "2")
                {
                    Move_Page(9);
                }
            }
            else
            {
                QMessageBox::information(this, "일정 삭제", "일정 삭제 실패");
            }
        }
    else if (result[0] == "Q&A")
        {
            if (result[1] == "CHECK")
            {
                if (result[2] == "X")
                {

                }
                else
                {
                    ui -> answer1 -> setText(result[2]);
                    ui -> answer2 -> setText(result[3]);
                    ui -> answer3 -> setText(result[4]);
                    ui->answer1->setReadOnly(true);
                    ui->answer2->setReadOnly(true);
                    ui->answer3->setReadOnly(true);
                    ui -> close_myday_Btn -> hide();
                }
            }
            else if (result[1] == "SUCCESS")
            {
                ui->label_49->setText("오늘의 일지 작성 완료!");
            }
            else if (result[1] == "FAILED")
            {

            }
        }
    else if (result[0] == "DOCTOR_LIST")
        {
            ui -> mydoctor -> clear();
            doctor_list.clear();
            for (int i = 1; i <= result.size()-1; i++)
            {
                doctor_list.append(result[i]);
            }
            for (int j = 1; j <= doctor_list.size(); j+=2)
            {
                ui -> mydoctor -> addItem(doctor_list[j]);
            }
        }
    else if (result[0] == "HOSPITAL")
        {
            for (int i = 1; i <= result.size()-1; i++)
            {
                hospital_list.append(result[i]);
            }
            for (int j = 1; j < hospital_list.size(); j+=2)
            {
                ui -> hospital -> addItem(hospital_list[j]);
                ui -> comboBox -> addItem(hospital_list[j]);
            }
            if (select_num == 1)
            {
                Move_Page(2);
            }
            else if (select_num == 2)
            {
                Move_Page(3);
            }
        }
    else if (result[0] == "PATIENT_LIST")
        {
            // 초기화
            patient_num_list.clear();
            name_list.clear();
            phone_list.clear();
            age_list.clear();
            gender_list.clear();

            myFrame = findChild<QFrame*>("frame_3");
            myFrame->setStyleSheet("QFrame { background-color: transparent; border: none; }");
            int x = 0;
            int y = 0;
            int width = 351;
            int height = 60;
            int title = 1;
            int totalHeigt = 0;
            for (int j = 1; j < (result.size()-1); j+=5)
            {
                patient_num_list.append(result[j]);
                name_list.append(result[j+1]);
                phone_list.append(result[j+2]);
                gender_list.append(result[j+3]);
                age_list.append(result[j+4]);
            }
            if (myFrame)
            {
                for (int i = 0; i < (result.size()-1)/5; i++)
                {
                    QString ButtonText = QString("%1 (%2)   %3\n%4")
                                         .arg(name_list[i])
                                         .arg(age_list[i])
                                         .arg(gender_list[i])
                                         .arg(phone_list[i]);
                    myButton = new QPushButton(ButtonText, myFrame);
                    myButton->setParent(myFrame);
                    myButton->setStyleSheet("QPushButton {"
                                            "color: black;"
                                            "font: 700 12pt 'Gaegu';"
                                            "background-color: rgb(255, 255, 255);"
                                            "border: 3px solid rgb(255, 255, 255);"
                                            "border-radius: 13px;"
                                            "padding: 1px 10px;"
                                            "}");
                    myButton->setGeometry(x, y, width, height);
                    totalHeigt += height + 20;
                    y += 80;
                    title += 3;
                    connect(myButton, &QPushButton::clicked, this, [i, this]()
                            {
                                QString msg = "PATIENT_QUESTION|" + user_num + "|" + patient_num_list[i];
                                m_socket -> write(msg.toUtf8());
                                ui -> label_70 -> setText(name_list[i] + "님의" + day_date + "일지입니다.");
                                select_patient_num = patient_num_list[i];
                            });
                    myButton -> show();
                }
            myFrame -> setFixedHeight(totalHeigt);
            }
            for (int j = 0; j < name_list.size(); j++)
            {
                ui -> user_list -> addItem(name_list[j]);
            }
            Move_Page(10);
        }
    else if (result[0] == "HISTORY")
    {
         if (result[1] == "X")
        {
            QMessageBox::information(this, "알림", "등록된 일지가 없습니다.");
        }
        else
        {
            QString question1 = result[1];
            QString question2 = result[2];
            QString question3 = result[3];
            ui -> answer1_2 -> setText(question1);
            ui -> answer2_2 -> setText(question2);
            ui -> answer3_2 -> setText(question3);
            ui -> stackedWidget -> setCurrentIndex(8);
        }
    }
    else if (result[0] == "MYINFO")  //내정보
    {
        QString id = result[1];
        QString pw = result[2];
        QString name = result[3];
        QString hp = result[4];
        QString email = result[5];
        QString jumin = result[6];
        ui -> id_textEdit -> setText(id);
        ui -> pw_textEdit -> setText(pw);
        ui -> name_textEdit -> setText(name);
        ui -> ph_textEdit -> setText(hp);
        ui -> email_textEdit -> setText(email);
        ui -> jumin_textEdit -> setText(jumin);
    }
    else if (result[0] == "SECESSTION")
    {
        if (result[1] == "FINISH") //탈퇴완료
        {
            dialog->close();
            QMessageBox::information(this, "알림", "성공적으로 회원탈퇴되었습니다.");
            Move_Page(0);
        }
        else if (result[1] == "PWERROR")  //탈퇴 실패
        {
            QMessageBox::information(this, "알림", "비밀번호가 일치하지 않습니다.");
        }
    }
    else if (result[0] == "CHAT_SUCCESS")
    {
        if (result[1] == "ALARM")
        {
            QString msg = result[2];
            QMessageBox::information(this, "알림", msg);
        }
        else if (result[1] == "DOCTOR")
        {
            QMessageBox::information(this, "알림", "알림이 전송되었습니다.");
        }
    }
    else if (result[0] == "CHAT_FAIL")
    {
        QMessageBox::information(this, "알림", "알림이 전송이 실패하였습니다.");
    }
}

//캘린더 클릭 후 이동 및 정보 받아오기
void Widget::calender_going()
{
    ui -> label -> setText(day_date + "의 일정 등록");
    Move_Page(7);
}
//일정 업데이트
void Widget::calender_update()
{
     Move_Page(14);
     QString title = ui -> textEdit -> toPlainText();
     QString space = ui -> textEdit_2 -> toPlainText();
     QString memo = ui -> textEdit_3 -> toPlainText();
     ui -> title_patient_2 -> setText(title);
     ui -> space_patient_2 -> setText(space);
     ui -> memo_patient_2 -> setText(memo);
     ui -> label_2 -> setText(day_date + "의 일정 등록");
}
//일정 업데이트
void Widget::calender_update_success()
{
     delete myButton;
     QString back_title = ui -> textEdit -> toPlainText() + "|";
     QString title = ui -> title_patient_2 -> toPlainText() + "|";
     QString space = ui -> space_patient_2 -> toPlainText() + "|";
     QString memo = ui -> memo_patient_2 -> toPlainText();
     QString msg = "SCHEDULE_UPDATE|" + day_date + "|" + back_title + title + space + memo;
     m_socket -> write(msg.toUtf8());
}
//캘린더 클릭
void Widget::on_calendarWidget_clicked(const QDate &date)
{
     delete myButton;
     day_date = date.toString("yyyy-MM-dd");
     QString msg = "SCHEDULE|" + day_date + "|" + user_num;
     m_socket -> write(msg.toUtf8());

}
// 스케줄 삭제
void Widget::delete_schedule()
{
    int choice = QMessageBox::question(this, "삭제", "정말 삭제하시겠습니까?", QMessageBox::Ok | QMessageBox::Cancel);
    if (choice == QMessageBox::Ok)
    {
        QString title = ui -> textEdit -> toPlainText();
        QString SendMSG = QString("SCHEDULE_DELETE|%1|%2|%3").arg(user_num).arg(day_date).arg(title);
        m_socket->write(SendMSG.toUtf8());
        m_socket->flush();
    }
    else if (choice == QMessageBox::Cancel)
    {
        ui->stackedWidget->setCurrentIndex(6);
    }
}
//일지 등록
void Widget::close_myday()
{
    if (ui -> answer1 -> toPlainText().isEmpty() || ui -> answer2 -> toPlainText().isEmpty() || ui -> answer3 -> toPlainText().isEmpty())
    {
        QMessageBox::information(this, "알림", "모든 질문에 답변해주세요.");
    }
    else
    {
        QString day = today_Tm() + "|";
        QString msg1 = ui -> answer1 -> toPlainText() + "|";
        QString msg2 = ui -> answer2 -> toPlainText() + "|";
        QString msg3 = ui -> answer3 -> toPlainText();
        QString data = "Q&A|INSERT|"
                       "" + user_num + "|" + day + msg1 + msg2 + msg3;

        m_socket -> write(data.toUtf8()); //서버에 데이터 보내기
        m_socket -> flush();
        QMessageBox::information(this, "알림", "오늘도 당신은 아름다운 사람입니다.");
        ui->answer1->setReadOnly(true);
        ui->answer2->setReadOnly(true);
        ui->answer3->setReadOnly(true);
        ui -> close_myday_Btn -> hide();
    }
}
//일정 등록
void Widget::save_myschedule()
{
    delete myButton;
    QString TEXT1 = ui->title_patient->toPlainText();
    QString TEXT2 = ui->space_patient->toPlainText();
    QString TEXT3 = ui->memo_patient->toPlainText();
    if(TEXT1.isEmpty())
    {
        QMessageBox::warning(this, "일정 등록", "제목을 입력해주세요");
    }
    else if(TEXT2.isEmpty())
    {
        QMessageBox::warning(this, "일정 등록", "장소를 입력해주세요");
    }
    else if(TEXT3.isEmpty())
    {
        QMessageBox::warning(this, "일정 등록", "내용을 입력해주세요");
    }
    else
    {
        QString day = day_date + "|";
        QString title = ui->title_patient-> toPlainText() + "|";
        QString space = ui->space_patient-> toPlainText() + "|";
        QString memo = ui-> memo_patient-> toPlainText();
        QString data = "SCHEDULE_ADD|" + user_num + "|" + day + title + space + memo;
        m_socket -> write(data.toUtf8()); //서버에 데이터 보내기
        m_socket -> flush();
        ui->title_patient->clear();
        ui->space_patient->clear();
        ui->memo_patient->clear();
        ui->alarm_patient->setCurrentIndex(0);
        ui->repeat_patient->setCurrentIndex(0);
    }
}

//로그아웃 버튼
void Widget::on_pushButton_33_clicked()
{
    int choice = QMessageBox::question(this, "로그아웃", "로그아웃하시겠습니까?", QMessageBox::Ok | QMessageBox::Cancel);
    if (choice == QMessageBox::Ok)
    {
        QString msg = "LOGOUT|" + user_num;
        m_socket -> write(msg.toUtf8());
        QMessageBox::information(this, "로그아웃", "로그아웃 되었습니다.");
        Move_Page(0);
    }
}
//환자 리스트 받아오기
void Widget::see_my_patient()
{
    QString msg = "PATIENT_LIST|" + user_num;
    m_socket -> write(msg.toUtf8());
}
//의사 리스트 받아오기(환자용)
void Widget::give_doctor_list()
{
    QString msg = "JOIN|SELECT_PATIENT";
    m_socket -> write(msg.toUtf8());
}
//의사 리스트 받아오기(의사용)
void Widget::give_doctor_list1()
{
    QString msg = "JOIN|SELECT_DOCTOR";
    m_socket -> write(msg.toUtf8());
}
//병원 리스트 받아오기(환자/의사)
void Widget::give_hospital_list()
{
    QString msg = "JOIN|SELECT_HOSPITAL";
    m_socket -> write(msg.toUtf8());
}
//선택한 병원 서버로 전송
void Widget::select_hospital_list()
{
    QString find;
    for (int i = 0; i < hospital_list.size(); i++)
    {
        if (ui -> hospital -> currentText() == hospital_list[i])
        {
            find = hospital_list[i-1];
        }
    }
    QString select_hospital ="JOIN|SELECT_DOCTOR|" + find + "|" + ui -> hospital -> currentText();
    m_socket -> write(select_hospital.toUtf8());
}
//회원가입-환자선택
void Widget::on_join_patient_Btn_clicked()
{
    select_num = 1;
}
//회원가입-의사선택
void Widget::on_join_doctor_Btn_clicked()
{
    select_num = 2;
}
//홈 선택
void Widget::select_home()
{
    if (select_human == "1")
    {
        Move_Page(6);
    }
    else if (select_human == "2")
    {
        Move_Page(9);
    }
}
//환경설정 선택
void Widget::select_setting()
{
    if (select_human == "1")
    {
        QIcon icon("C:\\Users\\code124\\Downloads\\231026\\resources\\book.png");
        ui -> setting_center_menu -> setIcon(icon);
    }
    else if (select_human == "2")
    {
        QIcon icon("C:\\Users\\code124\\Downloads\\231026\\resources\\list.png");
        ui -> setting_center_menu -> setIcon(icon);
    }
    ui -> setting_center_menu -> setIconSize(QSize(75, 75));
    QString msg = "GIVE_MYINFO|" + user_num;
    m_socket -> write(msg.toUtf8());
    Move_Page(12);
}
//가운데 메뉴 선택
void Widget::select_center()
{
    if (select_human == "1")
    {
        Move_Page(11);
    }
    else if (select_human == "2")
    {
        QString msg = "PATIENT_LIST|" + user_num;
        m_socket -> write(msg.toUtf8());
        Move_Page(10);
    }
}
//앱버전
void Widget::on_app_ver_clicked()
{
    QMessageBox::information(this, "App Version", "App Version : 1.0.1\n개발자 : 박영은 이건우 오경석 임호진");
}
//회원탈퇴 다이얼로그
void Widget::secession_detail()
{
    QDialog dialog(this);
    dialog.setWindowTitle("회원탈퇴");
    dialog.setFixedSize(220, 150);
    QWidget *boxWidget = new QWidget(&dialog);
    boxWidget->setLayout(new QHBoxLayout(boxWidget));
    layout = new QVBoxLayout(&dialog);
    label = new QLabel("정말 회원탈퇴하시겠습니까?", &dialog);
    label -> setFixedSize(200, 50);
    QPushButton *YesMyButton = new QPushButton("예", &dialog);
    QPushButton *NoMyButton = new QPushButton("아니오", &dialog);
    connect(YesMyButton, &QPushButton::clicked, this, [&dialog, boxWidget,this, YesMyButton, NoMyButton]() {
        result = QDialog::Accepted;
        if (secesstion_num == 0)
        {
            label->setText("비밀번호를 입력해주세요.");
            edit = new QTextEdit(&dialog);
            edit->setFixedSize(200, 30);

            //edit -> setGeometry(5, 50, 200, 30);
            layout->addWidget(edit);

            layout -> removeWidget(boxWidget);
            layout -> addWidget(boxWidget);

            YesMyButton -> setText("회원탈퇴");
            NoMyButton -> setText("취소");
            secesstion_num = 1;
        }
        else if (secesstion_num == 1)
        {
            QString msg = "SECESSTION|" + user_num + "|" + edit -> toPlainText();
            m_socket->write(msg.toUtf8());
        }

    });
  connect(NoMyButton, &QPushButton::clicked, [&dialog, this, YesMyButton, NoMyButton]() {
      result = QDialog::Rejected;
        dialog.reject();
  });
    boxWidget->layout()->addWidget(YesMyButton);
    boxWidget->layout()->addWidget(NoMyButton);
    layout->addWidget(label);
    layout->addWidget(boxWidget);
    dialog.setLayout(layout);
    result = dialog.exec();
}
//의사->환자로 알림 보내기
void Widget::go_to_message()
{
    QString user_name = ui -> user_list -> currentText();
    QString msg = ui -> chat_text -> text();
    QString send = "CHAT|" + user_name + "|" + msg;
    m_socket -> write(send.toUtf8());
    QString msg1 = user_name + " : " + msg + "\n";
    QString all_msg;
    all_msg += msg1;
    ui -> chat_log -> setText(all_msg);
    ui -> chat_text -> clear();
}
//내담자 일지 보기
void Widget::see_mypatient_detail()
{
    QString msg = "PATIENT_DETAIL|" + select_patient_num;
    m_socket -> write(msg.toUtf8());
}
