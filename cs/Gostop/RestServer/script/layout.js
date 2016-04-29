﻿var interval = 32;

var cardWidth = 54;
var cardHeight = 81;

var handCardWidth =	70;
var handCardHeight = 105;
var handCardMargin = 3;

var lightStartPoint = [5, 100];
var lightEndPoint = [50, 100];

var tenStartPoint = [115, 5];
var tenEndPoint = [260, 5];

var fiveStartPoint = [115, 100];
var fiveEndPoint = [260, 102];

var shellStartPoint1 = [330, 100];
var shellEndPoint1 = [440, 102];

var shellStartPoint2 = [330, 60];
var shellEndPoint2 = [440, 60];

var shellStartPoint3 = [330, 5];
var shellEndPoint3 = [440, 5];

var floorSlot = [[-150, 110], [-50, 110], [50, 110], [150, 110],
									[-200, 0], [-100, 0], [100, 0], [200, 0],
									[-150, -110], [-50, -110], [50, -110], [150, -110]];

var floorSlot2 = [floorSlot[5], floorSlot[6], floorSlot[1], floorSlot[10],
								floorSlot[2], floorSlot[9], floorSlot[0], floorSlot[3],
								floorSlot[4], floorSlot[7], floorSlot[8], floorSlot[11]];

// i'th element contains floor cards at i'th slot
var floorCards = new Array(12);

function O(i) {
	if (typeof i == 'object') {
		return i;
	} else {
		return document.getElementById(i);
	}
}

function getCoordinates(startPoint, endPoint, interval, numCards) {

	var result = [];
	if (startPoint[0] + interval * (numCards - 1) < endPoint[0]) {
	} else {
		interval = (endPoint[0] - startPoint[0]) / (numCards - 1);
	}
	for (var i = 0; i < numCards; i++) {
		result.push([startPoint[0] + interval * i, startPoint[1]]);
	}

	return result;
}

function drawAtCoordinates(cards, coordinates, screen) {

	for (var i = 0; i < cards.length; i++) {
		drawAtCoordinate(cards[i], coordinates[i], screen);
	}
}

function drawAtCoordinate(card, coordinate, screen, width = cardWidth, height = cardHeight) {
	var img = document.createElement('img');
	screen.appendChild(img);
	img.src = '/image/' + card + '.png';
	//alert(coordinate); 
	img.id = 'card-' + card;
	img.style.position = 'absolute';
	img.style.width = width + 'px';
	img.style.height = height + 'px';
	img.style.left = coordinate[0] + 'px';
	img.style.top = coordinate[1] + 'px';


	img.onmouseenter = function () {
		img.style.left = (coordinate[0]) + (-0) + 'px';
		img.style['z-index'] = '1';
	}
	img.onmouseout = function () {
		img.style.left = coordinate[0] + 'px';
		img.style['z-index'] = '0';
	}
	return img;
	
}

function drawTakenCards(cards, screen) {

	var splitedCards = splitCardsForDisplay(cards);

	var lights = splitedCards['광'];
	var tens = splitedCards['열끗'];
	var fives = splitedCards['다섯끗'];
	var shells = splitedCards['피'];
	var shellByLines = splitShellsForDisplay(shells);
	//alert(shellByLines[0]);

	var lightCoordinates = getCoordinates(lightStartPoint, lightEndPoint, interval, lights.length);
	var tenCoordinates = getCoordinates(tenStartPoint, tenEndPoint, interval, tens.length);
	var fiveCoordinates = getCoordinates(fiveStartPoint, fiveEndPoint, interval, fives.length);

	drawAtCoordinates(lights, lightCoordinates, screen);
	drawAtCoordinates(tens, tenCoordinates, screen);
	drawAtCoordinates(fives, fiveCoordinates, screen);

	var shellCoordinates1 =
		getCoordinates(shellStartPoint1, shellEndPoint1, interval, shellByLines[0].length);
	var shellCoordinates2 =
		getCoordinates(shellStartPoint2, shellEndPoint2, interval, shellByLines[1].length);
	var shellCoordinates3 =
		getCoordinates(shellStartPoint3, shellEndPoint3, interval, shellByLines[2].length);

	drawAtCoordinates(shellByLines[0], shellCoordinates1, screen);
	drawAtCoordinates(shellByLines[1], shellCoordinates2, screen);
	drawAtCoordinates(shellByLines[2], shellCoordinates3, screen);
}

function drawFloorCards(cards, screen) {
	// draw hidden card deck
	var hidden = document.createElement('img');
	screen.appendChild(hidden);
	hidden.src = '/image/back.png';
	hidden.style.width = cardWidth + 'px';
	hidden.style.height = cardHeight + 'px';
	hidden.style.position = 'absolute';

	var screenWidth = screen.offsetWidth;
	var screenHeight = screen.offsetHeight;

	var hiddenLeft = (screenWidth / 2 - hidden.width / 2);
	var hiddenTop = (screenHeight / 2 - hidden.height / 2);
	hidden.style.left = hiddenLeft + 'px';
	hidden.style.top = hiddenTop + 'px';

	// draw visible cards
	var floorSlotIndex = 0;
	for (var month in cards) {
		var thisMonthCards = cards[month];
		//alert(thisMonthCards.length);
		var adjustedCoordinate = [hiddenLeft, hiddenTop];
		adjustedCoordinate[0] += floorSlot2[floorSlotIndex][0] - 0;
		adjustedCoordinate[1] += floorSlot2[floorSlotIndex][1] - 0;
		for (var i = 0; i < thisMonthCards.length; i++) {
			var img = drawAtCoordinate(thisMonthCards[i], [adjustedCoordinate[0], adjustedCoordinate[1]], screen);
			//img.style['z-value'] = '' + (-i);
			adjustedCoordinate[0] += 10;
			adjustedCoordinate[1] += 10;
			if (floorCards[floorSlotIndex]) {
				floorCards[floorSlotIndex].push(thisMonthCards[i]);
			} else {
				floorCards[floorSlotIndex] = [thisMonthCards[i]];
			}
		}
		floorSlotIndex++;
	}
}

function insertToFloorCards(card, floorCards) {
	var month = getCard(card).month();
	if (floorCards[month]) {
		
		floorCards[month].push(card);
	} else {
		floorCards[month] = [card];
	}
}

function removeFromFloorCards(card, floorCards) {
	var month = getCard(card).month();
	if (floorCards[month]) {
		floorCards[month] = floorCards[month].filter(function (x) { return x != card; });
	} else {
		alert('removeFromFloorCards error');
	}
}

function drawHandCard(cards, screen) {
	for (var i = 0; i < cards.length; i++) {
		if (i < 4) { // upper line
			var left = i * (handCardWidth + handCardMargin);
			var top = handCardMargin;
		} else { // lower line
			var left = (i - 4) * (handCardWidth + handCardMargin);
			var top = handCardHeight + 2 * handCardMargin;
		}
		drawAtCoordinate(cards[i], [left, top], screen, handCardWidth, handCardHeight);
	}
}