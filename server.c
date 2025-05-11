#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
char buf[BUFSIZ];

void sys_err(char * s){
    perror(s);
    exit(1);
}

void *read_clit(void * cfd){
    while(1){
        int ret = read((int)cfd, buf, sizeof(buf));
        write( STDOUT_FILENO, buf, ret);
    }
    return NULL;
}
int main(){
    int fd = 0, ret = 0, cfd, clit_addr_len = 0;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == 0)
        sys_err("socket error");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1145);              //转移port
    addr.sin_addr.s_addr = htonl(INADDR_ANY); //转移ip

    bind(fd ,(struct sockaddr*)&addr, sizeof(addr));
    
    ret = listen(fd, 128);
    if(ret == -1)
        sys_err("listen error");
    
    cfd = accept(fd ,(struct sockaddr*)&addr, &clit_addr_len);
    if(cfd == -1)
        sys_err("accept");
    
    printf("connect success\n");
    pthread_t tid = 0;
    pthread_create(&tid, NULL, read_clit,(void *)cfd);
    while(1){
        memset(buf,0,sizeof buf);
        ret = read(STDIN_FILENO,buf,sizeof(buf));
        write( cfd, buf, ret);
    }
    close(fd);
    close(cfd);
    return 0;
}