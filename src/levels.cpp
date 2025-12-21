#include <Arduino.h>
#include <M5Cardputer.h>
#include <M5GFX.h>

#include "car.h"
#include "collision.h"

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