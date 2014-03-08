package queue;

public class Producer implements Runnable {
	
	public Producer(Factory factory, int ammount) {
		this.factory = factory;
		this.ammount = ammount;
	}
	
	public void run() {
		while (ammount-- > 0) {
			if (factory.pushBack(new String("bb")) == false) {
				System.out.println("Producer error");
				return;
			}
			if (ammount % 1000000 == 0) {
				System.out.println("Producer: inserted: " + ammount);
			}
		}
	}
	
	private Factory factory;
	private int ammount;

	
}
