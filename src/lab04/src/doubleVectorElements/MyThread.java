package doubleVectorElements;

import java.util.concurrent.BrokenBarrierException;
import java.util.logging.Level;
import java.util.logging.Logger;

public class MyThread implements Runnable {
    int id;
    int N;
    int[] v;
    int P;

    MyThread(int[] v, int id, int N, int P) {
        this.v = v;
        this.id = id;
        this.N = N;
        this.P = P;
    }

    @Override
    public void run() {
        int start = (int) (id * (double) N / P);
        int end = (int) ((id + 1) * (double) N / P);

        for (int i = start; i < end; i++) {
            v[i] = v[i] * 2;
        }

        try {
            doubleVectorElements.Main.barrier.await();
        } catch (InterruptedException | BrokenBarrierException e) {
            Logger.getLogger(doubleVectorElements.MyThread.class.getName())
                    .log(Level.SEVERE, "Thread interrupted", e);
        }
    }
}
