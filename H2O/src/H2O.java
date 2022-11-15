import java.util.concurrent.Semaphore;

public class H2O {
	private static Semaphore entry = new Semaphore(1);
	private static Semaphore oxy = new Semaphore(0);
	private static Semaphore exit = new Semaphore(0);

	private static int nh = 0;

	static public class H extends Thread {
		private static int ID = 0;
		private int id = ID++;

		@Override
		public void run() {
			try {
				while (true) {
					entry.acquire();
					nh++;
					if (nh != 2) {
						entry.release();
					} else {
						oxy.release();
					}

					bond();
					// bond

					exit.acquire();
					nh--;
					if (nh == 0) {
						entry.release();
					} else {
						exit.release();
					}
				}
			} catch (Exception e) {
			}
		}

		private void bond() throws InterruptedException {
			System.out.println("Hydrogen bonding..." + id);
			synchronized (this) {
				wait(1000);
			}
		}
	}

	static public class O extends Thread {
		private static int ID = 0;
		private int id = ID++;

		@Override
		public void run() {
			try {
				while (true) {
					oxy.acquire();

					bond();

					exit.release();
				}
			} catch (Exception e) {
			}
		}

		private void bond() throws InterruptedException {
			System.out.println("Oxygen bonding..." + id);
			synchronized (this) {
				wait(2000);
			}
		}
	}

	public static void main(String[] args) {
		for (int i = 0; i < 5; i++) {
			new H().start();
			new O().start();
			new H().start();
		}
	}
}
