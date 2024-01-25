#include <rtworks/ipc.h>

int main(int argc, char **argv) {
    T_OPTION option;
    T_IPC_MSG msg;
    T_INT4 i;

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

    // Create a message of type NUMERIC_DATA
    msg = TipcMsgCreate(TipcMtLookupByNum(T_MT_NUMERIC_DATA));

    // Set the destination subject of the message
    TipcMsgSetDest(msg, "/tutorial/lesson4");

    // Each time through the loop send a NUMERIC_DATA message with 3 values
    for (i = 0; i < 30; i += 3) {
        // Reset the number of the fields in the message to zero
        TipcMsgSetNumFields(msg, 0);

        // Build the message with 3 variable-value pairs
        TipcMsgAppendStrReal8(msg, "X", (T_REAL8)i);
        TipcMsgAppendStrReal8(msg, "Y", (T_REAL8)(i + 1));
        TipcMsgAppendStrReal8(msg, "Z", (T_REAL8)(i + 2));

        // Send the message
        TipcSrvMsgSend(msg, TRUE);
        TipcSrvFlush();
    }

    // Destroy the message
    TipcMsgDestroy(msg);

    // Disconnect from RTserver
    TipcSrvDestroy(T_IPC_SRV_CONN_NONE);

    return 0;
}

