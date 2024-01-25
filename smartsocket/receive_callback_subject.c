#include <rtworks/ipc.h>

static void T_ENTRY ProcessLesson4(T_IPC_CONN conn, T_IPC_CONN_PROCESS_CB_DATA data, T_CB_ARG arg);

/* ProcessLesson4 - callback for processing messages published to the /tutorial/lesson4 subject */
static void T_ENTRY ProcessLesson4(T_IPC_CONN conn, T_IPC_CONN_PROCESS_CB_DATA data, T_CB_ARG arg) {
    T_IPC_MSG msg = data->msg;
    T_STR mt_name;
    T_IPC_MT mt;
    T_INT4 mt_num;
    T_STR msg_text;
    T_STR var_name;
    T_REAL8 var_value;

    // Get the message type and print it out
    TipcMsgGetType(msg, &mt);
    TipcMtGetName(mt, &mt_name);
    TutOut("*** Received message of type <%s>.\n", mt_name);
    TipcMtGetNum(mt, &mt_num);

    switch(mt_num) {
        case T_MT_INFO:
            TipcMsgSetCurrent(msg, 0);
            TipcMsgNextStr(msg, &msg_text);
            TutOut("Text from message = %s\n", msg_text);
            break;
        case T_MT_NUMERIC_DATA:
            TipcMsgSetCurrent(msg, 0);
            while (TipcMsgNextStrReal8(msg, &var_name, &var_value)) {
                TutOut("Var Name = %s; Value = %f\n", var_name, var_value);
            }
            break;
        default:
            TutOut("Unable to process messages of this type!\n");
            break;
    }
}

int main(int argc, char **argv) {
    T_OPTION option;

    // Set the project name
    option = TutOptionLookup("project");
    TutOptionSetEnum(option, "smartsockets");

    // Connect to the RTserver
    if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
        TutOut("Could not connect to RTserver!\n");
        exit(T_EXIT_FAILURE);
    }

    // Setup callback to catch all messages sent to the "/tutorial/lesson4" subject
    TipcSrvSubjectCbCreate("/tutorial/lesson4", NULL, ProcessLesson4, NULL);

    // Start subscribing to the "/tutorial/lesson4" subject
    TipcSrvSubjectSetSubscribe("/tutorial/lesson4", TRUE);

    // Read and process all incoming messages
    TipcSrvMainLoop(T_TIMEOUT_FOREVER);

    // Disconnect from RTserver
    TipcSrvDestroy(T_IPC_SRV_CONN_NONE);

    return 0;
}

