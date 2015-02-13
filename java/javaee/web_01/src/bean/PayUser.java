package bean;

public class PayUser extends User {
	public PayUser(String id, String name, int payment) {
		super(id, name);
		this.payment = payment;
	}
	
	public PayUser(String id, String name) {
		this(id, name, 0);
	}
	
	public int getPayment() {
		return this.payment;
	}
	
	public void setPayment(int payment) {
		this.payment = payment;
	}
	
	private int payment;
}
