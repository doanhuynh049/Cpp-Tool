#include <rtworks/ipc.h>
int main(int argc, char **argv)
{
    T_IPC_MSG msg;
    T_STR msg_text;
    TipcSrvSubjectSetSubscribe("/tutorial/lesson1", TRUE);
    msg = TipcSrvMsgNext(T_TIMEOUT_FOREVER);
    TipcMsgSetCurrent(msg, 0);
    TipcMsgNextStr(msg, &msg_text);
    TutOut("Text from message = %s\n", msg_text);
    TipcSrvDestroy(T_IPC_SRV_CONN_NONE);
}
