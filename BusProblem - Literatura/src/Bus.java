import java.util.ArrayList;

class Bus extends Thread {
	private static final int maxNumPassage = 50;
	private ArrayList<Station> sl = new ArrayList<>();
	private Station nextStation;
	private int numFree;
	private boolean toExit = false;
	private static int ID = 0;
	private int BID = ID++;

	public Bus() {
		numFree = maxNumPassage;
	}

	private synchronized Station getNextStation() {
		notifyAll();
		nextStation = sl.get(0);
		return nextStation;
	}

	public synchronized void exitTheBus(Station exitStation) {
		while (!toExit || exitStation != nextStation) {
			try {
				wait();
			} catch (InterruptedException e) {
			}
		}
		numFree++;
	}
	
	public synchronized void permissionToExit() {
		toExit = true;
		notifyAll();
		try {
			wait(500);
		} catch (InterruptedException e) {}
		toExit = false;
	}
	
	@Override
	public void run() {
		while (true) {
			nextStation = getNextStation();
			// travel()
			permissionToExit();
			numFree = nextStation.busEnter(numFree, this);
		}
	}
}