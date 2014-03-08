package client;

public enum ClientId {
	First,
	Second,
	Third;
	
	public String toString() {
		if (this.equals(ClientId.First)) {
			return "Player 1";
		} else if (this.equals(ClientId.Second)) {
			return "Player 2";
		} else {
			return "Player 3";
		}
	}
}
