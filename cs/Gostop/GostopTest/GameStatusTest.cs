using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Gostop.Model;

namespace GostopTest
{
	/// <summary>
	/// Summary description for GameStatusTest
	/// </summary>
	[TestClass]
	public class GameStatusTest
	{
		private HashSet<int> playerA_HandCards;
		private HashSet<int> playerB_HandCards;
		private HashSet<int> playerC_HandCards;
		private HashSet<int> floorCards;
		private HashSet<int> hiddenCards;

		public GameStatusTest()
		{
			playerA_HandCards = new HashSet<int>();
			playerB_HandCards = new HashSet<int>();
			playerC_HandCards = new HashSet<int>();
			floorCards = new HashSet<int>();
			hiddenCards = new HashSet<int>();		
		}

		private TestContext testContextInstance;

		/// <summary>
		///Gets or sets the test context which provides
		///information about and functionality for the current test run.
		///</summary>
		public TestContext TestContext
		{
			get
			{
				return testContextInstance;
			}
			set
			{
				testContextInstance = value;
			}
		}

		#region Additional test attributes
		//
		// You can use the following additional attributes as you write your tests:
		//
		// Use ClassInitialize to run code before running the first test in the class
		// [ClassInitialize()]
		// public static void MyClassInitialize(TestContext testContext) { }
		//
		// Use ClassCleanup to run code after all tests in a class have run
		// [ClassCleanup()]
		// public static void MyClassCleanup() { }
		//
		// Use TestInitialize to run code before running each test 
		// [TestInitialize()]
		// public void MyTestInitialize() { }
		//
		// Use TestCleanup to run code after each test has run
		// [TestCleanup()]
		// public void MyTestCleanup() { }
		//
		#endregion

		[TestMethod]
		public void ShuffleCardsTest()
		{
			GameStatus.ShuffleCards(playerA_HandCards, playerB_HandCards,
				playerC_HandCards, floorCards, hiddenCards);
		}

		[TestMethod]
		public void ToStringTest()
		{
			GameStatus initialStatus = new GameStatus();
			Console.WriteLine(initialStatus);
		}

	}
}
