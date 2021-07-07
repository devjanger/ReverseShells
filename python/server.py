from socket import *
import threading
import time
import os, sys, time;


    
PORT = 4444
    
def send(sock):

    while True:


        time.sleep(.8)
        cmd = input("")

        sock.send((cmd+"\n").encode('CP949'))
                


def receive(s):
    while True:
        
        try:
            data = s.recv(9999999)
        except:
            print("ERROR")
            sys.exit()
            
        if len(data) > 0:
            time.sleep(0.03)
            print(data.decode('cp949', errors="replace"), end='')

            #continue



serverSock = socket(AF_INET, SOCK_STREAM)
serverSock.bind(('', PORT))
serverSock.listen(1)


print('%d번 포트로 접속 대기중...'%PORT)

connectionSock, addr = serverSock.accept()

print(str(addr), '에서 접속되었습니다.\n\n')




sender = threading.Thread(target=send, args=(connectionSock,))
#sender.daemon = True
sender.start()

receiver = threading.Thread(target=receive, args=(connectionSock,))
#receiver.daemon = True
receiver.start()






