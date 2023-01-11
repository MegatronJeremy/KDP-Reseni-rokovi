// kao region
public class Station {
	private String name;
	private int id;
	private int numP, numFP;
	private boolean busIN;
	private Bus bus;

	public Station(String name) {
		this.name = name;
		bus = null;
		busIN = false;
	}

	public synchronized Bus waitBus() {
		while (true) {
			// cekam da ode prvi autobus
			while (busIN) {
				try {
					wait();
				} catch (InterruptedException e) {
				}
			}
			numP++;
			while (!busIN) {
				try {
					wait();
				} catch (InterruptedException e) {
				}
			}
			numP--;
			// dozvoljavam da i izadju svi
			if (numP == 0)
				notifyAll();

			// ako je broj slobodnih mesta veci od nula
			if (numFP > 0) {
				numFP--;
				return bus;
			}
		}
	}

	public synchronized int busEnter(int numFree, Bus b) {
		while (busIN) {
			try {
				wait();
			} catch (InterruptedException e) {
			}
		}
		busIN = true;
		// mogli smo dobiti numFree iz bus objekta
		numFP = numFree;
		bus = b;
		notifyAll();
		while (numP != 0) {
			try {
				wait();
			} catch (InterruptedException e) {
			}
		}
		busIN = false;
		bus = null;
		notifyAll();
		// i bez ovog vracanja - samo da se izmeni polje
		return numFP;
	}
}
