package bug5;

import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * DO NOT MODIFY
 */
public class Main {
    static final int N = 1000000; // Try a smaller value for N
    static final Object lockA = new Object();
    static final Object lockB = new Object();
    static int valueA = 0;
    static int valueB = 0;

    public static void main(String[] args) {
        Thread[] threads = new Thread[2];

        threads[0] = new Thread(new MyThreadA());
        threads[1] = new Thread(new MyThreadB());
        for (int i = 0; i < 2; i++) {
            threads[i].start();
        }

        for (int i = 0; i < 2; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                Logger.getLogger(bug5.MyThreadA.class.getName())
                        .log(Level.SEVERE, "ThreadA interrupted", e);
                Logger.getLogger(bug5.MyThreadB.class.getName())
                        .log(Level.SEVERE, "ThreadB interrupted", e);
            }
        }

        System.out.println("I finished");
    }
}
