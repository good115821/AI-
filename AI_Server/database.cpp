#include "database.h"

Database::Database()
{
    // 생성자인데 안쓰는중
}

// 로그인 함수
void Database::Login(QTcpSocket * senderSKT, QStringList DATA_LIST)
{
    QSqlQuery query;

    QString Type = DATA_LIST[1];    // 1 : 환자, 2 : 의사
    QString user_type = "";
    if(Type == "1") {
        user_type = "PATIENT";
    }else if(Type == "2") {
        user_type = "DOCTOR";
    }

    QString ID = DATA_LIST[2];
    QString PW = DATA_LIST[3];
    query.prepare("SELECT * FROM user WHERE User_ID = :USER_ID AND User_PW = :USER_PW AND User_Type = :Type");
    query.bindValue(":Type", user_type);
    query.bindValue(":USER_ID", ID);
    query.bindValue(":USER_PW", PW);
    query.exec();
    if (query.next())
    {
        qDebug() << "로그인 결과 있음";
        QString SendMSG;
        QString user_num = query.value("User_Num").toString();
        QString user_active = query.value("User_Active").toString();
        QString user_name = query.value("User_Name").toString();
        if(user_active == "0")
        {
            // 중복 방지 업데이트
            query.prepare("UPDATE user SET User_Active = '1' WHERE User_ID = :USER_ID AND User_Type = :Type");
            query.bindValue(":USER_ID", ID);
            query.bindValue(":Type", user_type);
            query.exec();

            // 환자 일지 등록 확인
            QString check = "";
            QDateTime currentDate = QDateTime::currentDateTime();
            QString stringDate = currentDate.toString("yyyy-MM-dd");
            query.prepare("SELECT * FROM patient WHERE User_Num = :user_num AND Patient_Insert_Date = :date");
            query.bindValue(":user_num", user_num);
            query.bindValue(":date", stringDate);
            query.exec();
            if(query.next())
            {
                check = "O";
            }
            else
            {
                check = "X";
            }

            int days = stringDate.mid(8, 2).toInt();

            if(days > 15)
            {
                days -= 15;
            }
            QString text;
            QString author;
            qDebug() << "날짜 :" << days;
            query.prepare("SELECT * FROM life_quotes WHERE Quotes_Num = :day");
            query.bindValue(":day", days);
            query.exec();
            query.next();
            text = query.value("Life_Quotes").toString();
            author = query.value("Quotes_author").toString();
            qDebug() << "text : " << text << "author : " << author;
            SendMSG = QString("LOGINO|%1|%2|%3|%4|%5|%6").arg(user_type, user_num, check, user_name, text, author);
            // 소켓에 포함된 ID Map에 담기
            Qmap_User_Socket_ID.insert(senderSKT, ID);      // 임호진이 추가함
            // 소켓에 포함된 Type Map에 담기
            Qmap_User_ID_Type.insert(ID, user_type);        // 임호진이 추가함
        }
        else if(user_active == "1")
        {
            SendMSG = QString("LOGIN_DOUBLE");
        }
        senderSKT->write(SendMSG.toUtf8());
        senderSKT->flush();
    }
    else
    {
        // 로그인 안됨
        QString SendMSG = QString("LOGINX");
        qDebug() << SendMSG;
        senderSKT->write(SendMSG.toUtf8());
        senderSKT->flush();
    }
}

