package sampleForkJoinPoolFuture;

import java.util.concurrent.RecursiveTask;

public class FibonacciCalculator extends RecursiveTask<Integer> {
    private final int n;

    public FibonacciCalculator(int n) {
        this.n = n;
    }

    @Override
    protected Integer compute() {
        if (n <= 1) {
            return n; // Base case
        }

        FibonacciCalculator first = new FibonacciCalculator(n - 1);
        first.fork();

        FibonacciCalculator second = new FibonacciCalculator(n - 2);
        second.fork();

        return first.join() + second.join();
    }
}
