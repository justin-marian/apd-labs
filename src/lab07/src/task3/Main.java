package task3;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

public class Main {
    public static int N = 4;
    public static int solutionsFound = 0;
    public static final Object lock = new Object();

    public static boolean check (int[] arr, int step) {
        for (int i = 0; i <= step; i++) {
            for (int j = i + 1; j <= step; j++) {
                if (arr[i] == arr[j] || arr[i] + i == arr[j] + j || arr[i] + j == arr[j] + i)
                    return false;
            }
        }
        return true;
    }

    public static void printQueens (int[] sol) {
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
        ExecutorService executor = Executors.newFixedThreadPool(4);
        AtomicInteger inQueue = new AtomicInteger(1);
        int[] graph = new int[N];
        executor.submit(new MyRunnableQueens(graph, 0, inQueue, executor));
    }
}
