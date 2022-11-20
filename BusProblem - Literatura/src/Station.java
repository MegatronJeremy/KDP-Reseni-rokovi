
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
		try {
			while (true) {
				while (busIN) {
					wait();
				}
				numP++;
				while (!busIN) {
					wait();
				}
				numP--;
				if (numP == 0)
					notifyAll();
				if (numFP > 0) {
					numFP--;
					return bus;
				}
			}
		} catch (InterruptedException e) {
		}
		return null;
	}

	public synchronized int busEnter(int numFree, Bus b) {
		try {
			while (busIN) {
				wait();
			}
			busIN = true;
			numFP = numFree;
			bus = b;
			notifyAll();
			while (numP != 0) {
				wait();
			}
			busIN = false;
			bus = null;
			notifyAll();
		} catch (InterruptedException e) {
		}
		return numFP;
}
