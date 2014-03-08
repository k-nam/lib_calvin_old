package server;

import status.GameStatus;
import client.ClientManagerInterface;

public interface ServerInterface {
	public void processTurn(GameStatus status, ClientManagerInterface clientManager);
}
