#include <Arduino.h>
#include <M5Cardputer.h>
#include <M5GFX.h>

#include "car.h"
#include "collision.h"

#define GROUND_COLOR M5GFX::color565(10, 50, 0)
#define BARRIER_COLOR M5GFX::color565(255, 115, 0)
#define OBJ_COLOR M5GFX::color565(82, 48, 0)
#define FINISH_COLOR M5GFX::color565(149, 255, 0)

#define FRAME_TIME_MS 10 // 16ms -> abt. 60 fps


void runLevel(Car car, const std::vector<Box> &boxes, const std::vector<NGon> &barriers, const std::vector<Line> &lines,
			const Line &finishLine) {
	unsigned long frameStart = millis();
	while (true) {
		const unsigned long now = millis();
		float delta = static_cast<float>(now - frameStart) / 1000;
		if (delta < 0.001f) delta = 0.001f;
		frameStart = now;

		if (!car.tick(boxes, barriers, lines, finishLine, delta)) break;

		M5Cardputer.update();
		M5Canvas canvas(&M5.Lcd);

		car.handbrake = false;
		if (M5Cardputer.Keyboard.isPressed()) {
			const auto state = M5Cardputer.Keyboard.keysState();
			for (const char i: state.word) {
				switch (i) {
					case 'w':
						car.gas(delta);
						break;
					case ',':
						car.steerLeft(delta);
						break;
					case 'a':
						car.backward(delta);
						break;
					case '.':
						car.steerRight(delta);
						break;
					case 'q':
						car.brake(delta);
						break;
					case 'e':
						car.handbrake = true;
						break;
				}
			}
		}

		canvas.createSprite(240, 135);
		canvas.fillScreen(GROUND_COLOR);
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

		car.drawUI(&canvas, delta);

		canvas.pushSprite(0, 0);
		canvas.deleteSprite();

		const unsigned long end = millis();
		if (end - now < FRAME_TIME_MS) {
			delay(FRAME_TIME_MS - (end - now));
		}
	}

	M5Cardputer.Display.fillScreen(TFT_BLACK);
	M5Cardputer.Display.setTextSize(2);
	M5Cardputer.Display.setTextColor(TFT_WHITE);
	M5Cardputer.Display.drawCenterString("TIME: " + String(car.runTime, 3),
										M5Cardputer.Display.width() / 2, M5Cardputer.Display.height() / 2);
	while (M5Cardputer.Keyboard.isPressed()) M5Cardputer.update();
	while (!M5Cardputer.Keyboard.isPressed()) M5Cardputer.update();
}


