package philosophersProblem;

import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * DO NOT MODIFY
 */
public class Main {
    public static final int N = 6;

    public static void main(String[] args) {
        Thread[] threads = new Thread[N];
        Object[] forks = new Object[N];
        System.out.println("Philosophers problem!");

        for (int i = 0; i < N; i++) {
            forks[i] = new Object();
        }

        for (int i = 0; i < N; i++) {
            threads[i] = new Thread(new Philosopher(i, forks[i], forks[(i + 1) % N]));
        }

        for (int i = 0; i < N; i++) {
            threads[i].start();
        }

        for (int i = 0; i < N; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                Logger.getLogger(philosophersProblem.
                        Main.class.getName()).log(Level.SEVERE, "Thread interrupted", e);
            }
        }
    }

}
/* DO NOT MODIFY */