package philosophersProblem;

import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Lock[] locks = new Lock[N];
 * philosopher(int id) {
 *   while (true) {
 *     if (id != N - 1) {
 *       locks[id].lock();
 *       locks[id + 1].lock();
 *       // eat
 *       locks[id].release();
 *       locks[id + 1].release();
 *       // think
 *     } else {
 *       locks[0].lock();
 *       locks[N - 1].lock();
 *       // eat
 *       locks[0].release();
 *       locks[N - 1].release();
 *       // think
 *     }
 *   }
 * }
 */

public class Philosopher implements Runnable {
    private final Object leftFork;
    private final Object rightFork;
    private final int id;

    public Philosopher(int id, Object leftFork, Object rightFork) {
        this.leftFork = leftFork;
        this.rightFork = rightFork;
        this.id = id;
    }

    private void sleep() {
        try {
            Thread.sleep(100);
        } catch (InterruptedException e) {
            Logger.getLogger(philosophersProblem.
                    Philosopher.class.getName()).log(Level.SEVERE, "Thread interrupted", e);
        }
    }

    @Override
    public void run() {
        if (id == 0) {
            synchronized (leftFork) {
                sleep(); // delay added to make sure the deadlock is visible
                synchronized (rightFork) {
                    System.out.println("Philosopher " + id + " is eating");
                }
            }
        } else {
            synchronized (rightFork) {
                sleep(); // delay added to make sure the deadlock is visible
                synchronized (leftFork) {
                    System.out.println("Philosopher " + id + " is eating");
                }
            }
        }
    }
}
