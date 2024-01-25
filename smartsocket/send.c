#include <rtworks/ipc.h>
int main(int argc, char **argv)
{
    TipcSrvMsgWrite("/tutorial/lesson1", TipcMtLookupByNum(T_MT_INFO), TRUE, T_IPC_FT_STR, "Hello World!", NULL);
    TipcSrvFlush();
    TipcSrvDestroy(T_IPC_SRV_CONN_NONE);
}
