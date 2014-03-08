using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Gostop.Model;

namespace GostopTest
{
	/// <summary>
	/// Summary description for CardTest
	/// </summary>
	[TestClass]
	public class CardTest
	{
		public CardTest()
		{
			//
			// TODO: Add constructor logic here
			//
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
		public void CreateCardsTest()
		{
			Card.CreateCards();
		}

		[TestMethod]
		public void GetCardTest()
		{
			for (int i = 0; i < 48; i++)
			{
				Console.WriteLine(i + "'th card is: " + Card.GetCard(i));
			}
		}

		[TestMethod]
		public void IsSameMonthTest()
		{
			Console.WriteLine("1, 3: " + Card.IsSameMonth(1, 3));
			Console.WriteLine("1, 4: " + Card.IsSameMonth(1, 4));
			Console.WriteLine("4, 5: " + Card.IsSameMonth(4, 5));
		}

		[TestMethod]
		public void SortCardsTest()
		{
			HashSet<int> cards = new HashSet<int>();
			Console.WriteLine("1광, 5초단, 7피, 7피_");
			cards.Add(Card.GetCardId("1광"));
			cards.Add(Card.GetCardId("5초단"));
			cards.Add(Card.GetCardId("7피"));
			cards.Add(Card.GetCardId("7피_"));
			Console.WriteLine(Card.PrintSortedCards(Card.SortCards(cards)));
		}

		[TestMethod]
		public void GetCardSetTest()
		{
			String names = "1광 2고도리 8피 똥쌍피";
			Console.WriteLine("names = " + names);
			Console.WriteLine("results = " + Card.PrintCardSet(Card.GetCardSet(names)));
			
		}
	}
}
