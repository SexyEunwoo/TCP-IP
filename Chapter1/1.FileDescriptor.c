#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
    char buf[] = "Hello World!";
    int fd = open("test2.txt", O_CREAT | O_WRONLY | O_RDONLY);

    if(fd == -1)
    {
        printf("open() error! \n");
        return -1;
    }
    else
    {
        printf("File Descriptor is %d \n", fd);
    }
    
    write(fd, buf, sizeof(buf));

    close(fd);
    
    return 0;
}