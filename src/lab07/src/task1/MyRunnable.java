package task1;

import java.util.ArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.atomic.AtomicInteger;

public class MyRunnable implements Runnable {
    int[][] graph;
    ExecutorService tpe;
    ArrayList<Integer> partialPath;
    int destination;
    AtomicInteger inQueue;

    public MyRunnable(int[][] graph, ArrayList<Integer> partialPath, AtomicInteger inQueue, ExecutorService tpe, int i) {
        this.graph = graph;
        this.tpe = tpe;
        this.partialPath = partialPath;
        this.destination = i;
        this.inQueue = inQueue;
    }

    @Override
    public void run() {
        if (partialPath.get(partialPath.size() - 1) == destination) {
            System.out.println(partialPath);
            int left = inQueue.decrementAndGet();
            if (left == 0) {
                tpe.shutdown();
            }
            return;
        }

        int lastNodeInPath = partialPath.get(partialPath.size() - 1);

        for (int[] edge : graph) {
            if (edge[0] == lastNodeInPath && !partialPath.contains(edge[1])) {
                ArrayList<Integer> newPartialPath = new ArrayList<>(partialPath);
                newPartialPath.add(edge[1]);
                inQueue.incrementAndGet();
                tpe.submit(new MyRunnable(graph, newPartialPath, inQueue, tpe, destination));
            }
        }

        int left = inQueue.decrementAndGet();
        if (left == 0) {
            tpe.shutdown();
        }
    }
}
