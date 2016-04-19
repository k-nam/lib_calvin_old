using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using Gostop.Model;

namespace Gostop {
	static class Program {
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() {
			//Application.EnableVisualStyles();
			//Application.SetCompatibleTextRenderingDefault(false);
			//Application.Run(new Form1());

			GostopEngine engine = new GostopEngine();
			var gameStep = engine.Start();
			while (true) {
				Console.WriteLine();
				Console.WriteLine(gameStep.FinalStatus);
				// display history
				Console.WriteLine("History:");
				for (int i = 0; i < gameStep.History.Count; i++) {
					Console.WriteLine(i + ": " + gameStep.History[i].ToString());
				}
				Console.WriteLine();


				// display choices
				Console.WriteLine("Choices:");
				for (int i = 0; i < gameStep.Choices.Count; i++) {
					Console.WriteLine(i + ": " + gameStep.Choices[i].ToString());
				}
				Console.WriteLine("Current player: " + gameStep.FinalStatus.CurrentPlayer);
				// get input
				int choice = int.Parse(Console.ReadLine());
				gameStep = engine.ProceedWith(choice);

			}
		}
	}
}
