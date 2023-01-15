import java.util.Random;
import java.util.concurrent.Semaphore;

public class Lift {

	private static final int N = 9;
	private static Semaphore e[] = new Semaphore[N];
	private static int cekaOd[] = new int[N];
	private static int cekaDo[] = new int[N];
	private static Semaphore od[] = new Semaphore[N];
	private static Semaphore doo[] = new Semaphore[N];
	private static Semaphore lift = new Semaphore(0);

	static {
		for (int i = 0; i < N; i++) {
			e[i] = new Semaphore(1);
			od[i] = new Semaphore(0);
			doo[i] = new Semaphore(0);
		}
	}

	public static class Elevator extends Thread {
		private int sprat = 0;
		private int dir = 1;

		@Override
		public void run() {
			try {
				while (true) {
					System.out.println("Na spratu " + sprat);

					e[sprat].acquire();

					if (cekaDo[sprat] > 0) {
						doo[sprat].release();
						lift.acquire();
					}
					if (cekaOd[sprat] > 0) {
						od[sprat].release();
						lift.acquire();
					}

					e[sprat].release();

					System.out.println("Putujem...");
					synchronized (this) {
						wait(1000);
					}

					if (sprat == N - 1 && dir == 1)
						dir = -1;
					if (sprat == 0 && dir == -1)
						dir = 1;
					sprat += dir;
				}
			} catch (InterruptedException e) {
			}
		}
	}

	public static class Osoba extends Thread {
		int dir = 1;
		int id = ID++;
		private static int ID = 0;

		@Override
		public void run() {
			try {
				while (true) {
					int src = new Random().nextInt(N - 1);
					int dst = new Random().nextInt(src + 1, N);
					if (dir == 1) {
						src = new Random().nextInt(N - 1);
						dst = new Random().nextInt(src + 1, N);
						dir = 0;
					} else {
						src = new Random().nextInt(1, N);
						dst = new Random().nextInt(src);
						dir = 1;
					}
					
					
					e[src].acquire();
					
					cekaOd[src]++;

					e[src].release();
					
					od[src].acquire();
					
					System.out.println("Putnik " + id + " ukrcan na " + src + " do " + dst);
					
					cekaOd[src]--;
					cekaDo[dst]++;

					if (cekaOd[src] > 0) {
						od[src].release();
					} else {
						lift.release();
					}
					
					doo[dst].acquire();
					cekaDo[dst]--;
					
					System.out.println("Putnik " + id + " od " + src + " iskrcan na " + dst);

					if (cekaDo[dst] > 0) {
						doo[dst].release();
					} else {
						lift.release();
					}
				}
			} catch (InterruptedException e) {
			}
		}
		
		public static void main(String[] args) {
			new Elevator().start();
			for (int i = 0; i < N; i++) {
				new Osoba().start();
			}
		}
	}

}
