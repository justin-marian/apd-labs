package task2;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

public class Main  {
    static int N = 10;
    static int COLORS = 3;
    static int[][] graph = { { 0, 1 }, { 0, 4 }, { 0, 5 }, { 1, 0 }, { 1, 2 },
                             { 1, 6 }, { 2, 1 }, { 2, 3 }, { 2, 7 }, { 3, 2 },
                             { 3, 4 }, { 3, 8 }, { 4, 0 }, { 4, 3 }, { 4, 9 },
                             { 5, 0 }, { 5, 7 }, { 5, 8 }, { 6, 1 }, { 6, 8 },
                             { 6, 9 }, { 7, 2 }, { 7, 5 }, { 7, 9 }, { 8, 3 },
                             { 8, 5 }, { 8, 6 }, { 9, 4 }, { 9, 6 }, { 9, 7 } };

    public static boolean verifyColors (int[] colors, int step) {
        for (int i = 0; i < step; i++) {
            if (colors[i] == colors[step] && isEdge(i, step))
                return false;
        }
        return true;
    }

    private static boolean isEdge(int a, int b) {
        for (int[] ints : graph) {
            if (ints[0] == a && ints[1] == b)
                return true;
        }
        return false;
    }

    static void printColors(int[] colors) {
        StringBuilder aux = new StringBuilder();
        for (int color : colors) {
            aux.append(color).append(" ");
        }
        System.out.println(aux);
    }


    public static void main(String[] args) {
        ExecutorService tpe = Executors.newFixedThreadPool(4);
        AtomicInteger inQueue = new AtomicInteger(1);
        int[] colors = new int[N];
        tpe.submit(new MyRunnableColor(graph, colors, 0, inQueue, tpe));
    }
}