// 회원가입 함수(경석 수정)
void Database::Join(QTcpSocket * senderSKT, QStringList DATA_LIST)
{
    QSqlQuery query;
    qDebug() << "회원가입 : " << DATA_LIST;
    QString Type = DATA_LIST[1];
    if (Type == "DOCTOR")
    {
        //QString data = "JOIN|DOCTOR|" + id + "|" + pw + "|" + name + "|" + jumin_number +"|" + email + "|" + hp;
        QString NEW_ID = DATA_LIST[2];
        QString NEW_PW = DATA_LIST[3];
        QString NEW_NAME = DATA_LIST[4];
        QString NEW_JUMIN = DATA_LIST[5];
        QString NEW_EMAIL = DATA_LIST[6];
        QString NEW_PHONE = DATA_LIST[7];
        QString NEW_GENDER = DATA_LIST[8];
        QString NEW_DOCTOR_NUM = DATA_LIST[9];
        QString NEW_HOSPITAL_NUM = DATA_LIST[10];

        QString GENDER;
        if(NEW_GENDER == "1") {
            GENDER = "남";
        }else {
            GENDER = "여";
        }
        query.prepare("INSERT INTO user (User_ID, User_PW, User_Name, User_HP, User_Email, User_Gender, User_Type, User_Active, User_jumin, User_Doctor_Num, User_Hospital_Num) VALUES (:ID, :PW, :NAME, :PHONE, :EMAIL, :GENDER, :TYPE, 0, :JUMIN, :User_Doctor_Num, :HOSPITAL_NUM)");
        query.bindValue(":ID", NEW_ID);
        query.bindValue(":PW", NEW_PW);
        query.bindValue(":NAME", NEW_NAME);
        query.bindValue(":PHONE", NEW_PHONE);
        query.bindValue(":EMAIL", NEW_EMAIL);
        query.bindValue(":GENDER", GENDER);
        query.bindValue(":TYPE", Type);
        query.bindValue(":JUMIN", NEW_JUMIN);
        query.bindValue(":User_Doctor_Num", NEW_DOCTOR_NUM);
        query.bindValue(":HOSPITAL_NUM", NEW_HOSPITAL_NUM);
        query.exec();

        senderSKT->write("JOINO");
    }
    else if (Type == "PATIENT")
    {
        QString NEW_ID = DATA_LIST[2];
        QString NEW_PW = DATA_LIST[3];
        QString NEW_NAME = DATA_LIST[4];
        QString NEW_JUMIN = DATA_LIST[5];
        QString NEW_EMAIL = DATA_LIST[6];
        QString NEW_PHONE = DATA_LIST[7];
        QString NEW_GENDER = DATA_LIST[8];
        QString NEW_DOCTOR_NUM = DATA_LIST[9];
        //QString NEW_HOSPITAL_NUM = DATA_LIST[10];
        QString GENDER;
        if(NEW_GENDER == "1") {
            GENDER = "남";
        }else {
            GENDER = "여";
        }
        query.prepare("INSERT INTO user (User_ID, User_PW, User_Name, User_HP, User_Email, User_Gender, User_Type, User_Active, User_jumin, User_Doctor_Num, User_Hospital_Num) VALUES (:ID, :PW, :NAME, :PHONE, :EMAIL, :GENDER, :TYPE, 0, :JUMIN, :USER_DOC, 0)");
        query.bindValue(":ID", NEW_ID);
        query.bindValue(":PW", NEW_PW);
        query.bindValue(":NAME", NEW_NAME);
        query.bindValue(":PHONE", NEW_PHONE);
        query.bindValue(":EMAIL", NEW_EMAIL);
        query.bindValue(":GENDER", GENDER);
        query.bindValue(":TYPE", Type);
        query.bindValue(":JUMIN", NEW_JUMIN);
        query.bindValue(":USER_DOC", NEW_DOCTOR_NUM);
        query.exec();

        senderSKT->write("JOINO");
    }
}

// 중복확인(경석 추가)
void Database::ID_CHECK(QTcpSocket * senderSKT, QStringList DATA_LIST)
{
    QString Type = DATA_LIST[2];
    QString ID = DATA_LIST[3];
    QSqlQuery query;
    query.prepare("SELECT * FROM user WHERE User_ID = :USER_ID");
    query.bindValue(":USER_ID", ID);
    query.exec();
    if(query.next())
    {
        // 회원가입 불가
        QString SendMSG = QString("IDDUPLICATE|%1|IDCHECKX").arg(Type);
        senderSKT->write(SendMSG.toUtf8());
    }
    else
    {
        // 회원가입 가능
        QString SendMSG = QString("IDDUPLICATE|%1|IDCHECKO").arg(Type);
        senderSKT->write(SendMSG.toUtf8());
    }
}

