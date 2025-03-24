#include <cstdio>

#include "Process.h"

int CreateServer(CProcess* process);

int CreateClient(CProcess* process);

int main()
{
    CProcess processServer, processClient;

    processServer.SetFunctionEntry(CreateServer, &processServer);
    processServer.CreateSubProcess();

    processClient.SetFunctionEntry(CreateClient, &processClient);
    processClient.CreateSubProcess();
    return 0;
}

int CreateServer(CProcess* process)
{

}

int CreateClient(CProcess* process)
{

}
