using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Gostop.Model
{
	public class GameStep
	{
		private List<Action> _history;
		private GameStatus _finalStatus;
		private List<Action> _choices;

		// parameters are for getting result
		public GameStep(List<Action> history, GameStatus finalStatus,
			List<Action> choices)
		{
			_history = history;
			_finalStatus = finalStatus;
			_choices = choices;
		}

		#region // Getters
		public List<Action> History
		{
			get
			{
				return _history;
			}
		}
		public GameStatus FinalStatus
		{
			get
			{
				return _finalStatus;
			}
		}
		public List<Action> Choices
		{
			get
			{
				return _choices;
			}
		}

		#endregion
	}

	interface IGostopEngine
	{
		// Default constructor will be used
		GameStep Initialize(GameStatus initialStatus);
		GameStep ProceedWith(int action); // index in actionList parameter in GameStep
	}
}
