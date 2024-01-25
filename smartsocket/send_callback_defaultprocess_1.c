#include <rtworks/ipc.h>

int main(int argc, char **argv) {
    T_OPTION option;
    T_IPC_MSG msg;

    // Set the name of the project
    option = TutOptionLookup("project");
    TutOptionSetEnum(option, "smartsockets");

    // Connect to RTserver
    if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
        TutOut("Could not connect to RTserver!\n");
        return 1;
    }

    // Send an INFO message
    TipcSrvMsgWrite("/tutorial/lesson4", TipcMtLookupByNum(T_MT_INFO), TRUE, T_IPC_FT_STR, "Hello World!", NULL);

    // Send a NUMERIC_DATA message
    msg = TipcMsgCreate(TipcMtLookupByNum(T_MT_NUMERIC_DATA));
    TipcMsgSetDest(msg, "/tutorial/lesson4");
    TipcMsgAppendStrReal8(msg, "X", 10.0);
    TipcMsgAppendStrReal8(msg, "Y", 20.0);
    TipcMsgAppendStrReal8(msg, "Z", 30.0);
    TipcSrvMsgSend(msg, TRUE);
    TipcMsgDestroy(msg);

    // Send another INFO message (acting as a custom message for example purposes)
    TipcSrvMsgWrite("/tutorial/lesson4", TipcMtLookupByNum(T_MT_INFO), TRUE, T_IPC_FT_STR, "Custom Message", NULL);

    // Disconnect from RTserver
    TipcSrvDestroy(T_IPC_SRV_CONN_NONE);

    return 0;
}

