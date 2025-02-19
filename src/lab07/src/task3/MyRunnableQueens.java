package task3;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.atomic.AtomicInteger;

class MyRunnableQueens implements Runnable {
    int[] graph;
    int step;
    AtomicInteger inQueue;
    ExecutorService executor;

    public MyRunnableQueens(int[] graph, int step, AtomicInteger inQueue, ExecutorService executor) {
        this.graph = graph;
        this.step = step;
        this.inQueue = inQueue;
        this.executor = executor;
    }

    @Override
    public void run() {
        if (Main.N == step) {
            synchronized (Main.lock) {
                if (Main.solutionsFound == 0) {
                    System.out.println("Solutions:");
                }
                System.out.print(++Main.solutionsFound + ") ");
                Main.printQueens(graph);
            }
        } else {
            for (int i = 0; i < Main.N; ++i) {
                int[] newGraph = graph.clone();
                newGraph[step] = i;

                if (Main.check(newGraph, step)) {
                    inQueue.incrementAndGet();
                    executor.submit(new MyRunnableQueens(newGraph, step + 1, inQueue, executor));
                }
            }
        }

        if (inQueue.decrementAndGet() == 0) {
            executor.shutdown();
        }
    }
}