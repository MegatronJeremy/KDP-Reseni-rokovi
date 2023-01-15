import java.util.ArrayList;
import java.util.concurrent.Semaphore;

public class AtomicBroadcast {
	private static final int N = 100;
	private static final int B = 50;

	private static int buffer[] = new int[B];
	private static int cnt[] = new int[B];
	private static int dr[] = new int[B];
	private static int items[] = new int[N];
	
	private static Semaphore entry[] = new Semaphore[B];
	private static Semaphore empty[] = new Semaphore[B];
	private static Semaphore full[] = new Semaphore[B];


	public AtomicBroadcast() {
		for (int i = 0; i < B; i++) {
			entry[i] = new Semaphore(0);
			empty[i] = new Semaphore(1);
			full[i] = new Semaphore(0);
		}
	}

	private class Producer extends Thread {
		private int id;
		private int wi = 0;

		public Producer(int id) {
			this.id = id;
		}

		public void run() {
			try {
				while (true) {
					int item = produce();

					empty[wi].acquire();

					buffer[wi] = item;
					cnt[wi] = 0;
					for (int i = 0; i < N; i++)
						items[i]++;
					
					if (dr[wi] > 0) {
						dr[wi]--;
						full[wi].release();
					} else
						entry[wi].release();
					
					wi = (wi + 1) % B;
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

		private int produce() throws InterruptedException {
			System.out.println("Producing " + ++id);
			synchronized (this) {
				wait(200);
			}
			return id;
		}
	}

	private class Consumer extends Thread {
		private int id;
		private int ri = 0;

		public Consumer(int id) {
			this.id = id;
		}

		@Override
		public void run() {
			try {
				while (true) {
					entry[ri].acquire();

					if (items[id] == 0) {
						dr[ri]++;
						entry[ri].release();
						full[ri].acquire();
					}
					items[id]--;

					if (dr[ri] > 0) {
						dr[ri]--;
						full[ri].release();
					} else
						entry[ri].release();

					int item = buffer[ri];
					consume(item);

					entry[ri].acquire();

					cnt[ri]++;
					if (cnt[ri] == N)
						empty[ri].release();
					else
						entry[ri].release();
					
					ri = (ri + 1) % B;
				}
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

		private void consume(int item) throws InterruptedException {
			System.out.println(id + " consuming: " + item);
			synchronized(this) {
				wait(1000);
			}
		}
	}

	public static void main(String[] args) {
		AtomicBroadcast ac = new AtomicBroadcast();

		ac.new Producer(0).start();
		
		for (int i = 0; i < N; i++) {
			ac.new Consumer(i).start();
		}
	}

}
