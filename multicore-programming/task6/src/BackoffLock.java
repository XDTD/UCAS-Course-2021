import java.util.Random;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;

/**
 * Exponential Backoff Lock
 * 
 * @author tanding
 * @name BackoffLock
 * @implements Lock
 */
public class BackoffLock implements Lock {
	private Random random = new Random();
	AtomicBoolean atomicBoolean = new AtomicBoolean(false);
	private static final int MIN_DELAY = 16;
	private static final int MAX_DELAY = 1024;

	@Override
	public void lock() {
		int delay = MIN_DELAY;
		while (true) {
			while (atomicBoolean.get()) {
			}
			if (!atomicBoolean.getAndSet(true)) {
				// System.out.println(Thread.currentThread().getName() + " locked");
				return;
			} else {
				try {
					Thread.sleep(random.nextInt(delay));
					if (delay < MAX_DELAY) {
						delay = 2 * delay;
					}
				} catch (InterruptedException ex) {
				}
			}
		}
	}

	@Override
	public void unlock() {
		// System.out.println(Thread.currentThread().getName() + " unlocked");
		atomicBoolean.set(false);
	}

	/**
	 * Backoff for random duration.
	 * 
	 * @throws java.lang.InterruptedException
	 */
	public void randomDurationBackoff() throws InterruptedException {
		int limit = MIN_DELAY;
		int delay = random.nextInt(limit);
		if (limit < MAX_DELAY) {
			limit = 2 * limit;
		}
		Thread.sleep(delay);
	}

	@Override
	public Condition newCondition() {
		throw new java.lang.UnsupportedOperationException();
	}

	@Override
	public boolean tryLock(long time, TimeUnit unit) throws InterruptedException {
		throw new java.lang.UnsupportedOperationException();
	}

	@Override
	public boolean tryLock() {
		throw new java.lang.UnsupportedOperationException();
	}

	@Override
	public void lockInterruptibly() throws InterruptedException {
		throw new java.lang.UnsupportedOperationException();
	}
}