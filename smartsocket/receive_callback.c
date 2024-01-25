#include <rtworks/ipc.h>

// Callback for processing NUMERIC_DATA messages
static void T_ENTRY ProcessNumericData(T_IPC_CONN conn, T_IPC_CONN_PROCESS_CB_DATA data, T_CB_ARG arg) {
    T_IPC_MSG msg = data->msg;
    T_STR var_name;
    T_REAL8 var_value;

    TutOut("Received NUMERIC_DATA message\n");

    // Set current field to first field in message
    TipcMsgSetCurrent(msg, 0);

    // Traverse message and print each name-value pair
    while (TipcMsgNextStrReal8(msg, &var_name, &var_value)) {
        TutOut("Var Name = %s; Value = %f\n", var_name, var_value);
    }
}


int main(int argc, char **argv) {
    T_OPTION option;
    T_IPC_MT mt;

    // Set the project name
    option = TutOptionLookup("project");
    TutOptionSetEnum(option, "smartsockets");

    // Connect to RTserver
    if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
        TutOut("Could not connect to RTserver!\n");
        return 1;
    }

    // Setup callback for NUMERIC_DATA messages
    mt = TipcMtLookupByNum(T_MT_NUMERIC_DATA);
    TipcSrvProcessCbCreate(mt, ProcessNumericData, NULL);


    // Start subscribing to a subject
    TipcSrvSubjectSetSubscribe("/my/numeric/data", TRUE);

    // Process all incoming messages using TipcSrvMainLoop
    TipcSrvMainLoop(T_TIMEOUT_FOREVER);

    // Disconnect from RTserver
    TipcSrvDestroy(T_IPC_SRV_CONN_NONE);

    return 0;
}

