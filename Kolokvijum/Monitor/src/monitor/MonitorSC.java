package monitor;

import java.util.LinkedList;
import java.util.TreeMap;
import java.util.concurrent.Semaphore;

public class MonitorSC {
	protected static Semaphore e = new Semaphore(1);

	protected static class Condition {
		private LinkedList<Semaphore> lst = new LinkedList<>();
		private TreeMap<Integer, Semaphore> pq = new TreeMap<>();
		private int cnt = 0;

		public Condition() {
		}

		public void waitSC() throws InterruptedException {
			cnt++;
			Semaphore x = new Semaphore(0);
			lst.add(x);
			e.release();
			x.acquire();
			e.acquire();
		}

		public void signalSC() {
			if (cnt == 0)
				return;
			cnt--;
			if (lst.isEmpty()) {
				Semaphore p = pq.pollFirstEntry().getValue();
				p.release();
			} else {
				Semaphore p = lst.poll();
				p.release();
			}
		}

		public void signalAll() {
			while (cnt > 0)
				signalSC();
		}

		public boolean queue() {
			return cnt > 0;
		}

		public boolean empty() {
			return cnt == 0;
		}

		public void waitSC(int rank) throws InterruptedException {
			cnt++;
			Semaphore x = new Semaphore(0);
			pq.put(rank, x);
			e.release();
			x.acquire();
			e.acquire();
		}

		public int minrank() {
			if (pq.isEmpty())
				return -1;
			return pq.firstKey();
		}

	}

}