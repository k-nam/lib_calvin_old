using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Gostop.Model
{
	public class GameStep
	{
		private List<Event> _events;
		private GameStatus _finalStatus;
		private List<Action> _choices;
		
        // parameters are for getting result
		public GameStep(List<Event> events, GameStatus finalStatus,
			List<Action> choices)
		{
			_events = events;
			_finalStatus = finalStatus;
			_choices = choices;
		}

        #region // Getters
        public List<Event> Events
        {
            get
            {
                return _events;
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

	interface IServerMachine
	{
        // Default constructor will be used
        GameStep Initialize(GameStatus initialStatus);
        GameStep ProceedWith(int action); // index in actionList parameter in GameStep
    }
}
