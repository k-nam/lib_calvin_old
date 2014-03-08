package card;

public enum CardType {
	TrueLight(30),
	RainLight(30),
	FiveBirdsTen(10),
	NormalTen(10),
	RedFive(5),
	GrassFive(5),
	BlueFive(5),
	NormalFive(5),
	DoubleShell(0),
	NormalShell(0),
	
	EmptyCard(-1);
	
	private int value;
	
	private CardType(int value) {
		this.value = value;
	}
}
