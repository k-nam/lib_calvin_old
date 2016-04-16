using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Gostop.Model;
namespace GostopTest
{
	[TestClass]
	public class ServerMachineTest
	{
		#region // GetShellsToBeStolenTests
		[TestMethod]
		public void GetShellsToBeStolenTest()
		{
			HashSet<int> shells = new HashSet<int>();
			shells.Add(Card.GetCardId("1피"));
			shells.Add(Card.GetCardId("1피*"));
			Console.WriteLine("1피, 1피* 에서 1개 추출 ");
			HashSet<int> shellsToBeStolen = 
				ServerMachine.GetShellsToBeStolen(shells, 1);
			Console.WriteLine(Card.PrintCardSet(shellsToBeStolen));
		}

		[TestMethod]
		public void GetShellsToBeStolenTest2()
		{
			HashSet<int> shells = new HashSet<int>();
			shells.Add(Card.GetCardId("1피"));
			shells.Add(Card.GetCardId("1피*"));
			Console.WriteLine("1피, 1피* 에서 2개 추출 ");
			HashSet<int> shellsToBeStolen =
				ServerMachine.GetShellsToBeStolen(shells, 2);
			Console.WriteLine(Card.PrintCardSet(shellsToBeStolen));
		}

		[TestMethod]
		public void GetShellsToBeStolenTest3()
		{
			HashSet<int> shells = new HashSet<int>();
			shells.Add(Card.GetCardId("1피"));
			shells.Add(Card.GetCardId("1피*"));
			Console.WriteLine("1피, 1피* 에서 3개 추출 ");
			HashSet<int> shellsToBeStolen =
				ServerMachine.GetShellsToBeStolen(shells, 3);
			Console.WriteLine(Card.PrintCardSet(shellsToBeStolen));
		}

		[TestMethod]
		public void GetShellsToBeStolenTest4()
		{
			HashSet<int> shells = new HashSet<int>();
			shells.Add(Card.GetCardId("1피"));
			shells.Add(Card.GetCardId("똥쌍피"));
			Console.WriteLine("1피, 똥쌍피 에서 1개 추출 ");
			HashSet<int> shellsToBeStolen =
				ServerMachine.GetShellsToBeStolen(shells, 1);
			Console.WriteLine(Card.PrintCardSet(shellsToBeStolen));
		}

		[TestMethod]
		public void GetShellsToBeStolenTest5()
		{
			HashSet<int> shells = new HashSet<int>();
			shells.Add(Card.GetCardId("1피"));
			shells.Add(Card.GetCardId("똥쌍피"));
			Console.WriteLine("1피, 똥쌍피 에서 2개 추출 ");
			HashSet<int> shellsToBeStolen =
				ServerMachine.GetShellsToBeStolen(shells, 2);
			Console.WriteLine(Card.PrintCardSet(shellsToBeStolen));
		}

		[TestMethod]
		public void GetShellsToBeStolenTest6()
		{
			HashSet<int> shells = new HashSet<int>();
			shells.Add(Card.GetCardId("1피"));
			shells.Add(Card.GetCardId("똥쌍피"));
			shells.Add(Card.GetCardId("7피"));
			Console.WriteLine("1피, 똥쌍피, 7피 에서 2개 추출 ");
			HashSet<int> shellsToBeStolen =
				ServerMachine.GetShellsToBeStolen(shells, 2);
			Console.WriteLine(Card.PrintCardSet(shellsToBeStolen));
		}

		[TestMethod]
		public void GetShellsToBeStolenTest7()
		{
			HashSet<int> shells = new HashSet<int>();
			shells.Add(Card.GetCardId("1피"));
			shells.Add(Card.GetCardId("똥쌍피"));
			shells.Add(Card.GetCardId("7피"));
			Console.WriteLine("1피, 똥쌍피, 7피 에서 3개 추출 ");
			HashSet<int> shellsToBeStolen =
				ServerMachine.GetShellsToBeStolen(shells, 3);
			Console.WriteLine(Card.PrintCardSet(shellsToBeStolen));
		}
		#endregion

		[TestMethod]
		public void GetNextTurnActionsTest()
		{
			GameStatus randomStatus = new GameStatus();
			Console.WriteLine(randomStatus);
			List<Gostop.Model.Action> actionList = new List<Gostop.Model.Action>();
			ServerMachine.GetNewTurnActions(randomStatus, actionList);
			Console.WriteLine(Gostop.Model.Action.PrintActionList(actionList));
		}

