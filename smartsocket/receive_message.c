#include <rtworks/ipc.h>

int main(int argc, char **argv) {
    T_OPTION option;
    T_IPC_MSG msg;
    T_STR var_name;
    T_REAL8 var_value;

    // Set the project name
    option = TutOptionLookup("project");
    TutOptionSetEnum(option, "smartsockets");
    TutCommandParseStr("setopt Log_In_Data receive3.msg");
    // Connect to RTserver
    if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
        TutOut("Could not connect to RTserver!\n");
        return 1;
    }

    // Subscribe to receive messages sent to the subject
    TipcSrvSubjectSetSubscribe("/tutorial/lesson3", TRUE);

    // Get the next message from the queue
    msg = TipcSrvMsgNext(T_TIMEOUT_FOREVER);

    // Print out each variable name-value pair in the message
    TutOut("Contents of NUMERIC_DATA message:\n");
    TutOut("---------------------------------\n");
    while (TipcMsgNextStr(msg, &var_name)) {
        TipcMsgNextReal8(msg, &var_value);
        TutOut("Var name = %s, Var Value = %f\n", var_name, var_value);
    }

    // Disconnect from RTserver
    TipcSrvDestroy(T_IPC_SRV_CONN_NONE);

    return 0;
}

