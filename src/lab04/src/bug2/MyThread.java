package bug2;

/**
 * Why does this code not block? We took the same lock twice!
 * There is nothing to modify for this example. Just explain why we do NOT
 * have a deadlock.
 */
public class MyThread implements Runnable {
    static int i;

/* In Java, a thread is allowed to acquire the same lock multiple times.
   When a thread holds a lock and encounters another synchronized block protected
   by the same lock (in this case, synchronized (this)), it doesn't create a deadlock.
   This is known as "reentrant synchronization" or "lock reentrancy."
*/

    @Override
    public void run() {
        synchronized (this) {
            synchronized (this) {
                i++;
            }
        }
    }
}
