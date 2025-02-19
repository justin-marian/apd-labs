package multipleProducersMultipleConsumers;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Buffer {
	private final ArrayBlockingQueue<Integer> array = new ArrayBlockingQueue<>(Main.N);
	private final Lock lock;
	private final Condition notEmpty;
	private final Condition notFull;
	private boolean isEmpty;

	public Buffer() {
		this.lock = new ReentrantLock();
		this.notEmpty = lock.newCondition();
		this.notFull = lock.newCondition();
		this.isEmpty = true;
	}

	void put(int value) throws InterruptedException {
		lock.lock();
		try {
			while (!isEmpty) {
				notFull.await();
			}
			array.add(value);
			isEmpty = false;
			notEmpty.signalAll();
		} finally {
			lock.unlock();
		}
	}

	int get() throws InterruptedException {
		lock.lock();
		try {
			while (isEmpty) {
				notEmpty.await();
			}
			int value = array.remove();
			isEmpty = true;
			notFull.signalAll();
			return value;
		} finally {
			lock.unlock();
		}
	}
}
