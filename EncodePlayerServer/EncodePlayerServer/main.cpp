#include <cstdio>

#include "Process.h"
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#ifndef Print(str)
#define Print(str)  {printf("%s(%d)<%s> pid=%d %s\n",__FILE__,__LINE__,__FUNCTION__,getpid(),str);}
#endif // !Print(const char* log)

#ifndef Err_Return(PreserverRet,RealRet,ret)
#define Err_Return(PreserverRet,RealRet,ret) {if(PreserverRet != RealRet) return ret;}
#endif


int CreateServer(CProcess* process)
{
    Print("");
    return 0;
}

int CreateClient(CProcess* process)
{
    Print("");
    int fd = -1;
    int ret = process->RecvFD(fd);
    Print((std::string("ret:") + std::to_string(ret)).c_str());
    Print((std::string("fd:") + std::to_string(fd)).c_str());

    sleep(1);
    char msg[100] = "";
    lseek(fd, 0, SEEK_SET);
    read(fd, msg, 100);
    Print(msg);
    close(fd);
    return 0;
}

int main()
{
    CProcess processServer, processClient;

    bool bSucceed = processServer.SetFunctionEntry(CreateServer, &processServer);
    Print(bSucceed ? " server create succeed" : " server create failed");
    int ret = processServer.CreateSubProcess();
    Print(ret == 0 ? " server create subprocess succeed" : " server create subprocess failed");
    Err_Return(0, ret, -1);

    bSucceed = processClient.SetFunctionEntry(CreateClient, &processClient);
    Print(bSucceed ? " client create succeed" : " client create failed");
    ret = processClient.CreateSubProcess();
    Print(ret == 0 ? " client create subprocess succeed" : " client create subprocess failed");
    Err_Return(0, ret, -2);

    usleep(100 * 000);

    int fd = open("1.txt", O_RDWR | O_CREAT | O_APPEND);
    if (fd == -1) return -3;
    Print((std::string("fd:") + std::to_string(fd)).c_str());
    ret = processClient.SendFD(fd);
    if (ret != 0)
    {
        Print((std::string("errno:") + std::to_string(errno) + std::string(strerror(errno))).c_str());
    }
    const char* data = "import data to file!";
    write(fd, data , strlen(data));

    close(fd);

    getchar();
    return 0;
}

