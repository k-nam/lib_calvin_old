using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Gostop.Model
{
	public class Event
	{
		private Player _player;
		public Player Player
		{
			get
			{
				return _player;
			}
		}
	}

	class HitCardEvent: Event
	{
		int _card;
		public int Card
		{
			get
			{
				return _card;
			}
		}
	}

	class FlipCardEvent: Event
	{
		int _card;
		public int Card
		{
			get
			{
				return _card;
			}
		}
	}

	class TakeCardEvent: Event
	{
		int _card;
		public int Card
		{
			get
			{
				return _card;
			}
		}
	}

	// Got more point than 'LastGoPoint': now he can choose Go or Stop
	class FlyingEvent : Event
	{
	}

	class GoEvent: Event
	{
	}

	class StopEvent: Event
	{
	}

	class FourCardEvent: Event
	{
		private Month _month;
		public Month Month
		{
			get
			{
				return _month;
			}
		}
	}

	class ShakeEvent : Event
	{
	}

	class BombEvent : Event
	{
	}

	// _player denotes the winner of the game
	class GameFinishEvent : Event
	{
		private int _playerA_Point;
		private int _playerB_Point;
		private int _playerC_Point;

		public int PlayerA_Point
		{
			get
			{
				return _playerA_Point;
			}
		}
		public int PlayerB_Point
		{
			get
			{
				return _playerB_Point;
			}
		}
		public int PlayerC_Point
		{
			get
			{
				return _playerC_Point;
			}
		}
	}
}
