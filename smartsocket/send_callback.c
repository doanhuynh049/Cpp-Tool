#include <rtworks/ipc.h>
#include <stdio.h>

int main(int argc, char **argv) {
    T_OPTION option;
    T_IPC_MSG msg;
    T_IPC_MT mt;
    int i;

    // Set the project name
    option = TutOptionLookup("project");
    TutOptionSetEnum(option, "smartsockets");

    // Connect to RTserver
    if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
        fprintf(stderr, "Could not connect to RTserver!\n");
        return 1;
    }

    // Create and send NUMERIC_DATA messages
    mt = TipcMtLookupByNum(T_MT_NUMERIC_DATA);
    for (i = 0; i < 5; ++i) {
        msg = TipcMsgCreate(mt);
        TipcMsgSetDest(msg, "/my/numeric/data");
        TipcMsgAppendStrReal8(msg, "X", 10.0 * i);
        TipcMsgAppendStrReal8(msg, "Y", 20.0 * i);
        TipcMsgAppendStrReal8(msg, "Z", 30.0 * i);
        TipcSrvMsgSend(msg, TRUE);
        TipcSrvFlush();
        TipcMsgDestroy(msg);
    }
    // Disconnect from RTserver
    TipcSrvDestroy(T_IPC_SRV_CONN_NONE);

    return 0;
}