// 환자 회원가입 들어왔을 경우 병원 리스트
void Database::Hospital_List(QTcpSocket *senderSKT)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM hospital");
    query.exec();
    QString msg = "HOSPITAL";
    while(query.next())
    {
        QString h_num = query.value("Hospital_Num").toString();
        QString h_name = query.value("Hospital_Name").toString();
        msg += "|" + h_num + "|" + h_name;
    }
    qDebug() << "병원 리스트 : " << msg;
    senderSKT -> write(msg.toUtf8());
    senderSKT -> flush();
}

// 환자 회원가입 들어왔을 경우 환자 리스트
void Database::Doctor_List(QTcpSocket *senderSKT, QStringList DATA_LIST)
{
    QString h_num = DATA_LIST[2];
    QSqlQuery query;
    QString msg = "DOCTOR_LIST";
    query.prepare("SELECT * FROM user WHERE User_Hospital_Num = :h_num AND User_Type = 'DOCTOR'");
    query.bindValue(":h_num", h_num);
    query.exec();

    while(query.next())
    {
        QString user_num = query.value("User_Num").toString();
        QString user_name = query.value("User_Name").toString();
        msg += "|" + user_num + "|" + user_name;
    }
    qDebug() << "의사 리스트 : " << msg;
    senderSKT -> write(msg.toUtf8());
    senderSKT -> flush();
}

// 아이디 비번 찾기
void Database::Find_ID_PW(QTcpSocket * senderSKT, QStringList DATA_LIST)
{
    QString Type = DATA_LIST[1];
    QSqlQuery query;

    if (Type == "ID")
    {
        QString Name = DATA_LIST[2];
        QString JuImn_ID = DATA_LIST[2];
        query.prepare("SELECT * FROM user WHERE User_Name = :USER_NAME AND User_jumin = :USER_JUMIN");
        query.bindValue(":USER_NAME", Name);
        query.bindValue(":USER_JUMIN", JuImn_ID);
        query.exec();
        if(query.next())
        {
            QString ID = query.value("User_ID").toString();
            QString SendMSG = QString("IDO|%1").arg(ID);
            qDebug() << SendMSG;
            senderSKT->write(SendMSG.toUtf8());
            senderSKT->flush();
        }
        else
        {
            QString SendMSG = "IDX";
            qDebug() << SendMSG;
            senderSKT->write(SendMSG.toUtf8());
            senderSKT->flush();
        }
    }
    else if (Type == "PW")
    {
        QString ID = DATA_LIST[2];
        QString Name = DATA_LIST[3];
        QString JuImn_ID = DATA_LIST[4];
        query.prepare("SELECT * FROM user WHERE User_ID = :USER_ID AND User_Name = :USER_NAME AND User_jumin = USER_JUMIN");
        query.bindValue(":USER_ID", ID);
        query.bindValue(":USER_NAME", Name);
        query.bindValue(":USER_JUMIN", JuImn_ID);
        query.exec();
        if(query.next())
        {
            QString PW = query.value("User_PW").toString();
            QString SendMSG = QString("PWO|%1").arg(PW);
            qDebug() << SendMSG;
            senderSKT->write(SendMSG.toUtf8());
            senderSKT->flush();
        }
        else
        {
            QString SendMSG = "PWX";
            qDebug() << SendMSG;
            senderSKT->write(SendMSG.toUtf8());
            senderSKT->flush();
        }
    }
}

