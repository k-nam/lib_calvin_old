using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Gostop.Model {
	public class GameStep {
		// parameters are for getting result
		public GameStep(List<Action> history, GameStatus finalStatus,
			List<Action> choices) {
			History = history;
			FinalStatus = finalStatus;
			Choices = choices;
		}

		public List<Action> History { get; set; }
		public GameStatus FinalStatus { get; set; }
		public List<Action> Choices { get; set; }
	}

	interface IGostopEngine {
		// Default constructor will be used
		GameStep StartWith(GameStatus initialStatus);
		GameStep Start();
		GameStep ProceedWith(int action); // index in actionList parameter in GameStep
	}
}
