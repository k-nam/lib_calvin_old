using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Gostop.Model
{
	public enum ActionType
	{
		HitCardAction,
		ChooseAction,
		GoOrStopAction,
		ShakeAction,
		BombAction,
		FourCardAction
	}

	// Represents single action which a player can take in game
	public class Action
	{
        protected Player _player;
		protected ActionType _type;

        public Player Player
        {
            get
            {
                return _player;
            }
        }
        public ActionType Type
		{
			get
			{
				return _type;
			}
		}

		public static string PrintAction(ActionType action)
		{
			switch (action)
			{
				case ActionType.HitCardAction:
					return "HitCardAction";
				case ActionType.ChooseAction:
					return "ChooseCardAction";
				case ActionType.GoOrStopAction:
					return "GoStopActions";
				case ActionType.ShakeAction:
					return "ShakeAction";
				case ActionType.BombAction:
					return "BombAction";
				case ActionType.FourCardAction:
					return "FourCardAction";
			}
			return null;
		}
		public override string ToString()
		{
			return PrintAction(_type);
		}
		public static string PrintActionList(List<Action> actions)
		{
			StringBuilder builder = new StringBuilder();
			List<Action>.Enumerator enumerator = actions.GetEnumerator();
			while (enumerator.MoveNext())
			{
				builder.AppendLine(enumerator.Current.ToString());
			}
			return builder.ToString();
		}
	}

	public class HitAction : Action
	{
		private int _card;
		public HitAction(int card)
		{
			_type = ActionType.HitCardAction;
			_card = card;
		}
		public int Card
		{
			get
			{
				return _card;
			}
		}
		public override string ToString()
		{
			return (base.ToString() + ": " + Model.Card.GetCard(_card));
		}
	}

	public class ChooseCardAction : Action
	{
        private int _card;
        public ChooseCardAction(int card)
        {
            _type = ActionType.HitCardAction;
            _card = card;
        }
        public int Card
        {
            get
            {
                return _card;
            }
        }
        public override string ToString()
        {
            return (base.ToString() + ": " + Model.Card.GetCard(_card));
        }
    }

	public class GoOrStopAction : Action
	{
		private bool _isGo;
		GoOrStopAction(bool isGo)
		{
			_type = ActionType.GoOrStopAction;
			_isGo = isGo;
		}
		public bool IsGo
		{
			get
			{
				return _isGo;
			}
		}
	}

	public class ShakeAction : Action
	{
		private HashSet<int> _cards;
		public ShakeAction(HashSet<int> cards)
		{
			_type = ActionType.ShakeAction;
			_cards = cards;
		}
		public HashSet<int> Cards
		{
			get
			{
				return _cards;

			}
		}
		public override string ToString()
		{
			return (base.ToString() + ": " + Card.PrintCardSet(_cards));
		}
	}

	public class BombAction : Action
	{
		private HashSet<int> _cards;
		public BombAction(HashSet<int> cards)
		{
			_type = ActionType.BombAction;
			_cards = cards;
		}
		public HashSet<int> Cards
		{
			get
			{
				return _cards;

			}
		}
		public override string ToString()
		{
			return (base.ToString() + ": " + Card.PrintCardSet(_cards));
		}
	}

	public class FourCardAction : Action
	{
		private Month _month;
		public FourCardAction(Month month)
		{
			_type = ActionType.FourCardAction;
			_month = month;
		}
		public Month Month
		{
			get
			{
				return _month;
			}
		}
		public override string ToString()
		{
			return (base.ToString() + ": " + Card.PrintMonth(_month));
		}
	}
	
}
	

