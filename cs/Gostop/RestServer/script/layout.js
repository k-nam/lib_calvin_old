var Layout = {};

Layout.interval = 32;

Layout.cardWidth = 54;
Layout.cardHeight = 81;

Layout.handCardWidth =	70;
Layout.handCardHeight = 105;
Layout.handCardMargin = 3;

Layout.lightStartPoint = [5, 100];
Layout.lightEndPoint = [50, 100];

Layout.tenStartPoint = [115, 5];
Layout.tenEndPoint = [260, 5];

Layout.fiveStartPoint = [115, 100];
Layout.fiveEndPoint = [260, 102];

Layout.shellStartPoint1 = [330, 100];
Layout.shellEndPoint1 = [440, 102];

Layout.shellStartPoint2 = [330, 60];
Layout.shellEndPoint2 = [440, 60];

Layout.shellStartPoint3 = [330, 5];
Layout.shellEndPoint3 = [440, 5];

Layout.floorSlot = [[-150, 110], [-50, 110], [50, 110], [150, 110],
									[-200, 0], [-100, 0], [100, 0], [200, 0],
									[-150, -110], [-50, -110], [50, -110], [150, -110]];

Layout.floorSlot2 = [Layout.floorSlot[5], Layout.floorSlot[6], Layout.floorSlot[1], Layout.floorSlot[10],
								Layout.floorSlot[2], Layout.floorSlot[9], Layout.floorSlot[0], Layout.floorSlot[3],
								Layout.floorSlot[4], Layout.floorSlot[7], Layout.floorSlot[8], Layout.floorSlot[11]];

// i'th element contains floor cards at i'th slot
Layout.floorCards = new Array(12);

Layout.getCoordinates = function(startPoint, endPoint, interval, numCards) {
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

Layout.drawAtCoordinates = function(cards, coordinates, screen) {
	for (var i = 0; i < cards.length; i++) {
		Layout.drawAtCoordinate(cards[i], coordinates[i], screen, Layout.cardWidth, Layout.cardHeight);
	}
}

Layout.drawAtCoordinate = function (card, coordinate, screen, width, height) {
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

	img.onmouseenter = function() {
		img.style.left = (coordinate[0]) + (-0) + 'px';
		img.style['z-index'] = '1';
	}
	img.onmouseout = function () {
		img.style.left = coordinate[0] + 'px';
		img.style['z-index'] = '0';
	}
	return img;	
}


Layout.drawTakenCards = function(cards, screen) {
	var splitedCards = Card.splitCardsForDisplay(cards);
	var lights = splitedCards['광'];
	var tens = splitedCards['열끗'];
	var fives = splitedCards['다섯끗'];
	var shells = splitedCards['피'];
	var shellByLines = Card.splitShellsForDisplay(shells);
	//alert(shellByLines[0]);

	var lightCoordinates = Layout.getCoordinates(Layout.lightStartPoint, Layout.lightEndPoint,
		Layout.interval, lights.length);
	var tenCoordinates = Layout.getCoordinates(Layout.tenStartPoint, Layout.tenEndPoint,
		Layout.interval, tens.length);
	var fiveCoordinates = Layout.getCoordinates(Layout.fiveStartPoint, Layout.fiveEndPoint,
		Layout.interval, fives.length);

	Layout.drawAtCoordinates(lights, lightCoordinates, screen);
	Layout.drawAtCoordinates(tens, tenCoordinates, screen);
	Layout.drawAtCoordinates(fives, fiveCoordinates, screen);

	var shellCoordinates1 = Layout.getCoordinates(Layout.shellStartPoint1, Layout.shellEndPoint1,
		Layout.interval, shellByLines[0].length);
	var shellCoordinates2 = Layout.getCoordinates(Layout.shellStartPoint2, Layout.shellEndPoint2,
		Layout.interval, shellByLines[1].length);
	var shellCoordinates3 = Layout.getCoordinates(Layout.shellStartPoint3, Layout.shellEndPoint3,
		Layout.interval, shellByLines[2].length);

	Layout.drawAtCoordinates(shellByLines[0], shellCoordinates1, screen);
	Layout.drawAtCoordinates(shellByLines[1], shellCoordinates2, screen);
	Layout.drawAtCoordinates(shellByLines[2], shellCoordinates3, screen);
}

Layout.drawFloorCards = function(cards, screen) {
	// draw hidden card deck
	var hidden = document.createElement('img');
	screen.appendChild(hidden);
	hidden.src = '/image/back.png';
	hidden.style.width = Layout.cardWidth + 'px';
	hidden.style.height = Layout.cardHeight + 'px';
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
		adjustedCoordinate[0] += Layout.floorSlot2[floorSlotIndex][0] - 0;
		adjustedCoordinate[1] += Layout.floorSlot2[floorSlotIndex][1] - 0;
		for (var i = 0; i < thisMonthCards.length; i++) {
			var img = Layout.drawAtCoordinate(
				thisMonthCards[i], [adjustedCoordinate[0], adjustedCoordinate[1]], screen,
				Layout.cardWidth, Layout.cardHeight);
			//img.style['z-value'] = '' + (-i);
			adjustedCoordinate[0] += 10;
			adjustedCoordinate[1] += 10;
			if (Layout.floorCards[floorSlotIndex]) {
				Layout.floorCards[floorSlotIndex].push(thisMonthCards[i]);
			} else {
				Layout.floorCards[floorSlotIndex] = [thisMonthCards[i]];
			}
		}
		floorSlotIndex++;
	}
}

Layout.insertToFloorCards = function(card, floorCards) {
	var month = Card.getCard(card).month();
	if (floorCards[month]) {
		
		floorCards[month].push(card);
	} else {
		floorCards[month] = [card];
	}
}

Layout.removeFromFloorCards = function(card, floorCards) {
	var month = Card.getCard(card).month();
	if (floorCards[month]) {
		floorCards[month] = floorCards[month].filter(function(x) { return x != card; });
	} else {
		alert('removeFromFloorCards error');
	}
}

Layout.drawHandCards = function(cards, screen) {	
	for (var i = 0; i < cards.length; i++) {		
		if (i < 4) { // upper line
			var left = i * (Layout.handCardWidth + Layout.handCardMargin);
			var top = Layout.handCardMargin;
		} else { // lower line
			var left = (i - 4) * (Layout.handCardWidth + Layout.handCardMargin);
			var top = Layout.handCardHeight + 2 * Layout.handCardMargin;
		}
		
		Layout.drawAtCoordinate(cards[i], [left, top], screen,
			Layout.handCardWidth, Layout.handCardHeight);
	}
}