void testLevel() {
	Car car;

	std::vector<Box> boxes;
	std::vector<NGon> barriers;
	std::vector<Line> lines;
	Line finishLine(FINISH_COLOR);
	finishLine.points.push_back(Pos2D(40, 160));
	finishLine.points.push_back(Pos2D(-40, 160));

	car.init(-40, -40, 90);

	// test objects
	boxes.push_back(Box().init(0, 0, 25, 25, 45, OBJ_COLOR));
	boxes.push_back(Box().init(-90, 0, 20, 200, 0, OBJ_COLOR)); // left
	boxes.push_back(Box().init(0, -90, 20, 200, 100, OBJ_COLOR)); // top

	NGon barrier(BARRIER_COLOR);
	barrier.corners.push_back(Pos2D(-200, 200));
	barrier.corners.push_back(Pos2D(0, 300));
	barrier.corners.push_back(Pos2D(200, 200));
	barrier.corners.push_back(Pos2D(200, -200));
	barrier.corners.push_back(Pos2D(-200, -200));
	barriers.push_back(barrier);


	Line line(BARRIER_COLOR);
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
	Line finishLine(FINISH_COLOR);
	finishLine.points.push_back(Pos2D(608, 160));
	finishLine.points.push_back(Pos2D(608, 208));

	car.init(55, 970, 0);

	boxes.push_back(Box().init(471, 647, 16, 16, 45, OBJ_COLOR));

	NGon barrier(BARRIER_COLOR);
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
	Line finishLine(FINISH_COLOR);
	finishLine.points = {Pos2D(1875, -1414), Pos2D(1946, -1270)};

	car.init(70, 500, 0);

	NGon barrier(BARRIER_COLOR);
	barrier.corners = {
		Pos2D(34, 532), Pos2D(133, 533), Pos2D(148, 452), Pos2D(123, 368),
		Pos2D(155, 277), Pos2D(211, 209), Pos2D(231, 89), Pos2D(307, 84),
		Pos2D(416, 106), Pos2D(511, 101), Pos2D(590, 127), Pos2D(665, 176),
		Pos2D(741, 191), Pos2D(810, 227), Pos2D(876, 240), Pos2D(929, 231),
		Pos2D(989, 251), Pos2D(1070, 242), Pos2D(1153, 185), Pos2D(1248, 155),
		Pos2D(1276, 110), Pos2D(1331, 85), Pos2D(1362, 36), Pos2D(1411, -3),
		Pos2D(1452, -60), Pos2D(1462, -125), Pos2D(1496, -179), Pos2D(1497, -261),
		Pos2D(1528, -356), Pos2D(1513, -405), Pos2D(1527, -452), Pos2D(1541, -536),
		Pos2D(1512, -627), Pos2D(1531, -673), Pos2D(1539, -739), Pos2D(1529, -796),
		Pos2D(1542, -849), Pos2D(1530, -904), Pos2D(1536, -992), Pos2D(1507, -1057),
		Pos2D(1534, -1115), Pos2D(1517, -1166), Pos2D(1486, -1194), Pos2D(1475, -1261),
		Pos2D(1425, -1323), Pos2D(1417, -1404), Pos2D(1364, -1495), Pos2D(1291, -1552),
		Pos2D(1201, -1602), Pos2D(1164.729, -1633.9453), Pos2D(1114, -1679),
		Pos2D(1024, -1737), Pos2D(961, -1776), Pos2D(834, -1776), Pos2D(770, -1797),
		Pos2D(666.9087, -1812.897), Pos2D(532, -1821), Pos2D(419, -1844),
		Pos2D(322, -1835), Pos2D(213, -1855), Pos2D(144, -1817), Pos2D(65, -1814),
		Pos2D(-24, -1769), Pos2D(-116, -1773), Pos2D(-207, -1715), Pos2D(-279, -1691),
		Pos2D(-336, -1689), Pos2D(-374, -1633), Pos2D(-458, -1610), Pos2D(-489, -1566),
		Pos2D(-535, -1509), Pos2D(-612, -1460), Pos2D(-673, -1399), Pos2D(-721, -1302),
		Pos2D(-796, -1231), Pos2D(-819, -1168), Pos2D(-898, -1047), Pos2D(-1017, -1042),
		Pos2D(-1128, -1079), Pos2D(-1279, -1203), Pos2D(-1314, -1278), Pos2D(-1371, -1328),
		Pos2D(-1426, -1405), Pos2D(-1475, -1494), Pos2D(-1561, -1548), Pos2D(-1637, -1651),
		Pos2D(-1693, -1676), Pos2D(-1727, -1737), Pos2D(-1826, -1849), Pos2D(-1919, -1895),
		Pos2D(-2019, -1984), Pos2D(-2063, -2097), Pos2D(-2154, -2183), Pos2D(-2188, -2287),
		Pos2D(-2253, -2378), Pos2D(-2253, -2478), Pos2D(-2305, -2548), Pos2D(-2299, -2635),
		Pos2D(-2254, -2716), Pos2D(-2259, -2790), Pos2D(-2224, -2869), Pos2D(-2223, -2934),
		Pos2D(-2147, -2999), Pos2D(-2122, -3086), Pos2D(-2017, -3125), Pos2D(-1990, -3239),
		Pos2D(-1903.9442, -3317.1826), Pos2D(-1770, -3448), Pos2D(-1587, -3598),
		Pos2D(-1458, -3760), Pos2D(-1336, -3779), Pos2D(-1237.5073, -3838.0208),
		Pos2D(-1122, -3906), Pos2D(-1047, -3932), Pos2D(-938, -3944), Pos2D(-835, -3957),
		Pos2D(-763, -3986), Pos2D(-702.5055, -4030.019), Pos2D(-456, -4105), Pos2D(-354, -4113),
		Pos2D(-248, -4150), Pos2D(-131.82977, -4215.0615), Pos2D(31, -4305),
		Pos2D(175, -4357), Pos2D(389.44812, -4397.776), Pos2D(521, -4428),
		Pos2D(627, -4511), Pos2D(744, -4611), Pos2D(796, -4651), Pos2D(899, -4713),
		Pos2D(1041, -4751), Pos2D(1153.4712, -4712.3867), Pos2D(1242, -4683),
		Pos2D(1341, -4681), Pos2D(1426, -4700), Pos2D(1540, -4717), Pos2D(1642, -4654),
		Pos2D(1726, -4595), Pos2D(1793, -4554), Pos2D(1810, -4492), Pos2D(1853, -4415),
		Pos2D(1844, -4283), Pos2D(1804, -4179), Pos2D(1752, -4119), Pos2D(1646, -4073),
		Pos2D(1586, -4014), Pos2D(1505.9966, -3940.9055), Pos2D(1421, -3875),
		Pos2D(1330, -3818), Pos2D(1180, -3782), Pos2D(1041, -3751), Pos2D(930, -3701),
		Pos2D(850, -3604), Pos2D(742, -3519), Pos2D(639, -3448), Pos2D(552, -3373),
		Pos2D(425, -3369), Pos2D(273, -3369), Pos2D(149, -3350), Pos2D(7, -3279),
		Pos2D(-83, -3182), Pos2D(-149, -3031), Pos2D(-158, -2900),
		Pos2D(-133.65906, -2788.3894), Pos2D(-103, -2710), Pos2D(56, -2584),
		Pos2D(167, -2504), Pos2D(294, -2485), Pos2D(388, -2510), Pos2D(430, -2516),
		Pos2D(458, -2490), Pos2D(452, -2457), Pos2D(427, -2434), Pos2D(371, -2408),
		Pos2D(347, -2371), Pos2D(347, -2329), Pos2D(386, -2268), Pos2D(467, -2151),
		Pos2D(518, -2094), Pos2D(584, -2011), Pos2D(663, -1967), Pos2D(851.1852, -1869.0002),
		Pos2D(952, -1795), Pos2D(1088.1783, -1702.465), Pos2D(1274.387, -1575.9338),
		Pos2D(1445, -1460), Pos2D(1547, -1334), Pos2D(1671, -1239), Pos2D(1764, -1195),
		Pos2D(1812, -1188), Pos2D(1854, -1202), Pos2D(1935, -1247), Pos2D(1952, -1279),
		Pos2D(1888, -1414), Pos2D(1855, -1417), Pos2D(1766, -1373), Pos2D(1744, -1370),
		Pos2D(1722, -1372), Pos2D(1684.4708, -1396.6846), Pos2D(1624, -1469),
		Pos2D(1471, -1573), Pos2D(1326.0784, -1645.8872), Pos2D(1245, -1708),
		Pos2D(1157, -1826), Pos2D(1081, -1877), Pos2D(939.8856, -1936.974),
		Pos2D(745, -2048), Pos2D(642, -2115), Pos2D(580, -2188), Pos2D(515, -2260),
		Pos2D(498.0656, -2298.8494), Pos2D(499, -2336), Pos2D(511.56036, -2368.5605),
		Pos2D(576, -2433), Pos2D(592, -2512), Pos2D(571, -2559), Pos2D(497, -2594),
		Pos2D(383, -2608), Pos2D(274, -2641), Pos2D(171, -2722), Pos2D(62, -2827),
		Pos2D(10, -2950), Pos2D(41, -3053), Pos2D(93, -3142), Pos2D(147, -3175),
		Pos2D(226, -3232), Pos2D(342, -3249), Pos2D(453, -3223), Pos2D(586, -3236),
		Pos2D(697, -3305), Pos2D(772, -3358), Pos2D(859, -3465), Pos2D(956, -3548),
		Pos2D(1046, -3608), Pos2D(1186, -3656), Pos2D(1356, -3696), Pos2D(1428, -3736),
		Pos2D(1573.9967, -3859.1575), Pos2D(1652, -3948), Pos2D(1730, -3989),
		Pos2D(1842, -4044), Pos2D(1902, -4134), Pos2D(1956, -4239), Pos2D(1971, -4325),
		Pos2D(1975, -4443), Pos2D(1949, -4539), Pos2D(1898, -4638), Pos2D(1816, -4717),
		Pos2D(1716, -4756), Pos2D(1634, -4801), Pos2D(1535.6702, -4837.091),
		Pos2D(1416, -4801), Pos2D(1272, -4799), Pos2D(1180, -4811), Pos2D(1053, -4850),
		Pos2D(863, -4807), Pos2D(749, -4719), Pos2D(680, -4686), Pos2D(559, -4597),
		Pos2D(439, -4515), Pos2D(319, -4476), Pos2D(132, -4453), Pos2D(-29, -4389),
		Pos2D(-132.18945, -4324.3076), Pos2D(-267, -4256), Pos2D(-396, -4224),
		Pos2D(-635.37256, -4163.837), Pos2D(-693, -4173), Pos2D(-753, -4144),
		Pos2D(-823, -4104), Pos2D(-985.1123, -4047.762), Pos2D(-1077, -3996),
		Pos2D(-1153, -3972), Pos2D(-1317.4955, -3932.4548), Pos2D(-1396, -3882),
		Pos2D(-1535, -3857), Pos2D(-1675, -3675), Pos2D(-1867, -3540),
		Pos2D(-1925, -3448), Pos2D(-2038.6335, -3391.4282), Pos2D(-2078, -3303),
		Pos2D(-2153, -3229), Pos2D(-2254, -3205), Pos2D(-2286, -3066), Pos2D(-2360, -2951),
		Pos2D(-2351, -2864), Pos2D(-2373, -2794), Pos2D(-2358, -2740), Pos2D(-2410, -2649),
		Pos2D(-2396, -2592), Pos2D(-2408, -2534), Pos2D(-2375, -2460), Pos2D(-2377, -2403),
		Pos2D(-2348, -2327), Pos2D(-2306, -2287), Pos2D(-2286.5469, -2222.0938),
		Pos2D(-2245, -2129), Pos2D(-2173, -2064), Pos2D(-2155, -1983), Pos2D(-2088, -1902),
		Pos2D(-2015, -1827), Pos2D(-1940, -1792), Pos2D(-1817, -1685), Pos2D(-1773, -1598),
		Pos2D(-1716, -1566), Pos2D(-1683, -1504), Pos2D(-1660, -1459), Pos2D(-1596, -1423),
		Pos2D(-1556, -1349), Pos2D(-1498, -1309), Pos2D(-1459, -1243), Pos2D(-1394, -1186),
		Pos2D(-1377, -1134), Pos2D(-1267, -1064), Pos2D(-1129, -972), Pos2D(-982, -926),
		Pos2D(-874, -945), Pos2D(-819, -983), Pos2D(-752, -1088), Pos2D(-721, -1166),
		Pos2D(-650, -1245), Pos2D(-624, -1301), Pos2D(-567, -1382), Pos2D(-504, -1406),
		Pos2D(-445, -1486), Pos2D(-364, -1530), Pos2D(-277, -1584), Pos2D(-252, -1606),
		Pos2D(-167, -1633), Pos2D(-87, -1645), Pos2D(-11, -1695), Pos2D(54, -1698),
		Pos2D(120, -1741), Pos2D(273, -1757), Pos2D(411, -1751), Pos2D(472, -1718),
		Pos2D(546, -1738), Pos2D(688, -1712), Pos2D(742, -1706), Pos2D(792, -1678),
		Pos2D(871, -1679), Pos2D(926, -1642), Pos2D(984, -1631), Pos2D(1058, -1578),
		Pos2D(1081, -1526), Pos2D(1148, -1479), Pos2D(1253, -1425), Pos2D(1294, -1327),
		Pos2D(1343, -1266), Pos2D(1376, -1162), Pos2D(1368, -1080), Pos2D(1386, -1009),
		Pos2D(1375, -930), Pos2D(1407, -865), Pos2D(1403, -790), Pos2D(1420, -735),
		Pos2D(1413, -681), Pos2D(1400, -606), Pos2D(1412, -556), Pos2D(1399, -494),
		Pos2D(1419, -440), Pos2D(1405, -404), Pos2D(1404, -287), Pos2D(1377, -219),
		Pos2D(1352, -156), Pos2D(1313, -114), Pos2D(1283, -30), Pos2D(1237, -1), Pos2D(1209, 40), Pos2D(1148, 67),
		Pos2D(1126, 101), Pos2D(1082, 101), Pos2D(1000, 138), Pos2D(911, 117), Pos2D(842, 127),
		Pos2D(772, 98), Pos2D(684, 89), Pos2D(630, 49), Pos2D(560, 26),
		Pos2D(448, 33), Pos2D(339, 3), Pos2D(223, 8), Pos2D(126, 79),
		Pos2D(111, 176), Pos2D(68, 273), Pos2D(40, 362), Pos2D(43, 443),
		Pos2D(34, 532),
	};
	barriers.push_back(barrier);

	runLevel(car, boxes, barriers, lines, finishLine);
}
