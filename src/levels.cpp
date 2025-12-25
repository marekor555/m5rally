#include <Arduino.h>
#include <M5Cardputer.h>
#include <M5GFX.h>

#include "car.h"
#include "collision.h"


void runLevel(Car car, const std::vector<Box> &boxes, const std::vector<NGon> &barriers, const std::vector<Line> &lines, const Line &finishLine) {
	while (car.tick(boxes, barriers, lines, finishLine)) {
		M5Cardputer.update();
		M5Canvas canvas(&M5.Lcd);

		car.handbrake = false;
		if (M5Cardputer.Keyboard.isPressed()) {
			const auto state = M5Cardputer.Keyboard.keysState();
			for (const char i : state.word) {
				switch (i) {
					case 'w':
						car.gas();
						break;
					case ',':
						car.steerLeft();
						break;
					case 'a':
						car.backward();
						break;
					case '.':
						car.steerRight();
						break;
					case 'q':
						car.brake();
						break;
					case 'e':
						car.handbrake = true;
						break;
				}
			}
		}

		canvas.createSprite(240, 135);
		canvas.fillScreen(TFT_BLACK);
		canvas.setTextColor(TFT_WHITE, TFT_BLACK);
		canvas.setTextSize(1);

		const float
			camposX = car.posX - M5Cardputer.Lcd.width() / 2,
			camposY = car.posY - M5Cardputer.Lcd.height() / 2;

		car.draw(&canvas, camposX, camposY);
		for (const Box& box : boxes) {
			box.draw(&canvas, camposX, camposY);
		}
		for (const NGon& barrier : barriers) {
			barrier.drawOutline(&canvas, camposX, camposY);
		}
		for (const Line& line : lines) {
			line.drawOutline(&canvas, camposX, camposY);
		}
		finishLine.drawOutline(&canvas, camposX, camposY);

		car.drawUI(&canvas);

		canvas.pushSprite(0, 0);
		canvas.deleteSprite();
		delay(17);  // ~60fps
	}
}


void testLevel() {
	Car car;

	std::vector<Box> boxes;
	std::vector<NGon> barriers;
	std::vector<Line> lines;
	Line finishLine(TFT_GREEN);
	finishLine.points.push_back(Pos2D(40, 160));
	finishLine.points.push_back(Pos2D(-40, 160));

	car.init(-40, -40, 90);

	// test objects
	boxes.push_back(Box().init(0, 0, 25, 25, 45, TFT_DARKGREY));
	boxes.push_back(Box().init(-90, 0, 20, 200, 0, TFT_BROWN));   // left
	boxes.push_back(Box().init(0, -90, 20, 200, 100, TFT_BROWN));  // top

	NGon barrier(TFT_RED);
	barrier.corners.push_back(Pos2D(-200, 200));
	barrier.corners.push_back(Pos2D(0, 300));
	barrier.corners.push_back(Pos2D(200, 200));
	barrier.corners.push_back(Pos2D(200, -200));
	barrier.corners.push_back(Pos2D(-200, -200));
	barriers.push_back(barrier);


	Line line(TFT_RED);
	line.points.push_back(Pos2D(10, 10));
	line.points.push_back(Pos2D(20, 40));
	line.points.push_back(Pos2D(30, 90));
	line.points.push_back(Pos2D(40, 160));
	line.points.push_back(Pos2D(0, 300));
	lines.push_back(line);

	line.points.clear();
	line.points.push_back(Pos2D(-40, 160));
	line.points.push_back(Pos2D(-80, 100));
	lines.push_back(line);

	runLevel(car, boxes, barriers, lines, finishLine);
}

void first() {
	Car car;

	std::vector<Box> boxes;
	std::vector<NGon> barriers;
	const std::vector<Line> lines;
	Line finishLine(TFT_GREEN);
	finishLine.points.push_back(Pos2D(608, 160));
	finishLine.points.push_back(Pos2D(608, 208));

	car.init(55, 970, 0);

	boxes.push_back(Box().init(471, 647, 16, 16, 45, TFT_BROWN));

	NGon barrier(TFT_RED);
	barrier.corners.push_back(Pos2D(32, 991));
	barrier.corners.push_back(Pos2D(79, 991));
	barrier.corners.push_back(Pos2D(70, 844));
	barrier.corners.push_back(Pos2D(86, 799));
	barrier.corners.push_back(Pos2D(128, 735));
	barrier.corners.push_back(Pos2D(256, 737));
	barrier.corners.push_back(Pos2D(255, 1022));
	barrier.corners.push_back(Pos2D(416, 1022));
	barrier.corners.push_back(Pos2D(432, 1007));
	barrier.corners.push_back(Pos2D(447, 943));
	barrier.corners.push_back(Pos2D(495, 801));
	barrier.corners.push_back(Pos2D(511, 626));
	barrier.corners.push_back(Pos2D(496, 547));
	barrier.corners.push_back(Pos2D(496, 495));
	barrier.corners.push_back(Pos2D(508, 464));
	barrier.corners.push_back(Pos2D(558, 440));
	barrier.corners.push_back(Pos2D(895, 440));
	barrier.corners.push_back(Pos2D(942, 413));
	barrier.corners.push_back(Pos2D(976, 207));
	barrier.corners.push_back(Pos2D(976, 160));
	barrier.corners.push_back(Pos2D(606, 160));
	barrier.corners.push_back(Pos2D(606, 208));
	barrier.corners.push_back(Pos2D(933, 208));
	barrier.corners.push_back(Pos2D(933, 279));
	barrier.corners.push_back(Pos2D(908, 378));
	barrier.corners.push_back(Pos2D(892, 401));
	barrier.corners.push_back(Pos2D(558, 400));
	barrier.corners.push_back(Pos2D(496, 431));
	barrier.corners.push_back(Pos2D(464, 463));
	barrier.corners.push_back(Pos2D(449, 494));
	barrier.corners.push_back(Pos2D(449, 546));
	barrier.corners.push_back(Pos2D(432, 626));
	barrier.corners.push_back(Pos2D(447, 786));
	barrier.corners.push_back(Pos2D(383, 960));
	barrier.corners.push_back(Pos2D(368, 975));
	barrier.corners.push_back(Pos2D(304, 975));
	barrier.corners.push_back(Pos2D(303, 698));
	barrier.corners.push_back(Pos2D(127, 699));
	barrier.corners.push_back(Pos2D(95, 719));
	barrier.corners.push_back(Pos2D(47, 783));
	barrier.corners.push_back(Pos2D(32, 845));





	barriers.push_back(barrier);


	runLevel(car, boxes, barriers, lines, finishLine);
}