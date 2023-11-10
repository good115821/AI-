#include "server.h"

Server::Server(QObject *parent): QTcpServer(parent)
{
    initialize(); // 파이썬 서버랑 연결
    Mysql_DB_Connection(); // Mysql_DB 연결

    // 감시자한태 폴더 경로 지정
    watcher.addPath(auto_Path);

    // 감시자가 폴더내용 변경을 감지했을때 slot_send_file 함수로 이동시켜주기
    //connect(&watcher, &QFileSystemWatcher::directoryChanged, this, &Server::slot_send_file);
}

Server::~Server()
{
    foreach(QTcpSocket *client, qset_clntSKTList)
    {
        client->disconnectFromHost();
    }
}

void Server::initialize()
{
    tcpSocket = new QTcpSocket(this);
    QHostAddress serverAddress(py_ServerIP);
    tcpSocket->connectToHost(serverAddress, py_ServerPort.toUShort());

    connect(tcpSocket, &QTcpSocket::connected, this, &Server::Py_Server_Connected);

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(slot_readMSG()));     // 데이터 읽기
    //connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnect()));   //클라이언트에서 서버 연결 해제
}

void Server::Mysql_DB_Connection()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(hostName);       // MySQL 호스트 이름
    db.setPort(port);               // MySQL 포트 번호
    db.setDatabaseName(dbName);     // 데이터베이스 이름
    db.setUserName(userName);       // MySQL 사용자 이름
    db.setPassword(passWord);       // MySQL 암호

    if (!db.open())
    {
        qDebug() << "데이터베이스 연결 실패:" << db.lastError().text();
    }
    else
    {
        qDebug() << "데이터베이스 연결 성공";
    }
}

void Server::Py_Server_Connected()
{
    // 연결이됬을때
    if (tcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        qDebug() << "Connected to the python server";
        //tcpSocket->write(QString("[ 중앙서버 접속됨 ]").toUtf8());
        //tcpSocket->write(QString("2|오늘은 너무 기쁜 날이었다. 왜냐하면 모든 가족들이 함께 모여 바베큐 파티를 했다. 오랜만에 만나는 사촌들과 얘기도 너무 재미있었다. 가족들과의 행복한 관계가 내가 살아가는 이유인 것 같다.|오늘은 너무 슬펐다. 왜냐하면 몸이 너무 아팠다. 신체적인 몸도 너무 아팠을 뿐 더러, 정신적으로 너무 피폐하고 지쳤다. 숨을 쉬고 있는 동안 너무 힘든 기분 뿐이었다. 앞으로 상담을 하긴 하지만, 어떻게 잘 진행될지는 미지수이다.|오늘은 그저 그런 날이었다. 그냥 아침에 일어나 밥을 먹고, 운동을 갔다. 그리고 계속 집 청소를 했다. 그리고 밥을 먹고 그냥 아무 일 없이 오후시간을 보냈다. 아무런 일이 없는 그냥 무미건조한 하루였다.").toUtf8());
        //tcpSocket->flush();
    }
    else // 연결 실패일때
    {
        qDebug() << "Connection error: " << tcpSocket->errorString();
    }
}

void Server::incomingConnection(qintptr socketfd)
{
    // 새로운 연결 소켓을 만들고
    // 소켓에 파일 디스크립터를 식별한다.
    QTcpSocket *newConnectedSKT = new QTcpSocket(this);
    newConnectedSKT->setSocketDescriptor(socketfd);

    // qset 을 사용해 연결된 소켓 객체들을 관리한다.
    // qset 에 저장된 연결된 소켓(클라이언트)의 수를 전달하는 시그널을 동작시킨다.
    qset_clntSKTList.insert(newConnectedSKT);
    //emit signal_updateClntCNT(qset_clntSKTList.count());
    newConnectedSKT->write("[ 중앙서버와 연결되었습니다 ]");

    // 새로운 연결 소켓이 만들어질때
    QString str = QString("새로운 접속자: %1").arg(newConnectedSKT->peerAddress().toString());
    // 시그널을 동작시키고 정보 문자열을 매기변수로 넘긴다.
    emit signal_showNewMSG(str);

    // 연결 소켓에서 읽을 메시지가 들어와 readyRead() 시그널이 동작되면
    connect(newConnectedSKT, SIGNAL(readyRead()), this, SLOT(slot_readMSG()));

    // 연결 소켓에서 disconnected 시그널이 동작되면
    connect(newConnectedSKT, SIGNAL(disconnected()), this, SLOT(slot_disconnected()));
}

