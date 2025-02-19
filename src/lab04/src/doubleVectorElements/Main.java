package doubleVectorElements;

import java.util.concurrent.CyclicBarrier;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Main {
    // the program should work for any P <= N
    public static final int P = 4;
    public static CyclicBarrier barrier;

    public static void main(String[] args) {
        int N = 100000013;
        int[] v = new int[N];

        // int P = Runtime.getRuntime().availableProcessors();
        Thread[] threads = new Thread[P];
        barrier = new CyclicBarrier(P);

        for (int i = 0; i < N; i++) {
            v[i] = i;
        }

        // Parallelize me using P threads
        /* for (int i = 0; i < N; i++) v[i] = v[i] * 2; */

        for (int i = 0; i < P; i++) { // ADDED P as PARAMETER SOLVED THE PROBLEM!
            threads[i] = new Thread(new MyThread(v, i, N, P));
            threads[i].start();
        }

        for (int i = 0; i < P; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                Logger.getLogger(shortestPathsFloyd_Warshall.MyThread.class.getName())
                        .log(Level.SEVERE, "Thread interrupted", e);
            }
        }

        for (int i = 0; i < N; i++) {
            if (v[i] != i * 2) {
                System.out.println("Wrong answer");
                System.exit(1);
            }
        }
        System.out.println("Correct");
    }
}
