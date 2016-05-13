function displayHistory(history) {
	for (var i = 0; i < history.length; i++) {

	}
}

function GameStatus(otherTaken1, otherTaken2, playerTaken, floorCards, handCards,
		otherTakenScreen1, otherTakenScreen2, playerTakenScreen, floorCardsScreen, handCardsScreen) {
	this.otherTaken1 = otherTaken1;
	this.otherTaken2 = otherTaken2;
	this.playerTaken = playerTaken;
	this.floorCards = floorCards;
	this.handCards = handCards;
	this.otherTakenScreen1 = otherTakenScreen1;
	this.otherTakenScreen2 = otherTakenScreen2;
	this.playerTakenScreen = playerTakenScreen;
	this.floorCardsScreen = floorCardsScreen;
	this.handCardsScreen = handCardsScreen;

	function draw() {
		this.otherTakenScreen1.innerHTML = '';
		this.otherTakenScreen2.innerHTML = '';
		this.playerTakenScreen.innerHTML = '';
		this.floorCardsScreen.innerHTML = '';
		this.handCardsScreen.innerHTML = '';
		drawTakenCards(this.otherTaken1, this.otherTakenScreen1);
		drawTakenCards(this.otherTaken2, this.otherTakenScreen2);
		drawTakenCards(this.playerTaken, this.playerTakenScreen);
		drawFloorCards(this.floorCards, this.floorCardsScreen);
		drawHandCards(this.handCards, this.handCardsScreen);
	}

	function processHitAction(card) {

	}
	function processFlipHitAction(card) {

	}
	function processStealAction(card) {

	}
}