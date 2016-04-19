using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Gostop.Model {
	// Represents single action which a player can take in game
	// Some actions are voluntary (result of a choice), others are inevitable
	// But from the game engine's point of view, those two need not be distinguished
	public enum ActionType {
		HitAction,
		VoidHitAction, 
		FlipHitAction, // automatic
		FuckAction, // automatic
		UnFuckAction, // automatic
		DadakAction, // automatic
		KissAction, // automatic
		TakeAction, // automatic
		CleanupAction, // automatic
		StealAction, // automatic
		ChooseAction,
		GoOrStopAction,
		ShakeAction,
		BombAction,
		FourCardAction,
		EndTurnAction, // automatic
		TakeMoneyAction, // automatic
		EndGameAction, // automatic
	}

	public class Action {
		protected Player _player;
		protected ActionType _type;

		public Player Player {
			get {
				return _player;
			}
		}
		public ActionType Type {
			get {
				return _type;
			}
		}

		public static string PrintAction(ActionType action) {
			switch (action) {
				case ActionType.HitAction:
					return "HitAction";
				case ActionType.VoidHitAction:
					return "VoidHitAction";
				case ActionType.FlipHitAction:
					return "FlipHitAction";
				case ActionType.FuckAction:
					return "FuckAction";
				case ActionType.UnFuckAction:
					return "UnFuckAction";
				case ActionType.DadakAction:
					return "DadakAction";
				case ActionType.KissAction:
					return "KissAction";
				case ActionType.TakeAction:
					return "TakeAction";
				case ActionType.CleanupAction:
					return "CleanupAction";
				case ActionType.StealAction:
					return "StealAction";
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
				case ActionType.EndTurnAction:
					return "EndTurnAction";
				case ActionType.TakeMoneyAction:
					return "TakeMoneyAction";
				case ActionType.EndGameAction:
					return "EndGameAction";
			}
			return null;
		}
		public override string ToString() {
			return PrintAction(_type);
		}
		public static string PrintActionList(List<Action> actions) {
			StringBuilder builder = new StringBuilder();
			List<Action>.Enumerator enumerator = actions.GetEnumerator();
			while (enumerator.MoveNext()) {
				builder.AppendLine(enumerator.Current.ToString());
			}
			return builder.ToString();
		}
	}

	// hit with hand card
	public class HitAction : Action {
		private int _card;
		public HitAction(Player player, int card) {
			_player = player;
			_type = ActionType.HitAction;
			_card = card;
		}
		public int Card {
			get {
				return _card;
			}
		}
		public override string ToString() {
			return (base.ToString() + ": " + Model.Card.GetCard(_card));
		}
	}
	// just flip (only possible after bomb)
	public class VoidHitAction : Action {
		private int _card;
		public VoidHitAction(Player player) {
			_player = player;
			_type = ActionType.VoidHitAction;
		}
	}
	public class FlipHitAction : Action {
		private int _card;
		public FlipHitAction(Player player, int card) {
			_player = player;
			_type = ActionType.FlipHitAction;
			_card = card;
		}
		public int Card {
			get {
				return _card;
			}
		}
		public override string ToString() {
			return (base.ToString() + ": " + Model.Card.GetCard(_card));
		}
	}

	public class FuckAction : Action {
		private int _card;
		public FuckAction(Player player, int card) {
			_player = player;
			_type = ActionType.FuckAction;
			_card = card;
		}
		public int Card {
			get {
				return _card;
			}
		}
		public override string ToString() {
			return (base.ToString() + ": " + Model.Card.GetCard(_card));
		}
	}
	public class UnFuckAction : Action {
		private int _card;
		public UnFuckAction(Player player, int card) {
			_player = player;
			_type = ActionType.UnFuckAction;
			_card = card;
		}
		public int Card {
			get {
				return _card;
			}
		}
		public override string ToString() {
			return (base.ToString() + ": " + Model.Card.GetCard(_card));
		}
	}
	public class DadakAction : Action {
		private int _card;
		public DadakAction(Player player, int card) {
			_player = player;
			_type = ActionType.DadakAction;
			_card = card;
		}
		public int Card {
			get {
				return _card;
			}
		}
		public override string ToString() {
			return (base.ToString() + ": " + Model.Card.GetCard(_card));
		}
	}
	public class KissAction : Action {
		private int _card;
		public KissAction(Player player, int card) {
			_player = player;
			_type = ActionType.KissAction;
			_card = card;
		}
		public int Card {
			get {
				return _card;
			}
		}
		public override string ToString() {
			return (base.ToString() + ": " + Model.Card.GetCard(_card));
		}
	}
	public class CleanupAction : Action {
		public CleanupAction(Player player) {
			_player = player;
			_type = ActionType.CleanupAction;
		}
	}
	public class TakeAction : Action {
		private HashSet<int> _cards;
		public TakeAction(Player player, HashSet<int> cards) {
			_player = player;
			_type = ActionType.TakeAction;
			_cards = cards;
		}
		public HashSet<int> Cards {
			get {
				return _cards;
			}
		}
		public override string ToString() {
			return (base.ToString() + ": " + Card.PrintCardSet(_cards));
		}
	}
	public class StealAction : Action {
		private HashSet<int> _cards;
		public StealAction(Player player, HashSet<int> cards) {
			_player = player;
			_type = ActionType.StealAction;
			_cards = cards;
		}
		public HashSet<int> Cards {
			get {
				return _cards;
			}
		}
		public override string ToString() {
			return (base.ToString() + ": " + Card.PrintCardSet(_cards));
		}
	}
	public class ChooseAction : Action {
		private int _card;
		public ChooseAction(Player player, int card) {
			_player = player;
			_type = ActionType.ChooseAction;
			_card = card;
		}
		public int Card {
			get {
				return _card;
			}
		}
		public override string ToString() {
			return (base.ToString() + ": " + Model.Card.GetCard(_card));
		}
	}

	public class GoOrStopAction : Action {
		private bool _isGo;
		public GoOrStopAction(Player player, bool isGo) {
			_player = player;
			_type = ActionType.GoOrStopAction;
			_isGo = isGo;
		}
		public bool IsGo {
			get {
				return _isGo;
			}
		}
	}

	public class ShakeAction : Action {
		private HashSet<int> _cards;
		public ShakeAction(Player player, HashSet<int> cards) {
			_player = player;
			_type = ActionType.ShakeAction;
			_cards = cards;
		}
		public HashSet<int> Cards {
			get {
				return _cards;

			}
		}
		public override string ToString() {
			return (base.ToString() + ": " + Card.PrintCardSet(_cards));
		}
	}

	public class BombAction : Action {
		private HashSet<int> _cards;
		public BombAction(Player player, HashSet<int> cards) {
			_player = player;
			_type = ActionType.BombAction;
			_cards = cards;
		}
		public HashSet<int> Cards {
			get {
				return _cards;

			}
		}
		public override string ToString() {
			return (base.ToString() + ": " + Card.PrintCardSet(_cards));
		}
	}

	public class FourCardAction : Action {
		private Month _month;
		public FourCardAction(Player player, Month month) {
			_player = player;
			_type = ActionType.FourCardAction;
			_month = month;
		}
		public Month Month {
			get {
				return _month;
			}
		}
		public override string ToString() {
			return (base.ToString() + ": " + Card.PrintMonth(_month));
		}
	}

	public class EndTurnAction : Action {
		public EndTurnAction(Player player) {
			_player = player;
			_type = ActionType.EndTurnAction;
		}
	}

	public class TakeMoneyAction : Action {
		Player _opponent1;
		Player _opponent2;
		int _money1;
		int _money2;

		public TakeMoneyAction(Player player, Player opponent1, Player opponent2,
														int money1, int money2) {
			_player = player;
			_type = ActionType.TakeMoneyAction;
			_opponent1 = opponent1;
			_opponent2 = opponent2;
			_money1 = money1;
			_money2 = money2;
		}
	}

	public class EndGameAction : Action {
		public EndGameAction(Player player) {
			_player = player;
			_type = ActionType.EndGameAction;
		}
	}
}


