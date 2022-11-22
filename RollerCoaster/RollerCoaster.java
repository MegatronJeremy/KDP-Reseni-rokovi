import java.util.concurrent.Semaphore;

public class RollerCoaster {
	private static int N = 20;
	private static int K = 3;
	private static int cnt = 0;

	private static Semaphore entry = new Semaphore(1);
	private static Semaphore start = new Semaphore(0);
	private static Semaphore exit = new Semaphore(0);

	static class Putnik extends Thread {
		private static int ID = 0;
		int id = ID++;
		@Override
		public void run() {
			try {
				while (true) {
					entry.acquire();
					cnt++;
					getOn();
					if (cnt == K) {
						start.release();
					} else {
						entry.release();
					}

					exit.acquire();
					cnt--;
					if (cnt > 0) {
						exit.release();
					} else {
						entry.release();
					}
				}
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

		private void getOn() {
			System.out.println("Passenger " + id + " got on");
		}
	}

	static class Tobogan extends Thread {
		@Override
		public void run() {
			try {
				while (true) {
					start.acquire();
					ride();
					exit.release();
				}
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

		private void ride() throws InterruptedException {
			System.out.println("Starting ride...");
			synchronized (this) {
				wait(500);
			}
			System.out.println("Ending ride....");

		}
	}
	public static void main(String[] args) {
		new Tobogan().start();
		for (int i = 0; i < N; i++)
			new Putnik().start();
	}
}
