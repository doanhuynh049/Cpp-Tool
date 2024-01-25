/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/threads/threads.java#1 $
 */

import com.smartsockets.*;

/*
 * These two classes demonstrate one possible way to use multiple
 * threads to process incoming messages.
 *
 * The main class creates a process callback, and five threads to
 * process messages.  The run methods for each thread simply call
 * srv.mainLoop, which will cause the registered process callback
 * to be called by a different thread for each message received.
 *
 * To generate traffic, the main class also sends two messages to
 * RTserver, which are retransmitted by the process callback until
 * it has been sent 20 times.  When all five threads time out after
 * the 20 messages have been reflected off RTserver, the process will
 * exit.
 *
 * An interesting thing to note about this example is that there is
 * no connection between threads and callbacks.  This is true even
 * if the thread itself registers the callback.  Callbacks are
 * "per-connection" properties, not "per-thread" properties.  Which
 * thread actually ends up calling the process callback depends on
 * which thread calls srv.process (which is part of srv.mainLoop).
 * That, in turn, depends on the scheduler for the particular JVM
 * that runs this program.
 *
 * NOTE: RTserver must be running on the local machine for this
 * example to work.
 */

public class threads implements TipcProcessCb {

    // the number of times the message has been resent by the process callback
    int msgCount = 0;

    public void process(TipcMsg msg, Object arg) {

        TipcSrv srv = (TipcSrv)arg;
        String subj = msg.getDest();
        /*
         * The default name assigned to each thread will be "Thread-N" where N
         * will be a number, probably starting at 7.  Threads 0-6 are either
         * JVM processing threads or SmartSockets internal threads.
         */
        String name = Thread.currentThread().getName();
        System.out.println(name + "\tgot message for " + subj);
        if (msgCount < 20) {
            try {
                srv.send(msg); // will be flushed in mainLoop
            }
            catch (TipcException e) {
                Tut.exitFailure("exception sending message: " + e.getMessage());
            }
            msgCount++;
        }
    }

    static public void main(String[] argv) {

        TipcSrv srv;
        Thread[] subThreads;

        try {
            /*
             * Even though we subscribe to "/bar" and send a message to that
             * subject, we don't process it because we haven't registered a
             * callback for that subject, nor have we registered a default
             * callback.  The "/bar" message will be sent and received, but
             * it will be discarded by whichever thread reads it since there
             * are no matching callback for it.
             */
            srv = TipcSvc.getSrv();
            srv.setSubjectSubscribe("/foo", true);
            srv.setSubjectSubscribe("/bar", true);

            threads t = new threads();
            srv.addProcessCb(t, "/foo", srv);

            TipcMsg msg = TipcSvc.createMsg(TipcMt.INFO);
            msg.setDest("/foo");
            srv.send(msg);
            msg.setDest("/bar");
            srv.send(msg);

            /*
             * Make all five threads using SubThread as the thread
             * body, and start them.
             */
            subThreads = new Thread[5];
            for (int i=0;i<5;++i) {
                subThreads[i] = new Thread(new SubThread(srv));
                System.out.println("made thread " + subThreads[i].getName());
                subThreads[i].start();
            }

            /*
             * Wait for the five threads to exit.
             */
            for (int i=0;i<5;++i) {
                subThreads[i].join();
            }

            srv.destroy();
        }
        catch (TipcException e) {
            e.printStackTrace();
            System.out.println("exception: " + e.getMessage());
        }
        catch (InterruptedException e) {
            e.printStackTrace();
            System.out.println("exception: " + e.getMessage());
        }
    }

}

class SubThread implements Runnable {

    TipcSrv srv;

    public SubThread(TipcSrv s) {
        srv = s;
    }

    public void run() {
        try {
            System.out.println(Thread.currentThread().getName() + "\tstarted.");
            /*
             * When this times out, run will return and the thread will die.
             */
            srv.mainLoop(5.0);
        }
        catch (TipcException e) {
            e.printStackTrace();
            System.out.println("exception: " + e.getMessage());
        }
    }
}
