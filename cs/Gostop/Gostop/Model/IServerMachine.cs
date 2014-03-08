using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Gostop.Model
{
	public class GameStep
	{
		private List<Event> _eventList;
		private GameStatus _gameStatus;
		private List<Action> _actionList;
		
		public GameStep(List<Event> eventList, GameStatus gameStatus,
			List<Action> actionList)
		{
			_eventList = eventList;
			_gameStatus = gameStatus;
			_actionList = actionList;
		}

		#region // Getters
		public GameStatus GameStatus
		{
			get
			{
				return _gameStatus;
			}
		}
		public List<Action> ActionList
		{
			get
			{
				return _actionList;
			}
		}
		public List<Event> EventList
		{
			get
			{
				return _eventList;
			}
		}
		#endregion
	}

	interface IServerMachine
	{
		// Default constructor will be used
		void Initialize(GameStatus initialStatus);

		GameStep ShowNextStep();

		void ProceedWith(int actionId);

	}
}
