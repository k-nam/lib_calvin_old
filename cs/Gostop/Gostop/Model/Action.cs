using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Gostop.Model
{
	public enum Actions
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
		protected Actions _actionId;
		public Actions ActionId
		{
			get
			{
				return _actionId;
			}
		}

		public static string PrintAction(Actions action)
		{
			switch (action)
			{
				case Actions.HitCardAction:
					return "HitCardAction";
				case Actions.ChooseAction:
					return "ChooseCardAction";
				case Actions.GoOrStopAction:
					return "GoStopActions";
				case Actions.ShakeAction:
					return "ShakeAction";
				case Actions.BombAction:
					return "BombAction";
				case Actions.FourCardAction:
					return "FourCardAction";
			}
			return null;
		}
		public override string ToString()
		{
			return PrintAction(_actionId);
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

	public class HitCardAction : Action
	{
		private int _card;
		public HitCardAction(int card)
		{
			_actionId = Actions.HitCardAction;
			_card = card;
		}
		public int CardId
		{
			get
			{
				return _card;
			}
		}
		public override string ToString()
		{
			return (base.ToString() + " " + Card.GetCard(_card));
		}
	}

	public class ChooseAction : Action
	{
		private HashSet<int> _cards;
		public ChooseAction(HashSet<int> cards)
		{
			_actionId = Actions.ChooseAction;
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
			return (base.ToString() + " " + Card.PrintCardSet(_cards));
		}
	}

	public class GoOrStopAction : Action
	{
		private bool _isGo;
		GoOrStopAction(bool isGo)
		{
			_actionId = Actions.GoOrStopAction;
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
			_actionId = Actions.ShakeAction;
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
			return (base.ToString() + " " + Card.PrintCardSet(_cards));
		}
	}

	public class BombAction : Action
	{
		private HashSet<int> _cards;
		public BombAction(HashSet<int> cards)
		{
			_actionId = Actions.BombAction;
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
			return (base.ToString() + " " + Card.PrintCardSet(_cards));
		}
	}

	public class FourCardAction : Action
	{
		private Months _month;
		public FourCardAction(Months month)
		{
			_actionId = Actions.FourCardAction;
			_month = month;
		}
		public Months Month
		{
			get
			{
				return _month;
			}
		}
		public override string ToString()
		{
			return (base.ToString() + " " + Card.PrintMonth(_month));
		}
	}
	
}
	

