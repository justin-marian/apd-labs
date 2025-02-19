package task6;

import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveTask;

public class Main {
    public static int N = 4;
    public static int solutionsFound = 0;
    private static final Object lock = new Object();

    static class QueensTask extends RecursiveTask<Void> {
        private final int[] queens;
        private final int step;

        public QueensTask(int[] queens, int step) {
            this.queens = queens;
            this.step = step;
        }

        @Override
        protected Void compute() {
            if (N == step) {
                synchronized (lock) {
                    if (solutionsFound == 0) {
                        System.out.println("Solutions:");
                    }
                    System.out.print(++solutionsFound + ") ");
                    printQueens(queens);
                }
                return null;
            }

            for (int i = 0; i < N; i++) {
                int[] newQueens = queens.clone();
                newQueens[step] = i;

                if (check(newQueens, step)) {
                    QueensTask task = new QueensTask(newQueens, step + 1);
                    task.fork();
                    task.join();
                }
            }
            return null;
        }
    }

    private static boolean check(int[] arr, int step) {
        for (int i = 0; i <= step; i++) {
            for (int j = i + 1; j <= step; j++) {
                if (arr[i] == arr[j] || arr[i] + i == arr[j] + j || arr[i] + j == arr[j] + i)
                    return false;
            }
        }
        return true;
    }

    private static void printQueens(int[] sol) {
        StringBuilder aux = new StringBuilder();
        aux.append("[");
        for (int i = 0; i < sol.length; i++) {
            aux.append("(").append(sol[i] + 1).append(", ").append(i + 1).append("), ");
        }
        if (sol.length > 0) {
            aux.setLength(aux.length() - 2);
        }
        aux.append("]");
        System.out.println(aux);
    }

    public static void main(String[] args) {
        int[] queens = new int[N];
        ForkJoinPool pool = new ForkJoinPool();
        pool.invoke(new QueensTask(queens, 0));
        pool.shutdown();
    }
}
