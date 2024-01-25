#include <rtworks/ipc.h>

int main(int argc, char **argv) {
    T_OPTION option;
    T_IPC_MSG msg;

    // Set the project name
    option = TutOptionLookup("project");
    TutOptionSetEnum(option, "smartsockets");

    TutCommandParseStr("setopt Log_Out_Data send3.msg");
    // Connect to RTserver
    if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
        TutOut("Could not connect to RTserver!\n");
        return 1;
    }

    // Create a NUMERIC_DATA message
    msg = TipcMsgCreate(TipcMtLookupByNum(T_MT_NUMERIC_DATA));

    // Set the destination of the message
    TipcMsgSetDest(msg, "/tutorial/lesson3");

    // Build the NUMERIC_DATA message with variable-value pairs
    TipcMsgAppendStr(msg, "Temperature");
    TipcMsgAppendReal8(msg, 23.5); // Example temperature value

    TipcMsgAppendStr(msg, "Pressure");
    TipcMsgAppendReal8(msg, 1.013); // Example pressure value

    // Send the message
    TipcSrvMsgSend(msg, TRUE);
    TipcSrvFlush();

    // Destroy the message
    TipcMsgDestroy(msg);

    // Disconnect from RTserver
    TipcSrvDestroy(T_IPC_SRV_CONN_NONE);

    return 0;
}