		[TestMethod]
		public void GetNextTurnActionsTest2()   
		{
			HashSet<int> playerA_HandCards = Card.GetCardSet(
				"1광 1홍단 1피 1피* 2고도리 2홍단 2피");
			HashSet<int> playerB_HandCards = Card.GetCardSet("3피");
			HashSet<int> playerC_HandCards = Card.GetCardSet("");
			HashSet<int> floorCards = Card.GetCardSet("3피*");
			GameStatus status = new GameStatus(playerA_HandCards, playerB_HandCards,
				playerC_HandCards, floorCards);
			Console.WriteLine(status);
			List<Gostop.Model.Action> actionList = new List<Gostop.Model.Action>();
			ServerMachine.GetNewTurnActions(status, actionList);
			Console.WriteLine(Gostop.Model.Action.PrintActionList(actionList));
		}

		[TestMethod]
		// hit + miss
            // ddd
		public void ManageFloorCardsTest()
		{
			HashSet<int> floorCards = Card.GetCardSet(
				"1광 2피 3홍단 4초단 7열끗 8고도리");
			HashSet<int> cardsToBeTaken = new HashSet<int>();
			int hitCard = Card.GetCardId("1피");
			int flippedCard = Card.GetCardId("똥피");
			int stealPoint = 0;
			int fuckPoint = 0;
			int firstChoice = -1;
			int secondChoice = -1;
			Console.WriteLine("FloorCards before: " + Card.PrintCardSet(floorCards));
			Console.WriteLine("HitCard: " + Card.PrintCard(hitCard));
			Console.WriteLine("FlippedCard: " + Card.PrintCard(flippedCard));
			ServerMachine.ManageFloorCards(floorCards, hitCard, flippedCard,
				cardsToBeTaken, ref stealPoint, ref fuckPoint, firstChoice, secondChoice);						
			Console.WriteLine("FloorCards after: " + Card.PrintCardSet(floorCards));
			Console.WriteLine("TakenCards: " + Card.PrintCardSet(cardsToBeTaken));
			Console.WriteLine("StealPoint: " + stealPoint);
			Console.WriteLine("FuckPoint: " + fuckPoint);	
		}

		[TestMethod]
		// choice + hit
		public void ManageFloorCardsTest2()
		{
			HashSet<int> floorCards = Card.GetCardSet(
				"1광 1피* 3홍단 4초단 8피 8고도리");
			HashSet<int> cardsToBeTaken = new HashSet<int>();
			int hitCard = Card.GetCardId("1피");
			int flippedCard = Card.GetCardId("8피*");
			int stealPoint = 0;
			int fuckPoint = 0;
			int firstChoice = Card.GetCardId("1광");
			int secondChoice = Card.GetCardId("8고도리");
			Console.WriteLine("FloorCards before: " + Card.PrintCardSet(floorCards));
			Console.WriteLine("HitCard: " + Card.PrintCard(hitCard));
			Console.WriteLine("FlippedCard: " + Card.PrintCard(flippedCard));
			Console.WriteLine("FirstChoice: " + Card.PrintCard(firstChoice));
			Console.WriteLine("SecondChoice: " + Card.PrintCard(secondChoice));
			ServerMachine.ManageFloorCards(floorCards, hitCard, flippedCard,
				cardsToBeTaken, ref stealPoint, ref fuckPoint, firstChoice, secondChoice);
			Console.WriteLine("FloorCards after: " + Card.PrintCardSet(floorCards));
			Console.WriteLine("TakenCards: " + Card.PrintCardSet(cardsToBeTaken));
			Console.WriteLine("StealPoint: " + stealPoint);
			Console.WriteLine("FuckPoint: " + fuckPoint);
		}

		[TestMethod]
		// resolve-fuck + hit
		public void ManageFloorCardsTest3()
		{
			HashSet<int> floorCards = Card.GetCardSet(
				"1광 1피 1피* 4초단 7열끗 8고도리");
			HashSet<int> cardsToBeTaken = new HashSet<int>();
			int hitCard = Card.GetCardId("1홍단");
			int flippedCard = Card.GetCardId("7피");
			int stealPoint = 0;
			int fuckPoint = 0;
			int firstChoice = -1;
			int secondChoice = -1;
			Console.WriteLine("FloorCards before: " + Card.PrintCardSet(floorCards));
			Console.WriteLine("HitCard: " + Card.PrintCard(hitCard));
			Console.WriteLine("FlippedCard: " + Card.PrintCard(flippedCard));
			ServerMachine.ManageFloorCards(floorCards, hitCard, flippedCard,
				cardsToBeTaken, ref stealPoint, ref fuckPoint, firstChoice, secondChoice);
			Console.WriteLine("FloorCards after: " + Card.PrintCardSet(floorCards));
			Console.WriteLine("TakenCards: " + Card.PrintCardSet(cardsToBeTaken));
			Console.WriteLine("StealPoint: " + stealPoint);
			Console.WriteLine("FuckPoint: " + fuckPoint);
		}

