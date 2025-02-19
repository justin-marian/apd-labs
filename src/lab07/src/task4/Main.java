package task4;

import java.util.ArrayList;
import java.util.concurrent.RecursiveTask;
import java.util.concurrent.ForkJoinPool;

public class Main {
    static int[][] graph = { { 0, 1 }, { 0, 4 }, { 0, 5 }, { 1, 0 }, { 1, 2 },
                             { 1, 6 }, { 2, 1 }, { 2, 3 }, { 2, 7 }, { 3, 2 },
                             { 3, 4 }, { 3, 8 }, { 4, 0 }, { 4, 3 }, { 4, 9 },
                             { 5, 0 }, { 5, 7 }, { 5, 8 }, { 6, 1 }, { 6, 8 },
                             { 6, 9 }, { 7, 2 }, { 7, 5 }, { 7, 9 }, { 8, 3 },
                             { 8, 5 }, { 8, 6 }, { 9, 4 }, { 9, 6 }, { 9, 7 } };
    static class PathfindingTask extends RecursiveTask<Void> {
        private final ArrayList<Integer> partialPath;
        private final int destination;

        public PathfindingTask(ArrayList<Integer> partialPath, int destination) {
            this.partialPath = partialPath;
            this.destination = destination;
        }

        @Override
        protected Void compute() {
            int lastNodeInPath = partialPath.get(partialPath.size() - 1);

            if (lastNodeInPath == destination) {
                System.out.println(partialPath);
            } else {
                for (int[] ints : graph) {
                    if (ints[0] == lastNodeInPath && !partialPath.contains(ints[1])) {
                        ArrayList<Integer> newPartialPath = new ArrayList<>(partialPath);
                        newPartialPath.add(ints[1]);
                        PathfindingTask task = new PathfindingTask(newPartialPath, destination);
                        task.fork();
                        task.join();
                    }
                }
            }
            return null;
        }
    }

    public static void main(String[] args) {
        ArrayList<Integer> partialPath = new ArrayList<>();
        partialPath.add(0);
        ForkJoinPool forkJoinPool = new ForkJoinPool();
        forkJoinPool.invoke(new PathfindingTask(partialPath, 3));
    }
}
