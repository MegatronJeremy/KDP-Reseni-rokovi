import java.util.concurrent.Semaphore;

public class Parking {
	private static final int N = 10;
	private static final int M = 20;
	private static final int K = 5;

	private static int ID = 0;
	private static int pri = 2, ewi = 0, eri = 0, lwi = 0, lri = 0, ke = 0, kl = 0, de = 0, dl = 0, empty = N;

	private static Semaphore sem[] = new Semaphore[2 * N + 1];

	public Parking() {
		for (int i = 0; i < 2 * N; i++) {
			sem[i] = new Semaphore(0);
		}
		sem[2 * N] = new Semaphore(1);
	}

	private class Car extends Thread {
		private int id = ID++;

		public void run() {
			try {
				while (true) {
					requestEntry();
					System.out.println("Car " + id + " entering parking lot...");
					signalEntry();
					synchronized (this) {
						wait(1000);
					}
					requestExit();
					System.out.println("Car " + id + " exiting parking lot...");
					signalExit();

				}
			} catch (Exception e) {
				System.out.println("Car " + id + " looking for another parking lot...");
			}
		}

		private void signalExit() {
			if ((pri == 1 || dl == 0) && de > 0) {
				de--;
				int ind = eri;
				eri = (eri + 1) % N;
				sem[ind].release();
			} else if (dl > 0) {
				dl--;
				int ind = lri + N;
				lri = (lri + 1) % N;
				sem[ind].release();
			} else
				sem[2 * N].release();

		}

		private void requestExit() throws InterruptedException {
			sem[2 * N].acquire();
			if (pri == 1 && de > 0 && empty != 0) {
				dl++;
				int ind = lwi + N;
				lwi = (lwi + 1) % N;
				System.out.println("Exit blocked..." + pri + " " + de);
				sem[2 * N].release();
				sem[ind].acquire();
			}

			empty++;

			if (kl == K) {
				pri = 1;
				kl = 0;
			} else
				kl++;
			ke = 0;
		}

		private void signalEntry() {
			if ((pri == 2 || de == 0) && dl > 0) {
				dl--;
				int ind = lri + N;
				lri = (lri + 1) % N;
				sem[ind].release();
			} else if (de > 0) {
				de--;
				int ind = eri;
				eri = (eri + 1) % N;
				sem[ind].release();
			} else
				sem[2 * N].release();
		}

		private void requestEntry() throws InterruptedException {
			sem[2 * N].acquire();
			if (empty + dl - de == 0) {
				sem[2 * N].release();
				throw new InterruptedException();
			}
			if (empty == 0 || (pri == 2 && dl > 0)) {
				de++;
				int ind = ewi;
				ewi = (ewi + 1) % N;
				System.out.println("Entry blocked..." + pri + " " + dl + " " + empty);
				sem[2 * N].release();
				sem[ind].acquire();
			}

			empty--;

			if (ke == K) {
				pri = 2;
				ke = 0;
			} else
				ke++;

			kl = 0;
		}

	}

	public static void main(String[] args) {
		Parking p = new Parking();

		for (int i = 0; i < M; i++) {
			p.new Car().start();
		}

	}

}
