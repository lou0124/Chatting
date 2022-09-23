## 프로그램 설명

리눅스 epoll 기법을 활용한 다중 클라이언트 접속 채팅 프로그램 구현

## 실행 방법
1. 리눅스 os 사용
2. gcc -o 서버실행파일이름 chatting_server.c fd_linked_list.c -lpthread (서버 컴파일)
3. ./서버실행파일이름 포트번호 (서버 실행)
4. gcc -o 클라이언트실행파일이름 chatting_client.c (클라이언트 컴파일)
5. ./클라이언트실행파일이름 127.0.0.1 포트번호 (클라이언트 실행)


## 실행 모습
![](https://i.imgur.com/Mlozloj.png)

