package queue;

public class Consumer implements Runnable {

	public Consumer(Factory factory) {
		this.factory = factory;
	}

	@Override
	public void run() {
		while (factory.hasNext()) {
			Object thisItem = factory.getNext();
			if (thisItem == null) { // queue empty

			} else {
				process(thisItem);
			}
		}
	}

	public void process(Object item) {

	}

	private Factory factory;

}
