import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;

/**
 * Test-and-set (TAS) Lock
 * 
 * @author tanding
 * @name TASLock
 * @implements Lock
 */
public class TASLock implements Lock {
	AtomicBoolean atomicBoolean = new AtomicBoolean(false);

	@Override
	public void lock() {
		while (atomicBoolean.getAndSet(true)) {
		}
		// System.out.println(Thread.currentThread().getName() + " locked");
	}

	@Override
	public void unlock() {
		// System.out.println(Thread.currentThread().getName() + " unlocked");
		atomicBoolean.set(false);
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
