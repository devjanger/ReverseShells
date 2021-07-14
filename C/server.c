#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define MAX_BUFF 91024
#define PORT 8081

void *t_send(void *arg_sock)
{
    int sock = *((int*)arg_sock);

    char message[MAX_BUFF];

    
    while(1)
    {
        usleep(455555);
        
        fgets(message, sizeof(message) , stdin);
        
        write(sock, message, strlen(message));

        strcpy(message, "");
    }
    
}

void *t_recv(void *arg_sock)
{
    int sock = *((int*)arg_sock);

    char message[MAX_BUFF];
    int str_len;


    while(1)
    {
        bzero(message, MAX_BUFF);

        
        str_len = recv(sock, message, sizeof(message), 0);

        if(str_len == -1)
            exit(1);
            //return (void*)-1;

        printf("%s", message);
    }
}

int main(void)
{

    pthread_t p_thread[2];
    int thr_id;
    int status;


    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;



    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        fputs("socket() error!", stderr);


    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);


    bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
    
    if(listen(serv_sock, 5) == -1)
        fputs("listen() error!", stderr);


    clnt_addr_size = sizeof(clnt_addr);
    
    clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
    if(clnt_sock == -1)
        fputs("accept() error!", stderr);


    printf("Connected from IP: %s, PORT: %d\n", inet_ntoa(clnt_addr.sin_addr), clnt_addr.sin_port);



    // t_send
    thr_id = pthread_create(&p_thread[0], NULL, t_send, (void *)&clnt_sock);

    // t_recv
    thr_id = pthread_create(&p_thread[1], NULL, t_recv, (void *)&clnt_sock);


    pthread_join(p_thread[0], (void **)&status);
    pthread_join(p_thread[1], (void **)&status);


    close(clnt_sock);
    close(serv_sock);

    return 0;
}