		[TestMethod]
		// fuck
		public void ManageFloorCardsTest4()
		{
			HashSet<int> floorCards = Card.GetCardSet(
				"1광 2피 2피* 4초단 7열끗 8고도리");
			HashSet<int> cardsToBeTaken = new HashSet<int>();
			int hitCard = Card.GetCardId("1홍단");
			int flippedCard = Card.GetCardId("1피");
			int stealPoint = 0;
			int fuckPoint = 0;
			int firstChoice = -1;
			int secondChoice = -1;
			Console.WriteLine("FloorCards before: " + Card.PrintCardSet(floorCards));
			Console.WriteLine("HitCard: " + Card.PrintCard(hitCard));
			Console.WriteLine("FlippedCard: " + Card.PrintCard(flippedCard));
			ServerMachine.ManageFloorCards(floorCards, hitCard, flippedCard,
				cardsToBeTaken, ref stealPoint, ref fuckPoint, firstChoice, secondChoice);
			Console.WriteLine("FloorCards after: " + Card.PrintCardSet(floorCards));
			Console.WriteLine("TakenCards: " + Card.PrintCardSet(cardsToBeTaken));
			Console.WriteLine("StealPoint: " + stealPoint);
			Console.WriteLine("FuckPoint: " + fuckPoint);
		}

		[TestMethod]
		// kiss
		public void ManageFloorCardsTest5()
		{
			HashSet<int> floorCards = Card.GetCardSet(
				"1광 2피 2피* 4초단 7열끗 8고도리");
			HashSet<int> cardsToBeTaken = new HashSet<int>();
			int hitCard = Card.GetCardId("똥광");
			int flippedCard = Card.GetCardId("똥쌍피");
			int stealPoint = 0;
			int fuckPoint = 0;
			int firstChoice = -1;
			int secondChoice = -1;
			Console.WriteLine("FloorCards before: " + Card.PrintCardSet(floorCards));
			Console.WriteLine("HitCard: " + Card.PrintCard(hitCard));
			Console.WriteLine("FlippedCard: " + Card.PrintCard(flippedCard));
			ServerMachine.ManageFloorCards(floorCards, hitCard, flippedCard,
				cardsToBeTaken, ref stealPoint, ref fuckPoint, firstChoice, secondChoice);
			Console.WriteLine("FloorCards after: " + Card.PrintCardSet(floorCards));
			Console.WriteLine("TakenCards: " + Card.PrintCardSet(cardsToBeTaken));
			Console.WriteLine("StealPoint: " + stealPoint);
			Console.WriteLine("FuckPoint: " + fuckPoint);
		}

		[TestMethod]
		// dadak
		public void ManageFloorCardsTest6()
		{
			HashSet<int> floorCards = Card.GetCardSet(
				"1광 2피 2피* 4초단 7열끗 8고도리");
			HashSet<int> cardsToBeTaken = new HashSet<int>();
			int hitCard = Card.GetCardId("2홍단");
			int flippedCard = Card.GetCardId("2고도리");
			int stealPoint = 0;
			int fuckPoint = 0;
			int firstChoice = -1;
			int secondChoice = -1;
			Console.WriteLine("FloorCards before: " + Card.PrintCardSet(floorCards));
			Console.WriteLine("HitCard: " + Card.PrintCard(hitCard));
			Console.WriteLine("FlippedCard: " + Card.PrintCard(flippedCard));
			ServerMachine.ManageFloorCards(floorCards, hitCard, flippedCard,
				cardsToBeTaken, ref stealPoint, ref fuckPoint, firstChoice, secondChoice);
			Console.WriteLine("FloorCards after: " + Card.PrintCardSet(floorCards));
			Console.WriteLine("TakenCards: " + Card.PrintCardSet(cardsToBeTaken));
			Console.WriteLine("StealPoint: " + stealPoint);
			Console.WriteLine("FuckPoint: " + fuckPoint);
		}
	}
}
