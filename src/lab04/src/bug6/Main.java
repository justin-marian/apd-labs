package bug6;

import java.util.logging.Level;
import java.util.logging.Logger;

public class Main {
    public static int NUMBER_OF_THREADS = 8;

    public static void main(String[] args) {
        Thread[] threads = new Thread[NUMBER_OF_THREADS];

        for (int i = 0; i < NUMBER_OF_THREADS; i++) {
            threads[i] = new MyThread();
            threads[i].start();
        }

        for (int i = 0; i < NUMBER_OF_THREADS; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                Logger.getLogger(bug6.MyThread.class.getName())
                        .log(Level.SEVERE, "Thread interrupted", e);
            }
        }

        if (Singleton.getNumberOfInstances() == 1) {
            System.out.println("Correct");
        } else {
            System.out.println("Incorrect, there should be exactly one instance");
        }
    }
}
