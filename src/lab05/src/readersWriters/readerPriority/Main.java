package readersWriters.readerPriority;

import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.Semaphore;

/**
 * // the number of readers simultaneously reading from the shared resource
 * int readers = 0;
 * // mutex (or semaphore) used to change the number of readers
 * mutex mutexNumberOfReaders; // or semaphore mutexNumberOfReaders(1);
 * // semaphore (or mutex) used to protect the shared resource
 * semaphore readWrite(1); // or mutex readWrite
 *-
 * reader (int id) {
 *    while (true) {
 *      mutexNumberOfReaders.lock();
 *      readers = readers + 1;
 *      // if it is the first reader, then we reserve the memory area so that no writer enters
 *      if (readers == 1) {
 *          readWrite.acquire();
 *      };
 *      mutexNumberOfReaders.unlock();
 *      // read from shared resource;
 *      mutexNumberOfReaders.lock();
 *      readers = readers - 1;
 *      // if it's the last reader, we release the memory area from which it was read
 *      if (readers == 0) {
 *          readWrite.release();
 *      }
 *      mutexNumberOfReaders.unlock();
 *   }
 * }
 * // /////////////////////////////////////////////// /////////////////////////////////////////////////// //
 * writer (int id) {
 *    while (true) {
 *      // writer enters the shared resource
 *      readWrite.acquire();
 *      // write to shared resource;
 *      // the writer frees the resource
 *      readWrite.release();
 *   }
 * }
 */

public class Main {
    public static final int WRITERS = 6;
    public static final int READERS = 8;
    public static int currentReaders = 0;
    public static final boolean[] hasRead = new boolean[READERS];
    public static final boolean[] hasWritten = new boolean[WRITERS];

    // used to make sure threads try to go in the same to the shared zone
    public static CyclicBarrier barrier = new CyclicBarrier(READERS + WRITERS);

    // TODO: add semaphores
    public static Semaphore readWriteSem = new Semaphore(1);
    public static Semaphore mutexNumberOfReaders = new Semaphore(1);

    public static void main(String[] args) throws InterruptedException {
        Thread[] readers = new Reader[READERS];
        Thread[] writers = new Writer[WRITERS];

        for (int i = 0; i < READERS; i++) {
            readers[i] = new Reader(i);
        }

        for (int i = 0; i < WRITERS; i++) {
            writers[i] = new Writer(i);
        }

        for (var reader: readers) {
            reader.start();
        }

        for (var writer: writers) {
            writer.start();
        }

        for (var reader: readers) {
            reader.join();
        }

        for (var writer: writers) {
            writer.join();
        }
    }
}
