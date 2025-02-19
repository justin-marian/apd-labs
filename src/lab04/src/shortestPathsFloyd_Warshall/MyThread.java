package shortestPathsFloyd_Warshall;

import java.util.concurrent.BrokenBarrierException;
import java.util.logging.Level;
import java.util.logging.Logger;

public class MyThread implements Runnable {
    int[][] graph;
    int id, P;

    MyThread(int[][] graph, int id, int P) {
        this.graph = graph;
        this.id = id;
        this.P = P;
    }

    @Override
    public void run() {
        int start = (int) (this.id * ((double) graph.length / P));
        int end = (int) Math.min((this.id + 1) * ((double) graph.length / P), graph.length);

        for (int k = 0; k < 5; k++) {
            for (int i = start; i < end; i++) {
                for (int j = 0; j < 5; j++) {
                    graph[i][j] = Math.min(graph[i][k] + graph[k][j], graph[i][j]);
                }
            }
            try {
                Main.barrier.await();
            } catch (InterruptedException | BrokenBarrierException e) {
                Logger.getLogger(MyThread.class.getName())
                        .log(Level.SEVERE, "Thread interrupted", e);
            }

        }
    }
}
