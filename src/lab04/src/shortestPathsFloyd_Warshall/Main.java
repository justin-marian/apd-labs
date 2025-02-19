package shortestPathsFloyd_Warshall;

import java.util.logging.Level;
import java.util.logging.Logger;

import java.util.concurrent.CyclicBarrier;

public class Main {
    public static final int P = 4;
    public static CyclicBarrier barrier;

    public static void main(String[] args) {
        int M = 9;
        int[][] graph = {{0, 1, M, M, M},
                         {1, 0, 1, M, M},
                         {M, 1, 0, 1, 1},
                         {M, M, 1, 0, M},
                         {M, M, 1, M, 0}};

         for (int k = 0; k < 5; k++) {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    graph[i][j] = Math.min(graph[i][k] + graph[k][j], graph[i][j]);
                }
            }
        }

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                System.out.print(graph[i][j] + " ");
            }
            System.out.println();
        }

        System.out.println();

        // Parallelize the original code in order to compare them!
        Thread[] threads = new Thread[P];
        barrier = new CyclicBarrier(P);

        for (int i = 0; i < P; i++) {
            threads[i] = new Thread(new MyThread(graph, i, P));
            threads[i].start();
        }

        for (int i = 0; i < P; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                Logger.getLogger(MyThread.class.getName())
                        .log(Level.SEVERE, "Thread interrupted", e);
            }
        }

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                System.out.print(graph[i][j] + " ");
            }
            System.out.println();
        }
    }
}
