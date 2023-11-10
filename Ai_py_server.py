import socket
import matplotlib.pyplot as plt
import googletrans
import pandas as pd
import time
from mpl_toolkits.mplot3d import Axes3D

def Data_Graph_test(X1, Y1, Z1, X2, Y2, Z2, X3, Y3, Z3, Patient_Num, date):
    # 예측된 감정 점수 (3개의 점)
    points = [
        (X1, Y1, Z1, 'red'),  # Point 1 (빨간색)
        (X2, Y2, Z2, 'blue'),  # Point 2 (파란색)
        (X3, Y3, Z3, 'purple')  # Point 3 (보라색)
    ]

    # 3차원 그래프 생성
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    # 각 포인트에 대한 좌표 및 색상 설정
    for point in points:
        valence, arousal, dominance, color = point
        ax.scatter(valence, arousal, dominance, c=color, marker='o')

    # 축 레이블
    ax.set_xlabel('Valence')
    ax.set_ylabel('Arousal')
    ax.set_zlabel('Dominance')

    plt.savefig("C:/Users/fiveStones/PycharmProjects/AI_Graph_Png_File/{}_{}_Emotion_graph.png".format(Patient_Num, date))

def Google_Translation(str1, str2, str3):
    translator = googletrans.Translator()
    print("--------------------------------------------------------")
    print("번역중....")
    result1 = translator.translate(str1, dest='en')
    result2 = translator.translate(str2, dest='en')
    result3 = translator.translate(str3, dest='en')
    print("번역 완료")
    print(f"번역결과 : => \n텍스트 1 : {result1.text}\n텍스트 2 : {result2.text}\n텍스트 3 : {result3.text}")
    print("--------------------------------------------------------")
    return result1.text, result2.text, result3.text

def AI_MODEL_Runing(input_text):
    anew_data = pd.read_csv("C:/Users/fiveStones/PycharmProjects/anew.csv")
    words = input_text.split()  # 텍스트를 단어로 분할
    valence_scores = []
    arousal_scores = []
    dominance_scores = []

    for word in words:
        # 데이터셋에서 해당 단어를 찾아서 Valence, Arousal, Dominance 점수 가져오기
        word_info = anew_data[anew_data["term"] == word]
        if not word_info.empty:
            valence_scores.append(word_info["pleasure"].values[0])
            arousal_scores.append(word_info["arousal"].values[0])
            dominance_scores.append(word_info["dominance"].values[0])
    if valence_scores and arousal_scores and dominance_scores:
        # 단어별로 구한 점수들의 평균을 계산
        avg_valence = sum(valence_scores) / len(valence_scores)
        avg_arousal = sum(arousal_scores) / len(arousal_scores)
        avg_dominance = sum(dominance_scores) / len(dominance_scores)
        return avg_valence, avg_arousal, avg_dominance
    else:
        return None

 def Data_Graph(X1, Y1, X2, Y2, X3, Y3):
     # X축과 Y축 좌표 값
     image_emotion_1 = X1  # X축 좌표
     result_emotion_1 = Y1  # Y축 좌표

     image_emotion_2 = X2  # X축 좌표
     result_emotion_2 = Y2  # Y축 좌표

     image_emotion_3 = X3  # X축 좌표
     result_emotion_3 = Y3  # Y축 좌표

     average_image_emotion = (X1 + X2 + X3) / 3
     average_result_emotion = (Y1 + Y2 + Y3) / 3

     # 그래프 그리기
     plt.scatter(image_emotion_1, result_emotion_1, marker='o', color='purple', label='1 Data Point')
     plt.scatter(image_emotion_2, result_emotion_2, marker='o', color='blue', label='2 Data Point')
     plt.scatter(image_emotion_3, result_emotion_3, marker='o', color='green', label='3 Data Point')
     plt.scatter(average_image_emotion, average_result_emotion, marker='o', color='red', label='Average Data Point')

     # 그래프 제목과 축 레이블 설정
     plt.title("Emotion_GRAPH")
     plt.xlabel("Image_EMOTION")
     plt.ylabel("Result_EMOTION")

     # X축과 Y축 범위 설정
     plt.xlim(-1, 1)
     plt.ylim(-1, 1)

     # 0,0을 기준으로 세로선 그리기
     plt.axvline(0, color='black', linestyle='--')
     plt.axhline(0, color='black', linestyle='--')

     # 그래프에 레전드(범례) 추가
     plt.legend()

     # 사분면을 그리기
     plt.axhline(0, color='black', linestyle='--', linewidth=0.5)
     plt.axvline(0, color='black', linestyle='--', linewidth=0.5)
     plt.fill_between([-1, 0], -1, 1, color='lightgray', alpha=0.5, label='Quadrant I')
     plt.fill_between([0, 1], -1, 1, color='lightgray', alpha=0.5, label='Quadrant II')
     plt.fill_between([-1, 0], -1, 1, color='lightgray', alpha=0.5)
     plt.fill_between([-1, 0], -1, 1, color='lightgray', alpha=0.5, label='Quadrant III')

     # 그래프 저장
     plt.savefig("C:/Users/LMS23/Desktop/save_png/emotion_graph.png")

