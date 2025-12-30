#include <Arduino.h>
#include <M5Cardputer.h>
#include <M5GFX.h>

#include "car.h"
#include "collision.h"


void runLevel(Car car, const std::vector<Box> &boxes, const std::vector<NGon> &barriers, const std::vector<Line> &lines,
			const Line &finishLine) {
	while (car.tick(boxes, barriers, lines, finishLine)) {
		M5Cardputer.update();
		M5Canvas canvas(&M5.Lcd);

		car.handbrake = false;
		if (M5Cardputer.Keyboard.isPressed()) {
			const auto state = M5Cardputer.Keyboard.keysState();
			for (const char i: state.word) {
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
		for (const Box &box: boxes) {
			box.draw(&canvas, camposX, camposY);
		}
		for (const NGon &barrier: barriers) {
			barrier.drawOutline(&canvas, camposX, camposY);
		}
		for (const Line &line: lines) {
			line.drawOutline(&canvas, camposX, camposY);
		}
		finishLine.drawOutline(&canvas, camposX, camposY);

		car.drawUI(&canvas);

		canvas.pushSprite(0, 0);
		canvas.deleteSprite();
		delay(17); // ~60fps
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
	boxes.push_back(Box().init(-90, 0, 20, 200, 0, TFT_BROWN)); // left
	boxes.push_back(Box().init(0, -90, 20, 200, 100, TFT_BROWN)); // top

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

void second() {
	Car car;

	std::vector<Box> boxes = {};
	std::vector<NGon> barriers;
	const std::vector<Line> lines;
	Line finishLine(TFT_GREEN);
	finishLine.points = {Pos2D(1875, -1414), Pos2D(1946, -1270)};

	car.init(70, 500, 0);

	NGon barrier(TFT_RED);
	barrier.corners = {
		Pos2D(34, 532), Pos2D(133, 533), Pos2D(135, 461), Pos2D(140, 396),
		Pos2D(158, 295), Pos2D(188, 191), Pos2D(231, 89), Pos2D(307, 84),
		Pos2D(511, 101), Pos2D(665, 176), Pos2D(810, 227), Pos2D(924, 250),
		Pos2D(1088, 252), Pos2D(1170, 217), Pos2D(1248, 155), Pos2D(1331, 85),
		Pos2D(1411, -3), Pos2D(1496, -179), Pos2D(1528, -356), Pos2D(1530, -779),
		Pos2D(1512, -1103), Pos2D(1364, -1495), Pos2D(1164.729, -1633.9453),
		Pos2D(961, -1776), Pos2D(666.9087, -1812.897), Pos2D(419, -1844),
		Pos2D(65, -1814), Pos2D(-279, -1691), Pos2D(-489, -1566),
		Pos2D(-673, -1399), Pos2D(-796, -1231), Pos2D(-898, -1047),
		Pos2D(-1017, -1042), Pos2D(-1128, -1079), Pos2D(-1279, -1203),
		Pos2D(-1426, -1405), Pos2D(-1693, -1676),
		Pos2D(-1839.3262, -1814.2463), Pos2D(-2019, -1984),
		Pos2D(-2134.5454, -2178.5508), Pos2D(-2253, -2378),
		Pos2D(-2290, -2635), Pos2D(-2223, -2934), Pos2D(-2027, -3197),
		Pos2D(-1903.9442, -3317.1826), Pos2D(-1770, -3448),
		Pos2D(-1605.3275, -3612.6724), Pos2D(-1458, -3760),
		Pos2D(-1237.5073, -3838.0208), Pos2D(-938, -3944),
		Pos2D(-702.5055, -4030.019), Pos2D(-437, -4127),
		Pos2D(-293.7776, -4168.3286), Pos2D(-131.82977, -4215.0615),
		Pos2D(150, -4302), Pos2D(389.44812, -4397.776),
		Pos2D(597, -4482), Pos2D(792.3668, -4591.386),
		Pos2D(1006, -4711), Pos2D(1153.4712, -4712.3867),
		Pos2D(1325, -4714), Pos2D(1494.2014, -4681.9746),
		Pos2D(1642, -4654), Pos2D(1792.0427, -4550.663),
		Pos2D(1899, -4477), Pos2D(1844, -4283),
		Pos2D(1739.2458, -4153.9355), Pos2D(1607, -3991),
		Pos2D(1505.9966, -3940.9055), Pos2D(1359, -3868),
		Pos2D(1180, -3782), Pos2D(1009, -3681), Pos2D(850, -3604),
		Pos2D(595, -3496), Pos2D(347, -3416), Pos2D(112, -3359),
		Pos2D(-83, -3182), Pos2D(-191, -2935),
		Pos2D(-133.65906, -2788.3894), Pos2D(-103, -2710),
		Pos2D(56, -2584), Pos2D(220, -2462),
		Pos2D(433, -2517), Pos2D(468, -2490),
		Pos2D(427, -2434), Pos2D(344, -2388),
		Pos2D(347, -2329), Pos2D(386, -2268),
		Pos2D(467, -2151), Pos2D(571.7964, -2074.0771),
		Pos2D(668.68604, -2002.9583), Pos2D(851.1852, -1869.0002),
		Pos2D(952, -1795), Pos2D(1088.1783, -1702.465),
		Pos2D(1274.387, -1575.9338), Pos2D(1445, -1460),
		Pos2D(1573, -1363), Pos2D(1711, -1262),
		Pos2D(1817, -1176), Pos2D(1955, -1277),
		Pos2D(1885, -1418), Pos2D(1721, -1353),
		Pos2D(1604, -1449), Pos2D(1461.4144, -1550.0117),
		Pos2D(1326.0784, -1645.8872), Pos2D(1245, -1708),
		Pos2D(1170.7208, -1763.5006), Pos2D(1052.0549, -1852.1665),
		Pos2D(939.8856, -1936.974), Pos2D(763.7965, -2070.1091),
		Pos2D(633, -2169), Pos2D(515, -2260),
		Pos2D(481, -2338), Pos2D(576, -2433),
		Pos2D(571, -2559), Pos2D(497, -2594),
		Pos2D(383, -2608), Pos2D(274, -2641),
		Pos2D(171, -2722), Pos2D(62, -2827),
		Pos2D(10, -2950), Pos2D(41, -3053),
		Pos2D(147, -3175), Pos2D(372, -3275),
		Pos2D(626, -3385), Pos2D(859, -3465),
		Pos2D(1026, -3546), Pos2D(1195, -3649),
		Pos2D(1416, -3761), Pos2D(1573.9967, -3859.1575),
		Pos2D(1717, -3948), Pos2D(1833.2173, -4083.668),
		Pos2D(1956, -4227), Pos2D(2003, -4443),
		Pos2D(2003, -4595), Pos2D(1927, -4665),
		Pos2D(1820.8894, -4725.801), Pos2D(1653, -4822),
		Pos2D(1535.6702, -4837.091), Pos2D(1342, -4862),
		Pos2D(1156.1086, -4852.8335),
		Pos2D(973.2165, -4843.8154), Pos2D(719, -4669),
		Pos2D(516.3352, -4571.2144), Pos2D(319, -4476),
		Pos2D(130.9552, -4411.624), Pos2D(-14, -4362),
		Pos2D(-132.18945, -4324.3076), Pos2D(-368.00037, -4249.105),
		Pos2D(-635.37256, -4163.837), Pos2D(-823, -4104),
		Pos2D(-985.1123, -4047.762), Pos2D(-1146.745, -3991.69),
		Pos2D(-1317.4955, -3932.4548), Pos2D(-1535, -3857),
		Pos2D(-1694.4485, -3704.7556), Pos2D(-1867, -3540),
		Pos2D(-2038.6335, -3391.4282), Pos2D(-2254, -3205),
		Pos2D(-2360, -2951), Pos2D(-2397, -2720),
		Pos2D(-2407, -2537), Pos2D(-2377, -2403),
		Pos2D(-2286.5469, -2222.0938), Pos2D(-2202, -2053),
		Pos2D(-2088, -1902), Pos2D(-1960.6287, -1776.8298),
		Pos2D(-1852.3394, -1670.4126), Pos2D(-1683, -1504),
		Pos2D(-1459, -1243), Pos2D(-1267, -1064),
		Pos2D(-1129, -972), Pos2D(-982, -926),
		Pos2D(-874, -945), Pos2D(-753, -1149),
		Pos2D(-624, -1301), Pos2D(-504, -1406),
		Pos2D(-364, -1530), Pos2D(-167, -1633),
		Pos2D(-11, -1695), Pos2D(120, -1741),
		Pos2D(273, -1757), Pos2D(688, -1712),
		Pos2D(926, -1642), Pos2D(1081.7793, -1538.623),
		Pos2D(1253, -1425), Pos2D(1376, -1162),
		Pos2D(1410, -769), Pos2D(1405, -404),
		Pos2D(1377, -219), Pos2D(1315, -83),
		Pos2D(1237, -1), Pos2D(1113, 83),
		Pos2D(975, 112), Pos2D(822, 95),
		Pos2D(654, 41), Pos2D(487, 25),
		Pos2D(223, 8), Pos2D(126, 79),
		Pos2D(84, 181), Pos2D(68, 273),
		Pos2D(43, 443), Pos2D(34, 532)
	};
	barriers.push_back(barrier);

	runLevel(car, boxes, barriers, lines, finishLine);
}
