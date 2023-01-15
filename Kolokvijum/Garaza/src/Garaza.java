
public class Garaza {
	private static int nu = 0, ni = 0, tiketU = 0, tiketI = 0, nextU = 0, nextI = 0;
	private static final int N = 5;
	private static boolean rampa = false;
	private static int mesta = N;
	
	private static synchronized boolean trazim_ulaz(int id) throws InterruptedException {
		System.out.println("Auto " + id + " trazi ulaz");
		
		if (mesta + ni - nu == 0) return false;
		nu++;
		int redU = tiketU++;
		while (mesta == 0 || redU != nextU || ni > 0 || rampa) Garaza.class.wait();
		rampa = true;
		return true;
	}
	
	private static synchronized void usao(int id) {
		System.out.println("Auto " + id + " usao");
		
		nu--;
		rampa = false;
		mesta--;
		nextU++;
		Garaza.class.notifyAll();
	}
	
	private static synchronized void trazim_izlaz(int id) throws InterruptedException {
		System.out.println("Auto " + id + " trazi izlaz");
		
		ni++;
		int redI = tiketI++;
		while (redI != nextI || rampa) Garaza.class.wait();
		rampa = true;
	}
	
	private static synchronized void izasao(int id) {
		System.out.println("Auto " + id + " izasao");
		
		ni--;
		rampa = false;
		mesta++;
		nextI++;
		Garaza.class.notifyAll();
	}
	
	private static class Auto extends Thread {
		private int id = ID++;
		private static int ID = 0;
		@Override
		public void run() {
			try {
				while (true) {
					if (!trazim_ulaz(id)) {
						System.out.println("Auto "+ id + " trazi drugi parking...");
						break;
					}
					synchronized(this) {
						wait(500);
					}
					usao(id);
					synchronized(this) {
						wait(500);
					}
					trazim_izlaz(id);
					synchronized (this) {
						wait(500);
					}
					izasao(id);
				}
			} catch (Exception e) {}
		}
	}
	
	public static void main(String[] args) {
		for (int i = 0; i < 2 * N; i++) {
			new Auto().start();
		}
		Object o = new Object();
		synchronized(o) {
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
