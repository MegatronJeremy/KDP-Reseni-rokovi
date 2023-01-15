
import java.util.Arrays;
import java.util.Random;

import monitor.MonitorSC;

public class DPFifo extends MonitorSC {
	private static int N = 10;

	private static Condition c = new Condition();
	private static int state[] = new int[N], next = 2;

	public static void take_forks(int id) throws InterruptedException {
		e.acquire();

		System.out.println("Philosopher " + id + " wants to eat");

		int l = left(id), r = right(id);

		if (c.queue() || state[l] != 0 || state[r] != 0) {
			state[id] = 2;
			c.waitSC(next++ * N + id);
		} else
			state[id] = 1;

		if (c.empty()) next = 2;

		System.out.println("Philosopher " + id + " eating " + next );
		System.out.println(Arrays.toString(state));
		
		e.release();
	}

	public static void leave_forks(int id) throws InterruptedException {
		e.acquire();

		System.out.println("Philosopher " + id + " done eating");

		state[id] = 0;

		boolean ret = test();
		if (ret) test();

		e.release();
	}

	private static int left(int id) {
		return (id + N - 1) % N;
	}

	private static int right(int id) {
		return (id + 1) % N;
	}
	
	private static boolean test() {
		boolean ret = false;
		if (!c.queue()) return ret;
		int id = c.minrank() % N, l = left(id), r = right(id);
		
		if (state[l] != 1 && state[r] != 1) {
			ret = true;
			state[id] = 1;
			c.signalSC();
		}
		
		return ret;
	}
	
	private static class Philosopher extends Thread {
		private int id = ID++;
		private static int ID = 0;

		@Override
		public void run() {
			try {
				while (true) {
					think();
					take_forks(id);
					eat();
					leave_forks(id);
				}
			} catch (InterruptedException e) {
			}
		}

		private void eat() throws InterruptedException {
			synchronized (this) {
				wait(1000 + new Random().nextInt(2000));
			}
		}

		private void think() throws InterruptedException {
			synchronized (this) {
				wait(1000 + new Random().nextInt(1000));
			}
		}
	}

	public static void main(String[] args) {
		for (int i = 0; i < N; i++) {
			new Philosopher().start();
		}
	}

}