// 환자 상담 등록했는지 확인
void Database::Patient_Question_Check(QTcpSocket *senderSKT, QStringList DATA_LIST)
{
    qDebug() << "환자 상담 등록 여부 : " << DATA_LIST;
    QString user_num = DATA_LIST[2];
    QDateTime currentDate = QDateTime::currentDateTime();
    QString stringDate = currentDate.toString("yyyy-MM-dd");
    QSqlQuery query;
    QString msg = "Q&A|CHECK";
    query.prepare("SELECT * FROM patient WHERE User_Num = :user_num AND Patient_Insert_Date = :stringDate");
    query.bindValue(":user_num", user_num);
    query.bindValue(":stringDate", stringDate);
    query.exec();
    if(query.next())
    {
        qDebug() << "데이터 있음";
        QString question1 = query.value("Question_1").toString();
        QString question2 = query.value("Question_2").toString();
        QString question3 = query.value("Question_3").toString();
        msg += "|" + question1 + "|" + question2 + "|" + question3;
    }
    else
    {
        qDebug() << "데이터 없음";
        msg += "|X";
    }
    qDebug() << msg;
    senderSKT -> write(msg.toUtf8());
    senderSKT -> flush();

}

// 환자 상담 등록하기
QString Database::Patient_Question_ADD(QTcpSocket *senderSKT, QStringList DATA_LIST)
{
    QString user_num = DATA_LIST[2];
    QString date = DATA_LIST[3];
    QString question1 = DATA_LIST[4];
    QString question2 = DATA_LIST[5];
    QString question3 = DATA_LIST[6];
    QSqlQuery query;
    query.prepare("SELECT User_Doctor_Num FROM user WHERE User_Num = :user_num");
    query.bindValue(":user_num", user_num);
    query.exec();
    query.next();
    QString doctor_num = query.value("User_Doctor_Num").toString();
    query.prepare("INSERT INTO patient VALUES(NULL, :user_num, :doctor_num, :question1, :question2, :question3, :date)");
    query.bindValue(":user_num", user_num);
    query.bindValue(":doctor_num", doctor_num);
    query.bindValue(":question1", question1);
    query.bindValue(":question2", question2);
    query.bindValue(":question3", question3);
    query.bindValue(":date", date);
    bool success = query.exec();
    if(success)
    {
        qDebug() << "성공";
        QString msg = "Q&A|SUCCESS";
        senderSKT -> write(msg.toUtf8());
        senderSKT -> flush();
        QString Return_ = user_num + "|" + date;
        return Return_;
    }
    else
    {
        qDebug() << "실패";
        QString msg = "Q&A|FAILED";
        senderSKT -> write(msg.toUtf8());
        senderSKT -> flush();
    }
}

// 등록된 일정 보여주기
void Database::Schedule_Search(QTcpSocket *senderSKT, QStringList DATA_LIST)
{
    QString date = DATA_LIST[1];
    QString user_num = DATA_LIST[2];
    QSqlQuery query;
    int data_num = 0;
    query.prepare("SELECT * FROM schedule WHERE User_num = :user_num AND Schedule_Date = :date");
    query.bindValue(":user_num", user_num);
    query.bindValue(":date", date);
    query.exec();
    QString MSG = "SCHEDULE";
    while(query.next())
    {
        data_num += 1;
        QString title = query.value("Schedule_Title").toString();
        QString space = query.value("Schedule_Space").toString();
        QString memo = query.value("Schedule_Memo").toString();
        MSG += "|" + title + "|" + space + "|" + memo;
    }

    if(data_num == 0) {
        MSG += "X";
        senderSKT -> write(MSG.toUtf8());
        senderSKT -> flush();
    }
    else if(data_num > 0)
    {
        qDebug() << data_num;
        senderSKT -> write(MSG.toUtf8());
        senderSKT -> flush();
    }

}

// 환자 일정 등록하기
void Database::Schedule_Insert(QTcpSocket *senderSKT, QStringList DATA_LIST)
{
    qDebug() << DATA_LIST;
    QString user_num = DATA_LIST[1];
    QString date = DATA_LIST[2];
    QString title = DATA_LIST[3];
    QString space = DATA_LIST[4];
    QString memo = DATA_LIST[5];
    QSqlQuery query;
    query.prepare("INSERT INTO schedule VALUES(NULL, :user_num, :date, :memo, :title, :space)");
    query.bindValue(":user_num", user_num);
    query.bindValue(":date", date);
    query.bindValue(":memo", memo);
    query.bindValue(":title", title);
    query.bindValue(":space", space);
    bool success = query.exec();
    if(success)
    {
        QString msg = "SCHEDULE_INSERT|SUCCESS";
        senderSKT -> write(msg.toUtf8());
        senderSKT -> flush();
    }
    else
    {
        QString msg = "SCHEDULE_INSERT|FAILED";
        senderSKT -> write(msg.toUtf8());
        senderSKT -> flush();
    }
}