def start_server():
    host = '10.10.21.108'
    port = 30000

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((host, port))
    server_socket.listen(5)
    print("--------------------------------------------------------")
    print(f"서버가 IP : {host} / PORT : {port} 의 환경으로 실행중임")
    print("--------------------------------------------------------")
    while True:
        client_socket, client_address = server_socket.accept()
        print(f"중앙서버가 연결되었습니다 : {client_address}")

        Received_DATA = client_socket.recv(1024).decode()
        DATA_LIST = Received_DATA.split("|")
        print("데이터 수신 대기")
        time.sleep(2)
        print("데이터 수신 완료")

        if len(DATA_LIST) == 5:
            print("--------------------------------------------------------")
            print(f"받은 메시지: {Received_DATA}")
            Patient_Num = DATA_LIST[0]
            date = DATA_LIST[1]
            Question_1 = DATA_LIST[2]
            Question_2 = DATA_LIST[3]
            Question_3 = DATA_LIST[4]

            # 구글번역기임
            Translation_success = Google_Translation(Question_1, Question_2, Question_3)
            # 번역 결과를 변수 1,2,3에 담음
            text1, text2, text3 = Translation_success
            # 감정 예측
            emotion_scores = AI_MODEL_Runing(text1)
            valence1, arousal1, dominance1 = 0, 0, 0

            if emotion_scores:
                valence1, arousal1, dominance1 = emotion_scores
                print("1번 결과")
                print(f"Valence (감정가) : {valence1}")
                print(f"Arousal (각성가) : {arousal1}")
                print(f"Dominance (구체성): {dominance1}\n")
            else:
                print("1번 결과")
                print("입력한 텍스트에 대한 감정 정보를 찾을 수 없습니다.")

            # 감정 예측 2
            emotion_scores = AI_MODEL_Runing(text2)
            valence2, arousal2, dominance2 = 0, 0, 0

            if emotion_scores:
                valence2, arousal2, dominance2 = emotion_scores
                print("2번 결과")
                print(f"Valence (감정가) : {valence2}")
                print(f"Arousal (각성가) : {arousal2}")
                print(f"Dominance (구체성): {dominance2}\n")
            else:
                print("2번 결과")
                print("입력한 텍스트에 대한 감정 정보를 찾을 수 없습니다.")

            # 감정 예측 3
            emotion_scores = AI_MODEL_Runing(text3)
            valence3, arousal3, dominance3 = 0, 0, 0
            if emotion_scores:
                valence3, arousal3, dominance3 = emotion_scores
                print("3번 결과")
                print(f"Valence (감정가) : {valence3}")
                print(f"Arousal (각성가) : {arousal3}")
                print(f"Dominance (구체성): {dominance3}\n")
            else:
                print("3번 결과")
                print("입력한 텍스트에 대한 감정 정보를 찾을 수 없습니다.")

            # 데이터 그래프 입력 후 PNG로 저장
            Data_Graph_test(X1=valence1, Y1=arousal1, Z1=dominance1, X2=valence2, Y2=arousal2, Z2=dominance2, X3=valence3, Y3=arousal3, Z3=dominance3, Patient_Num=Patient_Num, date=date)

            # 파일 읽기(임)
            file_path = "C:/Users/fiveStones/PycharmProjects/AI_Graph_Png_File/{}_{}_Emotion_graph.png".format(Patient_Num, date)
            with open(file_path, "rb") as file:
                byte_data = file.read()

            # 헤더 생성(임)
            fileName = '{}_{}_Emotion_graph.png'.format(Patient_Num, date)
            Header = "PYTHON|PNG_FILE|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|".format(fileName, valence1, arousal1, dominance1,
                                                                            valence2, arousal2, dominance2,
                                                                            valence3, arousal3, dominance3).encode('utf-8')

            byte_data = Header + byte_data
            client_socket.sendall(byte_data)
            print("파일 전송 완료")
            print("--------------------------------------------------------")
        else:
            print("--------------------------------------------------------")
            print(f"받은 메시지: {Received_DATA}")
            Send_MSG = f"PYTHON|Error : OUT OF INDEX PYTHON"
            client_socket.send(Send_MSG.encode())

            print(f"보낸 메시지: {Send_MSG}")
            print("--------------------------------------------------------")

if __name__ == '__main__':
    start_server()
