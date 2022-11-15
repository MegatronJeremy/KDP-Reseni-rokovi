import java.util.LinkedList;
import java.util.Random;
import java.util.concurrent.Semaphore;

public class ChildCare {
	private static int K = 3;
	private static int n = 0;
	private static int ch = 0;

	private static Semaphore entry = new Semaphore(1);
	private static LinkedList<Semaphore> list = new LinkedList<>();
	private static LinkedList<Integer> who = new LinkedList<>();

	static class Parent extends Thread {
		private static int ID = 0;
		private int id = ID++;

		@Override
		public void run() {
			try {
				while (true) {
					int brings = new Random().nextInt(1, 4);
					bringChildren(brings);
					takeChildren(brings);
				}
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

		private void bringChildren(int brings) throws InterruptedException {
			entry.acquire();
			if (ch + brings > n * K || !list.isEmpty()) {
				Semaphore x = new Semaphore(0);
				who.add(ch);
				list.add(x);

				entry.release();
				x.acquire();
			}
			ch += brings;

			System.out.println("Parent " + id + " bringing " + brings + " children");

			signalCode();

			synchronized (this) {
				wait(1000);
			}
		}

		private void takeChildren(int takes) throws InterruptedException {
			entry.acquire();
			ch -= takes;

			System.out.println("Parent " + id + " taking " + takes + " children...");

			signalCode();

			synchronized (this) {
				wait(1000);
			}
		}

	}

	static class Nanny extends Thread {
		private static int ID = 0;
		private int id = ID++;

		@Override
		public void run() {
			try {
				while (true) {
					startJob();
					endJob();
				}
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

		}

		private void startJob() throws InterruptedException {
			entry.acquire();
			n++;

			System.out.println("Nanny " + id + " working...");

			signalCode();

			synchronized (this) {
				wait(1000);
			}
		}

		private void endJob() throws InterruptedException {
			entry.acquire();
			if (ch > (n - 1) * K || !list.isEmpty()) {
				Semaphore x = new Semaphore(0);
				who.add(0);
				list.add(x);

				entry.release();
				x.acquire();
			}
			n--;

			System.out.println("Nanny " + id + " going home...");

			signalCode();

			synchronized (this) {
				wait(1000);
			}

		}

	}

	private static void signalCode() {
		if (!list.isEmpty() && who.peek() == 0 && ch <= (n-1) * K) {
			who.pop();
			Semaphore x = list.pop();
			x.release();
		} else if (!list.isEmpty() && (ch + who.peek()) <= n * K) {
			who.pop();
			Semaphore x = list.pop();
			x.release();
		} else {
			entry.release();
		}
	}

	public static void main(String[] args) {
		int N = 5;
		for (int i = 0; i < N; i++) {
			new Parent().start();
			new Nanny().start();
		}
	}

}