// 환자 일정 상세보기
void Database::Schedule_Detail(QTcpSocket *senderSKT, QStringList DATA_LIST)
{
    QString title = DATA_LIST[1];
    QString user_num = DATA_LIST[2];
    int i = DATA_LIST[3].toInt() - 1;
    QSqlQuery query;
    QString msg = "SCHEDULE_DETAIL";
    query.prepare("SELECT * FROM schedule WHERE User_Num = :user_num AND Schedule_Title = :Title");
    query.bindValue(":user_num", user_num);
    query.bindValue(":Title", title);
    query.exec();
    while(query.next())
    {
        QString date = query.value("Schedule_Date").toString();
        QString memo = query.value("Schedule_Memo").toString();
        QString title = query.value("Schedule_Title").toString();
        QString space = query.value("Schedule_Space").toString();
        msg += "|" + title + "|" + date + "|" + space + "|" + memo;
    }
    senderSKT -> write(msg.toUtf8());
    senderSKT -> flush();
}

// 환자 일정 업데이트
void Database::Schedule_Update(QTcpSocket *senderSKT, QStringList DATA_LIST)
{
    QString date = DATA_LIST[1];
    QString origin_title = DATA_LIST[2];
    QString new_title = DATA_LIST[3];
    QString new_space = DATA_LIST[4];
    QString new_memo = DATA_LIST[5];

    QSqlQuery query;
    query.prepare("UPDATE schedule SET Schedule_Title = :title, Schedule_Space = :space, Schedule_Memo = :memo WHERE Schedule_Title = :origin_title AND Schedule_Date = :date");
    query.bindValue(":date", date);
    query.bindValue(":origin_title", origin_title);
    query.bindValue(":title", new_title);
    query.bindValue(":space", new_space);
    query.bindValue(":memo", new_memo);
    bool success = query.exec();
    if(success)
    {
        qDebug() << "성공";
        QString msg = "SCHEDULE_UPDATE|SUCCESS";
        senderSKT -> write(msg.toUtf8());
        senderSKT -> flush();
    }
    else
    {
        qDebug() << "실패";
        QString msg = "SCHEDULE_UPDATE|FAILED";
        senderSKT -> write(msg.toUtf8());
        senderSKT -> flush();
    }
}

// 환자 일정 삭제하기
void Database::Schedule_Delete(QTcpSocket *senderSKT, QStringList DATA_LIST)
{
    qDebug() << "일정 삭제 : " << DATA_LIST;
    QString user_num = DATA_LIST[1];
    QString date = DATA_LIST[2];
    QString title = DATA_LIST[3];

    QSqlQuery query;
    query.prepare("DELETE FROM schedule WHERE User_Num = :user_num AND Schedule_Date = :date AND Schedule_Title = :title");
    query.bindValue(":user_num", user_num);
    query.bindValue(":date", date);
    query.bindValue(":title", title);
    bool success = query.exec();
    if(success)
    {
        qDebug() << "삭제 성공";
        QString msg = "SCHEDULE_DELETE|SUCCESS";
        senderSKT -> write(msg.toUtf8());
        senderSKT -> flush();
    }
    else
    {
        qDebug() << "삭제 실패";
        QString msg = "SCHEDULE_DELETE|FAILED";
        senderSKT -> write(msg.toUtf8());
        senderSKT -> flush();
    }
}

