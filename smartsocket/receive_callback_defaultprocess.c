#include <rtworks/ipc.h>

// Callback for processing NUMERIC_DATA messages
static void T_ENTRY ProcessNumData(T_IPC_CONN conn, T_IPC_CONN_PROCESS_CB_DATA data, T_CB_ARG arg) {
    T_IPC_MSG msg = data->msg;
    T_STR var_name;
    T_REAL8 var_value;

    TutOut("Received NUMERIC_DATA message\n");
    TipcMsgSetCurrent(msg, 0);
    while (TipcMsgNextStrReal8(msg, &var_name, &var_value)) {
        TutOut("Var Name = %s; Value = %f\n", var_name, var_value);
    }
}

// Default callback function
static void T_ENTRY DefaultReceiveFunc(T_IPC_CONN conn, T_IPC_CONN_PROCESS_CB_DATA data, T_CB_ARG arg) {
    T_IPC_MSG msg = data->msg;
    T_IPC_MT mt;
    T_STR name;

    TipcMsgGetType(msg, &mt);
    TipcMtGetName(mt, &name);
    TutOut("Receive: unexpected message type name is <%s>\n", name);
}

int main(int argc, char **argv) {
    T_OPTION option;
    T_IPC_MT mt;

    option = TutOptionLookup("project");
    TutOptionSetEnum(option, "smartsockets");

    if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
        TutOut("Could not connect to RTserver!\n");
        return 1;
    }
    mt = TipcMtLookupByNum(T_MT_NUMERIC_DATA);
    TipcSrvProcessCbCreate(mt, ProcessNumData, NULL);
    TipcSrvDefaultCbCreate(DefaultReceiveFunc, NULL);
    TipcSrvSubjectSetSubscribe("/tutorial/lesson4", TRUE);
    TipcSrvMainLoop(T_TIMEOUT_FOREVER);

    TipcSrvDestroy(T_IPC_SRV_CONN_NONE);

    return 0;
}
