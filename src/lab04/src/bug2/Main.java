package bug2;

import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * DO NOT MODIFY
 */
public class Main {

    public static void main(String[] args) {
        Thread thread = new Thread(new MyThread());
        thread.start();
        try {
            thread.join();
        } catch (InterruptedException e) {
            Logger.getLogger(bug2.MyThread.class.getName())
                    .log(Level.SEVERE, "Thread interrupted", e);
        }

        System.out.println(MyThread.i);
    }

}