// 환자 리스트
void Database::Patient_List(QTcpSocket *senderSKT, QStringList DATA_LIST)
{
    QString user_num = DATA_LIST[1];
    QSqlQuery query;
    query.prepare("SELECT * FROM user WHERE User_Type = 'PATIENT' AND User_Doctor_Num = :user_num");
    query.bindValue(":user_num", user_num);
    query.exec();
    QString msg = "PATIENT_LIST";
    while(query.next())
    {
        QString num = query.value("User_Num").toString();
        QString name = query.value("User_Name").toString();
        QString hp = query.value("User_HP").toString();
        QString gender = query.value("User_Gender").toString();
        QString age = query.value("User_jumin").toString();
        age = age.mid(0, 6);
        msg += "|" + num + "|" + name + "|" + hp + "|" + gender + "|" + age;
    }
    qDebug() << "환자 리스트 : " << msg;
    senderSKT -> write(msg.toUtf8());
    senderSKT -> flush();
}

// 환자 분석 상세보기
void Database::Patient_Detail(QTcpSocket *senderSKT, QStringList DATA_LIST)
{
    QString patient_num = DATA_LIST[1];
    QDateTime currentDate = QDateTime::currentDateTime();
    QString stringDate = currentDate.toString("yyyy-MM-dd");

    QSqlQuery query;
    query.prepare("SELECT * FROM ai WHERE Patient_Num = :patient_num AND Date = :stringDate");
    query.bindValue(":patient_num", patient_num);
    query.bindValue(":stringDate", stringDate);
    query.exec();
    QString msg = "PATIENT_DETAIL|";
    while(query.next())
    {
        QString valence = query.value("Valence").toString();
        QString arousal = query.value("Arousal").toString();
        QString dominance = query.value("Dominance").toString();
        msg += valence + "|" + arousal + "|" + dominance + "|";
    }
    // 파일명

    QString file_name = QString("%1_%2_Emotion_graph.png").arg(patient_num).arg(stringDate);

    // 파일 경로
    QString savePath = "C:/Users/LMS23/Desktop/server_file/";
    QString file_path = savePath + file_name;
    qDebug() << "파일 경로 : " << file_path;
    // 파일이 존재하는지 확인
    QFile file(file_path);
    if(file.exists())
    {
        if (file.open(QIODevice::ReadOnly))                // 읽기 전용
        {
            QByteArray DATA = file.readAll();

            int chunkSize = 1024;
            int totalDataSize = DATA.size();
            int bytesSent = 0;

            QString DATA_SIZE_STR = QString::number(totalDataSize);

            qDebug() << DATA_SIZE_STR;
            msg += DATA_SIZE_STR.toUtf8() + "|";
            qDebug() << "msg 는 뭐게~" << msg;
            QByteArray Header = msg.toUtf8();
            Header.resize(128);
            DATA.prepend(Header);

            while (bytesSent < totalDataSize)
            {
                QByteArray chunk = DATA.mid(bytesSent, chunkSize);
                int bytesWritten = senderSKT->write(chunk);

                if (bytesWritten == -1)
                {
                    qDebug() << "Error : 데이터 전송 실패";
                    break;
                }
                bytesSent += bytesWritten;
            }
            senderSKT->flush(); // 모든 데이터를 송신하기 위해 flush 호출
            file.close(); // 파일 닫기
        }
        else
        {
            qDebug() << "Error : 파일 전송 실패";
        }
    }
    else
    {
        qDebug() << "전송할 파일이 없습니다.";
    }
}

