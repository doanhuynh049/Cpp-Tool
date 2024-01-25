#include <rtworks/ipc.h>

static void T_ENTRY DefaultCallback(T_IPC_CONN conn,
                                    T_IPC_CONN_PROCESS_CB_DATA data, T_CB_ARG arg);
static void T_ENTRY ProcessSubjectStatus(T_IPC_CONN conn, 
                                         T_IPC_CONN_PROCESS_CB_DATA data, T_CB_ARG arg);

// Default callback function
static void T_ENTRY DefaultCallback(T_IPC_CONN conn,
                                    T_IPC_CONN_PROCESS_CB_DATA data, T_CB_ARG arg) {
    TipcMsgPrintError(data->msg);
}

// Callback to process MON_SUBJECT_SUBSCRIBE_STATUS message
static void T_ENTRY ProcessSubjectStatus(T_IPC_CONN conn,
                                         T_IPC_CONN_PROCESS_CB_DATA data, T_CB_ARG arg) {
    T_IPC_MSG msg = data->msg;
    T_STR subject_name;
    T_STR *subscribe_client_names;
    T_INT4 num_subscribe_clients;
    T_STR start_client;
    T_STR stop_client;
    T_INT4 n;

    // Set current field
    TipcMsgSetCurrent(msg, 0);

    // Get the fields from the message
    TipcMsgNextStr(msg, &subject_name);
    TipcMsgNextStrArray(msg, &subscribe_client_names, &num_subscribe_clients);
    TipcMsgNextStr(msg, &start_client);
    TipcMsgNextStr(msg, &stop_client);

    // Print out the new information received about the subject
    TutOut("Received change notice for subject <%s>\n", subject_name);
    TutOut("Number of clients subscribed to <%s> = %d\n", subject_name, num_subscribe_clients);
    for (n = 0; n < num_subscribe_clients; n++) {
        TutOut(" [%d] %s\n", n, subscribe_client_names[n]);
    }
    TutOut("Client who just subscribed: %s\n", start_client);
    TutOut("Client who just unsubscribed: %s\n", stop_client);
    TutOut("==================================================\n");
}

int main(int argc, char **argv) {
    T_IPC_MT mt;

    // Set the project name
    TutCommandParseStr("setopt project smartsockets");

    // Connect to RTserver
    if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
        TutOut("Could not connect to RTserver.\n");
        exit(T_EXIT_FAILURE);
    }

    // Create callback to process MON_SUBJECT_SUBSCRIBE_STATUS messages
    mt = TipcMtLookupByNum(T_MT_MON_SUBJECT_SUBSCRIBE_STATUS);
    TipcSrvProcessCbCreate(mt, ProcessSubjectStatus, NULL);

    // Setup callback to receive unwanted message types
    TipcSrvDefaultCbCreate(DefaultCallback, NULL);

    // Start watching who is subscribed to /tutorial/lesson5 subject
    TipcMonSubjectSubscribeSetWatch("/tutorial/lesson4", TRUE);

    // Read messages from RTserver and call the callbacks
    TipcSrvMainLoop(T_TIMEOUT_FOREVER);

    // Disconnect from RTserver
    TipcSrvDestroy(T_IPC_SRV_CONN_NONE);
}

