import monitor.MonitorSC;

public class Garaza extends MonitorSC {
	private static final int N = 5;
	private static boolean rampaZauzeta = false;
	private static int mesta = N;
	private static Condition rampa[] = new Condition[] { new Condition(), new Condition() };
	private static int ticket[] = new int[2];
	private static int next[] = new int[2];
	private static int cnt[] = new int[2];

	private static boolean trazim_ulaz(int id) throws InterruptedException {
		e.acquire();

		System.out.println("Auto " + id + " trazi ulaz");

		if (mesta + cnt[1] - cnt[0] == 0) {
			e.release();
			return false;
		}

		cnt[0]++;
		int myT = ticket[0]++;

		if (rampaZauzeta || myT != next[0])
			rampa[0].waitSC(myT);
		else
			rampaZauzeta = true;

		next[0]++;

		e.release();

		return true;
	}

	private static void usao(int id) throws InterruptedException {
		e.acquire();

		System.out.println("Auto " + id + " usao");

		cnt[0]--;
		mesta--;

		signalCar();

		e.release();
	}

	private static void trazim_izlaz(int id) throws InterruptedException {
		e.acquire();

		System.out.println("Auto " + id + " trazi izlaz");

		cnt[1]++;
		int myT = ticket[1]++;

		if (myT != next[1] || rampaZauzeta)
			rampa[1].waitSC(myT);
		else
			rampaZauzeta = true;

		next[1]++;
		
		e.release();
	}

	private static void izasao(int id) throws InterruptedException {
		e.acquire();

		System.out.println("Auto " + id + " izasao");

		cnt[1]--;
		mesta++;
		
		signalCar();
		
		e.release();
	}

	private static void signalCar() throws InterruptedException {
		if (rampa[1].queue()) {
			rampa[1].signalSC();
		}else if (rampa[0].queue()) {
			rampa[0].signalSC();
		}else {
			rampaZauzeta = false;
		}
	}

	private static class Auto extends Thread {
		private int id = ID++;
		private static int ID = 0;

		@Override
		public void run() {
			try {
				while (true) {
					if (!trazim_ulaz(id)) {
						System.out.println("Auto " + id + " trazi drugi parking...");
						break;
					}
					synchronized (this) {
						wait(500);
					}
					usao(id);
					synchronized (this) {
						wait(1000);
					}
					trazim_izlaz(id);
					synchronized (this) {
						wait(500);
					}
					izasao(id);
				}
			} catch (Exception e) {
				System.out.println("bruh");
			}
		}
	}

	public static void main(String[] args) {
		for (int i = 0; i < 2 * N; i++) {
			new Auto().start();
		}
		Object o = new Object();
		synchronized (o) {
			try {
				o.wait(2000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		for (int i = 0; i < 2 * N; i++) {
			new Auto().start();
		}

	}

}