// 환자 분석 결과 DB에 저장
void Database::Analyze_Result_Save(QString patient_num, QString DATA_LIST)
{
    for(int i = 0; i <= 8; i++)
    {
        if(DATA_LIST.split("|")[i].toFloat() > 0)
        {
            // 문자열을 실수로 변환
            float value = float(DATA_LIST.split("|")[i].toFloat());
            // 소수점 둘째 자리에서 반올림
            float rounded_value = roundf(value * 100) / 100;
            DATA_LIST.split("|")[i] = QString::number(rounded_value);
        }
    }
    qDebug() << "DATA_LIST : " << DATA_LIST;

    QString x1 = DATA_LIST.split("|")[0];
    QString y1 = DATA_LIST.split("|")[1];
    QString z1 = DATA_LIST.split("|")[2];
    QString x2 = DATA_LIST.split("|")[3];
    QString y2 = DATA_LIST.split("|")[4];
    QString z2 = DATA_LIST.split("|")[5];
    QString x3 = DATA_LIST.split("|")[6];
    QString y3 = DATA_LIST.split("|")[7];
    QString z3 = DATA_LIST.split("|")[8];

    QDateTime currentDate = QDateTime::currentDateTime();
    QString stringDate = currentDate.toString("yyyy-MM-dd");

    QSqlQuery query;
    // 1번 질문 결과값 저장
    query.prepare("INSERT INTO ai VALUES(:patient_num, :x1, :y1, :z1, :stringDate)");
    query.bindValue(":patient_num", patient_num);
    query.bindValue(":x1", x1);
    query.bindValue(":y1", y1);
    query.bindValue(":z1", z1);
    query.bindValue(":stringDate", stringDate);
    query.exec();

    // 2번 질문 결과값 저장
    query.prepare("INSERT INTO ai VALUES(:patient_num, :x2, :y2, :z2, :stringDate)");
    query.bindValue(":patient_num", patient_num);
    query.bindValue(":x2", x2);
    query.bindValue(":y2", y2);
    query.bindValue(":z2", z2);
    query.bindValue(":stringDate", stringDate);
    query.exec();

    // 3번 질문 결과값 저장
    query.prepare("INSERT INTO ai VALUES(:patient_num, :x3, :y3, :z3, :stringDate)");
    query.bindValue(":patient_num", patient_num);
    query.bindValue(":x3", x3);
    query.bindValue(":y3", y3);
    query.bindValue(":z3", z3);
    query.bindValue(":stringDate", stringDate);
    query.exec();
}

// 담당 환자 답변 보기
void Database::Patient_Question(QTcpSocket *senderSKT, QStringList DATA_LIST)
{
    QString doctor_num = DATA_LIST[1];
    QString user_num = DATA_LIST[2];
    qDebug() << "의사 번호 : " << doctor_num;
    qDebug() << "회원 번호 : " << user_num;
    QDateTime currentDate = QDateTime::currentDateTime();
    QString stringDate = currentDate.toString("yyyy-MM-dd");
    qDebug() << "오늘 날짜 : " << stringDate;

    QSqlQuery query;
    QString msg = "HISTORY";
    query.prepare("SELECT * FROM patient WHERE User_Num = :user_num AND Doctor_Num = :doctor_num AND Patient_Insert_Date = :date");
    query.bindValue(":user_num", user_num);
    query.bindValue(":doctor_num", doctor_num);
    query.bindValue(":date", stringDate);
    query.exec();

    if(query.next())
    {
        QString q1 = query.value("Question_1").toString();
        QString q2 = query.value("Question_2").toString();
        QString q3 = query.value("Question_3").toString();
        msg += "|" + q1 + "|" + q2 + "|" + q3;
    }
    else
    {
        msg += "|X";
    }
    qDebug() << "보낼 메세지 : " << msg;
    senderSKT -> write(msg.toUtf8());
    senderSKT -> flush();
}

// 셋팅창 내정보 보내기
void Database::Setting(QTcpSocket *senderSKT, QStringList DATA_LIST)
{
    QString user_num = DATA_LIST[1];
    QSqlQuery query;
    query.prepare("SELECT * FROM user WHERE User_Num = :user_num");
    query.bindValue(":user_num", user_num);
    query.exec();

    if(query.next())
    {
        QString id = query.value("User_ID").toString();
        QString pw = query.value("User_PW").toString();
        QString name = query.value("User_Name").toString();
        QString hp = query.value("User_HP").toString();
        QString email = query.value("User_Email").toString();
        QString jumin = query.value("User_jumin").toString();
        jumin = jumin.mid(0, 6);
        QString msg = "MYINFO|" + id + "|" + pw + "|" + name + "|" + hp + "|" + email + "|" + jumin;

        senderSKT -> write(msg.toUtf8());
        senderSKT -> flush();
    }
}