// AI 서버 연결 종료
void Server::PYTHON_Disconnected()
{
    tcpSocket->disconnectFromHost();
    emit signal_showNewMSG("AI 서버 강제 연결 종료됨");
}

void Server::slot_readMSG()
{
    QTcpSocket* senderSKT = reinterpret_cast<QTcpSocket*>(sender());           // 신호 보낸 소켓 찾기
    QByteArray readData = senderSKT->readAll();                                // 신호보낸 소켓에 있는 데이터 모두 수신
    QString String_DATA = QString::fromUtf8(readData);                         // UTF-8 인코딩으로 STRING 변수에 담음
    QStringList DATA_LIST = String_DATA.split("|");                            // " | " 문자를 기준으로 스플릿(나눔)
    qDebug() << "받은 메세지 : " << DATA_LIST;

    if(DATA_LIST[0] == "LOGIN")                                                // 로그인 관련 처리
    {
        Login(senderSKT, DATA_LIST);
    }
    else if(DATA_LIST[0] == "JOIN")                                            // 회원가입 관련 처리 JOINADUPLICATE
    {
        if(DATA_LIST[1] == "IDDUPLICATE")                                      // 아이디 중복 확인
        {
            ID_CHECK(senderSKT, DATA_LIST);
        }
        else if(DATA_LIST[1] == "SELECT_HOSPITAL")
        {
            Hospital_List(senderSKT);                                            // 병원 리스트 보내기
        }
        else if(DATA_LIST[1] == "SELECT_DOCTOR")
        {
            Doctor_List(senderSKT, DATA_LIST);                                  // 의사 리스트 보내기
        }
        else                                                                   // 회원가입 처리
        {
            Join(senderSKT, DATA_LIST);
        }
    }
    else if(DATA_LIST[0] == "FIND")                                            // 아이디 / 비번 찾기
    {
        Find_ID_PW(senderSKT, DATA_LIST);
    }
    else if(DATA_LIST[0] == "PYTHON")                                          // AI_서버 관련 처리
    {
        if(DATA_LIST[1] == "PNG_FILE")
        {
            QString File_Name = DATA_LIST[2];
            // 바이트 상태로 데이터 추출하기.
            QByteArray buffer = readData.mid(DATA_LIST[0].length() + DATA_LIST[1].length() + DATA_LIST[2].length()
                                             + DATA_LIST[3].length() + DATA_LIST[4].length() + DATA_LIST[5].length()
                                             + DATA_LIST[6].length() + DATA_LIST[7].length() + DATA_LIST[8].length()
                                             + DATA_LIST[9].length() + DATA_LIST[10].length() + DATA_LIST[11].length() + 12);

            // 파일 경로는 = 서버가 지정한 저장경로 + 파일 이름
            QString filePath = savePath + File_Name;
            QFile file(filePath);
            if(file.open(QIODevice::WriteOnly))
            {
                file.write(buffer);
                QString message = QString("INFO :: Attachment from sd:%1 successfully stored on disk under the path %2").arg(tcpSocket->socketDescriptor()).arg(QString(savePath));
                emit signal_showNewMSG(message);
                file.close();
                Read_file(filePath);
            }

            QStringList split = File_Name.split("_");
            QString patient_num = split[0];
            QString result = DATA_LIST[3] + "|" + DATA_LIST[4] + "|" + DATA_LIST[5] + "|" + DATA_LIST[6] + "|" + DATA_LIST[7] + "|" + DATA_LIST[8] + "|" + DATA_LIST[9]
                 + "|" + DATA_LIST[10] + "|" + DATA_LIST[11];
            Analyze_Result_Save(patient_num, result);

        }
    }
    else if(DATA_LIST[0] == "SCHEDULE")
    {
        Schedule_Search(senderSKT, DATA_LIST);                                  // 등록된 일정 찾기
    }
    else if(DATA_LIST[0] == "Q&A")
    {
        if(DATA_LIST[1] == "CHECK")
        {
            Patient_Question_Check(senderSKT, DATA_LIST);                       // 환자 Q&A 완료했는지 체크
        }
        else if(DATA_LIST[1] == "INSERT")
        {
            QString Return_ = Patient_Question_ADD(senderSKT, DATA_LIST);                         // 환자 상담 등록하기
            qDebug() << "Server : " << Return_;
            Send_PYTHON(tcpSocket , Return_);
        }
    }
    else if(DATA_LIST[0] == "SCHEDULE_DETAIL")
    {
        Schedule_Detail(senderSKT, DATA_LIST);                                  // 일정 상세보기
    }
    else if(DATA_LIST[0] == "SCHEDULE_ADD")
    {
        Schedule_Insert(senderSKT, DATA_LIST);                                  // 일정 등록하기
    }
    else if(DATA_LIST[0] == "SCHEDULE_UPDATE")
    {
        Schedule_Update(senderSKT, DATA_LIST);                                  // 일정 업데이트
    }
    else if(DATA_LIST[0] == "SCHEDULE_DELETE")
    {
        Schedule_Delete(senderSKT, DATA_LIST);                                  // 일정 삭제
    }
    else if(DATA_LIST[0] == "PATIENT_LIST")
    {
        Patient_List(senderSKT, DATA_LIST);                                     // 환자 리스트
    }
    else if(DATA_LIST[0] == "PATIENT_DETAIL")
    {
        Patient_Detail(senderSKT, DATA_LIST);                                   // 환자 분석 상세보기
    }
    else if(DATA_LIST[0] == "PATIENT_QUESTION")
    {
        Patient_Question(senderSKT, DATA_LIST);                                 // 의사가 선택한 환자의 질문지 전송
    }
    else if(DATA_LIST[0] == "LOGOUT")
    {
        User_Logout(DATA_LIST);                                                 // 로그아웃
    }
    else if(DATA_LIST[0] == "GIVE_MYINFO")
    {
        Setting(senderSKT, DATA_LIST);                                          // 설정창 정보
    }
    else if(DATA_LIST[0] == "SECESSTION")
    {
        Secession(senderSKT, DATA_LIST);                                        // 회원 탈퇴
    }
    else if(DATA_LIST[0] == "FILE_COMPLETE")
    {
        qDebug() << "Send_File_complete";
        senderSKT->write(QString("Start_next_task").toUtf8());
        senderSKT->flush();
    }
}

// 메모장 내용 읽기
void Server::Read_file(QString filepath)
{
    qDebug() << "Filepath: " << filepath;
    QFile file(filepath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Could not open the file for reading: " << file.errorString();
        return;
    }
    QTextStream in(&file);
    QString myText = in.readAll();
    qDebug() << "File Content: " << myText;
    file.close();
}

void Server::slot_disconnected()
{
    // 접속 종료 시그널을 발생시킨 소켓을 찾기
    QTcpSocket *leftSKT = (QTcpSocket*)sender();

    // 접속 끊긴 소켓을 qset에서 제거
    qset_clntSKTList.remove(leftSKT);

    // 접속 끊긴 소켓에 있는 아이디의 Active = 0으로 변경
    DB_Disconnected(leftSKT);

    // 서버 메시지창에 보여주고
    QString str = QString("*연결 종료 : IP : %1").arg(leftSKT->peerAddress().toString());
    emit signal_showNewMSG(str);
}
