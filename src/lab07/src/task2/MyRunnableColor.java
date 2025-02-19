package task2;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.atomic.AtomicInteger;

class MyRunnableColor implements Runnable {
    int[][] graph;
    int[] colors;
    int step;
    ExecutorService tpe;
    AtomicInteger inQueue;

    public MyRunnableColor(int[][] graph, int[] colors, int step, AtomicInteger inQueue, ExecutorService tpe) {
        this.graph = graph;
        this.colors = colors;
        this.step = step;
        this.tpe = tpe;
        this.inQueue = inQueue;
    }

    @Override
    public void run() {
        if (step == Main.N) {
            Main.printColors(colors);
        } else {
            for (int i = 0; i < Main.COLORS; i++) {
                int[] newColors = colors.clone();
                newColors[step] = i;
                if (Main.verifyColors(newColors, step)) {
                    inQueue.incrementAndGet();
                    tpe.submit(new MyRunnableColor(graph, newColors, step + 1, inQueue, tpe));
                }
            }
        }

        if (inQueue.decrementAndGet() == 0) {
            tpe.shutdown();
        }
    }
}