// 로그아웃
void Database::User_Logout(QStringList DATA_LIST)
{
    QString user_num = DATA_LIST[1];
    QSqlQuery query;
    query.prepare("UPDATE user SET User_Active = 0 WHERE User_Num = :user_num");
    query.bindValue(":user_num", user_num);
    query.exec();
}

// 회원탈퇴
void Database::Secession(QTcpSocket *senderSKT, QStringList DATA_LIST)
{
    QString user_num = DATA_LIST[1];
    QString pw = DATA_LIST[2];
    QSqlQuery query;
    QString msg = "SECESSTION";

    query.prepare("SELECT * FROM user WHERE User_Num = :user_num AND User_PW = :pw");
    query.bindValue(":user_num", user_num);
    query.bindValue(":pw", pw);
    query.exec();
    if(query.next())
    {
        // 유저 정보 삭제
        QString User_Type = query.value("User_Type").toString();
        query.prepare("DELETE FROM user WHERE User_Num = :user_num");
        query.bindValue(":user_num", user_num);
        query.exec();

        // 스케줄 테이블 유저 정보 삭제
        query.prepare("DELETE FROM schedule WHERE User_Num = :user_num");
        query.bindValue(":user_num", user_num);
        query.exec();
        if(User_Type == "DOCTOR")
        {
            //query.prepare("");
        }
        else if(User_Type == "PATIENT")
        {
            // 환자가 작성한 일지 삭제
            query.prepare("DELETE FROM patient WHERE User_Num = :user_num");
            query.bindValue(":user_num", user_num);
            query.exec();
        }
        msg += "|FINISH";
    }
    else
    {
        msg += "|PWERROR";
    }
    senderSKT -> write(msg.toUtf8());
    senderSKT -> flush();
}

// 파이썬으로 데이터 전송
void Database::Send_PYTHON(QTcpSocket * SKT, QString TEXT)
{
    QStringList test = TEXT.split("|");
    QString Patient_Num = test[0];
    QString date = test[1];

    qDebug() << "Send_PYTHON : " << Patient_Num << "/" << date;
    QSqlQuery query;
    query.prepare("SELECT * FROM patient WHERE User_Num = :USER_NUM AND Patient_Insert_Date = :DATE");
    query.bindValue(":USER_NUM", Patient_Num);
    query.bindValue(":DATE", date);
    query.exec();
    if(query.next())
    {
        QString Question_1 = query.value("Question_1").toString();
        QString Question_2 = query.value("Question_2").toString();
        QString Question_3 = query.value("Question_3").toString();

        QString SendMSG = QString("%1|%2|%3|%4|%5").arg(Patient_Num).arg(date).arg(Question_1).arg(Question_2).arg(Question_3);
        qDebug() << SendMSG;
        qDebug() << SKT;
        SKT->write(SendMSG.toUtf8());
    }
    else
    {
        qDebug() << "실패 ㅠㅠ : ";
    }
}

// 클라이언트와 연결 종료시
void Database::DB_Disconnected(QTcpSocket * leftSKT)
{
    QString ID = Qmap_User_Socket_ID.value(leftSKT);
    QString Type = Qmap_User_ID_Type.value(ID);
    qDebug() << "ID : " << ID;
    qDebug() << "Type : " << Type;
    QSqlQuery query;
    query.prepare("UPDATE user SET User_Active = :USER_ACTIVE WHERE User_ID = :ID AND User_Type = :USER_TYPE");
    query.bindValue(":USER_ACTIVE", "0");
    query.bindValue(":ID", ID);
    query.bindValue(":USER_TYPE", Type);
    query.exec();
    Qmap_User_Socket_ID.remove(leftSKT);
    Qmap_User_ID_Type.remove(ID);
}
