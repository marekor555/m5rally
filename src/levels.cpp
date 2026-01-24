#include <Arduino.h>
#include <M5Cardputer.h>
#include <M5GFX.h>
#include <SPI.h>
#include <SD.h>
#include <ArduinoJson.h>

#include "car.h"
#include "collision.h"
#include "particle.h"
#include "utils.h"

#define GROUND_COLOR M5GFX::color565(10, 50, 0)
#define BARRIER_COLOR M5GFX::color565(255, 115, 0)
#define BARRIER_INFILL M5GFX::color565(33, 33, 33)
#define OBJ_COLOR M5GFX::color565(82, 48, 0)
#define FINISH_COLOR M5GFX::color565(149, 255, 0)

#define FRAME_TIME_MS 10 // 16ms -> abt. 60 fps
#define MAX_CAR_PARTICLES 400

void runLevel(Car car, const std::vector<Box> &boxes, const std::vector<NGon> &barriers, const std::vector<Line> &lines,
			const Line &finishLine) {
	unsigned long frameStart = millis();
	ParticleSpawner particle_spawner(MAX_CAR_PARTICLES, 0);
	while (true) {
		const unsigned long now = millis();
		float delta = static_cast<float>(now - frameStart) / 1000;
		if (delta < 0.001f) delta = 0.001f;
		frameStart = now;

		if (!car.tick(boxes, barriers, lines, finishLine, delta)) break;
		if (particle_spawner.tick(delta)) {
			particle_spawner.spawn(car.posX, car.posY, 3, 7, TFT_BLACK);
		}
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


		for (const NGon &barrier: barriers) {
			barrier.drawInfill(&canvas, camposX, camposY);
			barrier.drawOutline(&canvas, camposX, camposY);
		}
		for (const Box &box: boxes) {
			box.draw(&canvas, camposX, camposY);
		}
		for (const Line &line: lines) {
			line.drawOutline(&canvas, camposX, camposY);
		}
		finishLine.drawOutline(&canvas, camposX, camposY);
		particle_spawner.draw(&canvas, camposX, camposY);

		car.draw(&canvas, camposX, camposY);

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

void parseAndRun(const char* json) {
	JsonDocument doc;
	const DeserializationError error = deserializeJson(doc, json, DeserializationOption::NestingLimit(50));

	if (error) {
		M5Cardputer.Display.fillScreen(TFT_BLACK);
		M5Cardputer.Display.setCursor(0,0);
		M5Cardputer.Display.printf("JSON Error: %s\n", error.c_str());
		int len = strlen(json);
		M5Cardputer.Display.printf("Len: %d\n", len);
		if (len > 20) M5Cardputer.Display.println(json + len - 20);
		while (!M5Cardputer.Keyboard.isPressed()) { M5Cardputer.update(); }
		return;
	}

	Car car;
	std::vector<Box> boxes;
	std::vector<NGon> barriers;
	std::vector<Line> lines;
	Line finishLine(FINISH_COLOR);

	car.init(doc["car"]["x"], doc["car"]["y"], doc["car"]["angle"]);

	JsonArray boxesJson = doc["boxes"];
	for (JsonObject box : boxesJson) {
		boxes.push_back(Box().init(box["x"], box["y"], box["w"], box["h"], box["angle"], OBJ_COLOR));
	}

	JsonArray barriersJson = doc["barriers"];
	for (JsonObject box : barriersJson) {
		NGon barrier(BARRIER_COLOR, BARRIER_INFILL);

		for (JsonObject point : box["points"].as<JsonArray>()) {
			barrier.corners.push_back(Pos2D(point["x"], point["y"]));
		}

		barriers.push_back(barrier);
	}

	JsonArray linesJson = doc["lines"];
	for (JsonObject line_ : linesJson) {
		Line line(BARRIER_COLOR);

		for (JsonObject point : line_["points"].as<JsonArray>()) {
			line.points.push_back(Pos2D(point["x"], point["y"]));
		}

		lines.push_back(line);
	}


	for (JsonObject point : doc["finishLine"].as<JsonArray>()) {
		finishLine.points.push_back(Pos2D(point["x"], point["y"]));
	}

	runLevel(car, boxes, barriers, lines, finishLine);
}

void loadLevel() {
	debounceKeyboard();
	std::vector<String> files;
	String view = "/";
	SPI.begin(40, 39, 14, 12);
	while (!SD.begin(12, SPI)) {}

	while (true) {
		File root = SD.open(view);
		files.clear();
		if (view == "/") files.push_back("||-exit-||");
		if (view != "/") files.push_back("|..|");
		File file = root.openNextFile();
		while (file) {
			if (file.isDirectory()) {
				files.push_back("|"+String(file.name())+"|");
			}
			file = root.openNextFile();
		}
		root = SD.open(view);
		file = root.openNextFile();
		while (file) {
			if (!file.isDirectory()) {
				files.push_back(String(file.name()));
			}
			file = root.openNextFile();
		}

		String filename = scrollTextArrHighlight(files, true, TFT_WHITE, TFT_BLUE);
		if (filename == "||-exit-||") {
			SD.end();
			return;
		} if (filename == "|..|") {
			view = view.substring(0, view.lastIndexOf("/"));
		} else {
			if (filename.startsWith("|") && filename.endsWith("|")) {
				filename = filename.substring(1, filename.length()-1);
			}
			File selectedFile = SD.open(view + "/" + filename);
			if (selectedFile.isDirectory()) {
				view += "/" + filename;
			} else {
				const String content = selectedFile.readString();
				selectedFile.close();

				parseAndRun(content.c_str());
				return;
			}
		}
	}
}



void testLevel() {
	parseAndRun(R"(
		{
		  "car": {
		    "x": -40,
		    "y": -40,
		    "angle": 90
		  },
		  "boxes": [
		    { "x": 0, "y": 0, "w": 25, "h": 25, "angle": 45 },
		    { "x": -90, "y": 0, "w": 20, "h": 200, "angle": 0 },
		    { "x": 0, "y": -90, "w": 20, "h": 200, "angle": 100 }
		  ],
		  "barriers": [
		    {
		      "points": [
		        { "x": -200, "y": 200 },
		        { "x": 0, "y": 300 },
		        { "x": 200, "y": 200 },
		        { "x": 200, "y": -200 },
		        { "x": -200, "y": -200 }
		      ]
		    }
		  ],
		  "lines": [
		    {
		      "points": [
		        { "x": 10, "y": 10 },
		        { "x": 20, "y": 40 },
		        { "x": 30, "y": 90 },
		        { "x": 40, "y": 160 },
		        { "x": 0, "y": 300 }
		      ]
		    },
		    {
		      "points": [
		        { "x": -40, "y": 160 },
		        { "x": -80, "y": 100 }
		      ]
		    }
		  ],
		  "finishLine": [
		    { "x": 40, "y": 160 },
		    { "x": -40, "y": 160 }
		  ]
		}
	)");
}

void first() {
	parseAndRun(R"({"car": {"x": 55, "y": 970, "angle": 0}, "boxes": [{"x": 471, "y": 647, "w": 16, "h": 16, "angle": 45}], "barriers": [{"points": [{"x": 32, "y": 991}, {"x": 79, "y": 991}, {"x": 70, "y": 844}, {"x": 86, "y": 799}, {"x": 128, "y": 735}, {"x": 256, "y": 737}, {"x": 255, "y": 1022}, {"x": 416, "y": 1022}, {"x": 432, "y": 1007}, {"x": 447, "y": 943}, {"x": 495, "y": 801}, {"x": 511, "y": 626}, {"x": 496, "y": 547}, {"x": 496, "y": 495}, {"x": 508, "y": 464}, {"x": 558, "y": 440}, {"x": 895, "y": 440}, {"x": 942, "y": 413}, {"x": 976, "y": 207}, {"x": 976, "y": 160}, {"x": 606, "y": 160}, {"x": 606, "y": 208}, {"x": 933, "y": 208}, {"x": 933, "y": 279}, {"x": 908, "y": 378}, {"x": 892, "y": 401}, {"x": 558, "y": 400}, {"x": 496, "y": 431}, {"x": 464, "y": 463}, {"x": 449, "y": 494}, {"x": 449, "y": 546}, {"x": 432, "y": 626}, {"x": 447, "y": 786}, {"x": 383, "y": 960}, {"x": 368, "y": 975}, {"x": 304, "y": 975}, {"x": 303, "y": 698}, {"x": 127, "y": 699}, {"x": 95, "y": 719}, {"x": 47, "y": 783}, {"x": 32, "y": 845}]}], "lines": [], "finishLine": [{"x": 608, "y": 160}, {"x": 608, "y": 208}]})");
}

void second() {
	parseAndRun(R"({
	  "car": { "x": 70, "y": 500, "angle": 0 },
	  "boxes": [],
	  "barriers": [
	    {
	      "points": [
	        {"x":34,"y":532},{"x":133,"y":533},{"x":148,"y":452},{"x":123,"y":368},
	        {"x":155,"y":277},{"x":211,"y":209},{"x":231,"y":89},{"x":307,"y":84},
	        {"x":416,"y":106},{"x":511,"y":101},{"x":590,"y":127},{"x":665,"y":176},
	        {"x":741,"y":191},{"x":810,"y":227},{"x":876,"y":240},{"x":929,"y":231},
	        {"x":989,"y":251},{"x":1070,"y":242},{"x":1153,"y":185},{"x":1248,"y":155},
	        {"x":1276,"y":110},{"x":1331,"y":85},{"x":1362,"y":36},{"x":1411,"y":-3},
	        {"x":1452,"y":-60},{"x":1462,"y":-125},{"x":1496,"y":-179},{"x":1497,"y":-261},
	        {"x":1528,"y":-356},{"x":1513,"y":-405},{"x":1527,"y":-452},{"x":1541,"y":-536},
	        {"x":1512,"y":-627},{"x":1531,"y":-673},{"x":1539,"y":-739},{"x":1529,"y":-796},
	        {"x":1542,"y":-849},{"x":1530,"y":-904},{"x":1536,"y":-992},{"x":1507,"y":-1057},
	        {"x":1534,"y":-1115},{"x":1517,"y":-1166},{"x":1486,"y":-1194},{"x":1475,"y":-1261},
	        {"x":1425,"y":-1323},{"x":1417,"y":-1404},{"x":1364,"y":-1495},{"x":1291,"y":-1552},
	        {"x":1201,"y":-1602},{"x":1164.729,"y":-1633.9453},{"x":1114,"y":-1679},
	        {"x":1024,"y":-1737},{"x":961,"y":-1776},{"x":834,"y":-1776},
	        {"x":770,"y":-1797},{"x":666.9087,"y":-1812.897},{"x":532,"y":-1821},
	        {"x":419,"y":-1844},{"x":322,"y":-1835},{"x":213,"y":-1855},
	        {"x":144,"y":-1817},{"x":65,"y":-1814},{"x":-24,"y":-1769},
	        {"x":-116,"y":-1773},{"x":-207,"y":-1715},{"x":-279,"y":-1691},
	        {"x":-336,"y":-1689},{"x":-374,"y":-1633},{"x":-458,"y":-1610},
	        {"x":-489,"y":-1566},{"x":-535,"y":-1509},{"x":-612,"y":-1460},
	        {"x":-673,"y":-1399},{"x":-721,"y":-1302},{"x":-796,"y":-1231},
	        {"x":-819,"y":-1168},{"x":-898,"y":-1047},{"x":-1017,"y":-1042},
	        {"x":-1128,"y":-1079},{"x":-1279,"y":-1203},{"x":-1314,"y":-1278},
	        {"x":-1371,"y":-1328},{"x":-1426,"y":-1405},{"x":-1475,"y":-1494},
	        {"x":-1561,"y":-1548},{"x":-1637,"y":-1651},{"x":-1693,"y":-1676},
	        {"x":-1727,"y":-1737},{"x":-1826,"y":-1849},{"x":-1919,"y":-1895},
	        {"x":-2019,"y":-1984},{"x":-2063,"y":-2097},{"x":-2154,"y":-2183},
	        {"x":-2188,"y":-2287},{"x":-2253,"y":-2378},{"x":-2253,"y":-2478},
	        {"x":-2305,"y":-2548},{"x":-2299,"y":-2635},{"x":-2254,"y":-2716},
	        {"x":-2259,"y":-2790},{"x":-2224,"y":-2869},{"x":-2223,"y":-2934},
	        {"x":-2147,"y":-2999},{"x":-2122,"y":-3086},{"x":-2017,"y":-3125},
	        {"x":-1990,"y":-3239},{"x":-1903.9442,"y":-3317.1826},
	        {"x":-1770,"y":-3448},{"x":-1587,"y":-3598},{"x":-1458,"y":-3760},
	        {"x":-1336,"y":-3779},{"x":-1237.5073,"y":-3838.0208},
	        {"x":-1122,"y":-3906},{"x":-1047,"y":-3932},{"x":-938,"y":-3944},
	        {"x":-835,"y":-3957},{"x":-763,"y":-3986},{"x":-702.5055,"y":-4030.019},
	        {"x":-456,"y":-4105},{"x":-354,"y":-4113},{"x":-248,"y":-4150},
	        {"x":-131.82977,"y":-4215.0615},{"x":31,"y":-4305},
	        {"x":175,"y":-4357},{"x":389.44812,"y":-4397.776},{"x":521,"y":-4428},
	        {"x":627,"y":-4511},{"x":744,"y":-4611},{"x":796,"y":-4651},
	        {"x":899,"y":-4713},{"x":1041,"y":-4751},{"x":1153.4712,"y":-4712.3867},
	        {"x":1242,"y":-4683},{"x":1341,"y":-4681},{"x":1426,"y":-4700},
	        {"x":1540,"y":-4717},{"x":1642,"y":-4654},{"x":1726,"y":-4595},
	        {"x":1793,"y":-4554},{"x":1810,"y":-4492},{"x":1853,"y":-4415},
	        {"x":1844,"y":-4283},{"x":1804,"y":-4179},{"x":1752,"y":-4119},
	        {"x":1646,"y":-4073},{"x":1586,"y":-4014},{"x":1505.9966,"y":-3940.9055},
	        {"x":1421,"y":-3875},{"x":1330,"y":-3818},{"x":1180,"y":-3782},
	        {"x":1041,"y":-3751},{"x":930,"y":-3701},{"x":850,"y":-3604},
	        {"x":742,"y":-3519},{"x":639,"y":-3448},{"x":552,"y":-3373},
	        {"x":425,"y":-3369},{"x":273,"y":-3369},{"x":149,"y":-3350},
	        {"x":7,"y":-3279},{"x":-83,"y":-3182},{"x":-149,"y":-3031},
	        {"x":-158,"y":-2900},{"x":-133.65906,"y":-2788.3894},
	        {"x":-103,"y":-2710},{"x":56,"y":-2584},{"x":167,"y":-2504},
	        {"x":294,"y":-2485},{"x":388,"y":-2510},{"x":430,"y":-2516},
	        {"x":458,"y":-2490},{"x":452,"y":-2457},{"x":427,"y":-2434},
	        {"x":371,"y":-2408},{"x":347,"y":-2371},{"x":347,"y":-2329},
	        {"x":386,"y":-2268},{"x":467,"y":-2151},{"x":518,"y":-2094},
	        {"x":584,"y":-2011},{"x":663,"y":-1967},{"x":851.1852,"y":-1869.0002},
	        {"x":952,"y":-1795},{"x":1088.1783,"y":-1702.465},
	        {"x":1274.387,"y":-1575.9338},{"x":1445,"y":-1460},
	        {"x":1547,"y":-1334},{"x":1671,"y":-1239},{"x":1764,"y":-1195},
	        {"x":1812,"y":-1188},{"x":1854,"y":-1202},{"x":1935,"y":-1247},
	        {"x":1952,"y":-1279},{"x":1888,"y":-1414},{"x":1855,"y":-1417},
	        {"x":1766,"y":-1373},{"x":1744,"y":-1370},{"x":1722,"y":-1372},
	        {"x":1684.4708,"y":-1396.6846},{"x":1624,"y":-1469},
	        {"x":1471,"y":-1573},{"x":1326.0784,"y":-1645.8872},
	        {"x":1245,"y":-1708},{"x":1157,"y":-1826},{"x":1081,"y":-1877},
	        {"x":939.8856,"y":-1936.974},{"x":745,"y":-2048},
	        {"x":642,"y":-2115},{"x":580,"y":-2188},{"x":515,"y":-2260},
	        {"x":498.0656,"y":-2298.8494},{"x":499,"y":-2336},
	        {"x":511.56036,"y":-2368.5605},{"x":576,"y":-2433},
	        {"x":592,"y":-2512},{"x":571,"y":-2559},{"x":497,"y":-2594},
	        {"x":383,"y":-2608},{"x":274,"y":-2641},{"x":171,"y":-2722},
	        {"x":62,"y":-2827},{"x":10,"y":-2950},{"x":41,"y":-3053},
	        {"x":93,"y":-3142},{"x":147,"y":-3175},{"x":226,"y":-3232},
	        {"x":342,"y":-3249},{"x":453,"y":-3223},{"x":586,"y":-3236},
	        {"x":697,"y":-3305},{"x":772,"y":-3358},{"x":859,"y":-3465},
	        {"x":956,"y":-3548},{"x":1046,"y":-3608},{"x":1186,"y":-3656},
	        {"x":1356,"y":-3696},{"x":1428,"y":-3736},{"x":1573.9967,"y":-3859.1575},
	        {"x":1652,"y":-3948},{"x":1730,"y":-3989},{"x":1842,"y":-4044},
	        {"x":1902,"y":-4134},{"x":1956,"y":-4239},{"x":1971,"y":-4325},
	        {"x":1975,"y":-4443},{"x":1949,"y":-4539},{"x":1898,"y":-4638},
	        {"x":1816,"y":-4717},{"x":1716,"y":-4756},{"x":1634,"y":-4801},
	        {"x":1535.6702,"y":-4837.091},{"x":1416,"y":-4801},
	        {"x":1272,"y":-4799},{"x":1180,"y":-4811},{"x":1053,"y":-4850},
	        {"x":863,"y":-4807},{"x":749,"y":-4719},{"x":680,"y":-4686},
	        {"x":559,"y":-4597},{"x":439,"y":-4515},{"x":319,"y":-4476},
	        {"x":132,"y":-4453},{"x":-29,"y":-4389},{"x":-132.18945,"y":-4324.3076},
	        {"x":-267,"y":-4256},{"x":-396,"y":-4224},{"x":-635.37256,"y":-4163.837},
	        {"x":-693,"y":-4173},{"x":-753,"y":-4144},{"x":-823,"y":-4104},
	        {"x":-985.1123,"y":-4047.762},{"x":-1077,"y":-3996},
	        {"x":-1153,"y":-3972},{"x":-1317.4955,"y":-3932.4548},
	        {"x":-1396,"y":-3882},{"x":-1535,"y":-3857},{"x":-1675,"y":-3675},
	        {"x":-1867,"y":-3540},{"x":-1925,"y":-3448},{"x":-2038.6335,"y":-3391.4282},
	        {"x":-2078,"y":-3303},{"x":-2153,"y":-3229},{"x":-2254,"y":-3205},
	        {"x":-2286,"y":-3066},{"x":-2360,"y":-2951},{"x":-2351,"y":-2864},
	        {"x":-2373,"y":-2794},{"x":-2358,"y":-2740},{"x":-2410,"y":-2649},
	        {"x":-2396,"y":-2592},{"x":-2408,"y":-2534},{"x":-2375,"y":-2460},
	        {"x":-2377,"y":-2403},{"x":-2348,"y":-2327},{"x":-2306,"y":-2287},
	        {"x":-2286.5469,"y":-2222.0938},{"x":-2245,"y":-2129},
	        {"x":-2173,"y":-2064},{"x":-2155,"y":-1983},{"x":-2088,"y":-1902},
	        {"x":-2015,"y":-1827},{"x":-1940,"y":-1792},{"x":-1817,"y":-1685},
	        {"x":-1773,"y":-1598},{"x":-1716,"y":-1566},{"x":-1683,"y":-1504},
	        {"x":-1660,"y":-1459},{"x":-1596,"y":-1423},{"x":-1556,"y":-1349},
	        {"x":-1498,"y":-1309},{"x":-1459,"y":-1243},{"x":-1394,"y":-1186},
	        {"x":-1377,"y":-1134},{"x":-1267,"y":-1064},{"x":-1129,"y":-972},
	        {"x":-982,"y":-926},{"x":-874,"y":-945},{"x":-819,"y":-983},
	        {"x":-752,"y":-1088},{"x":-721,"y":-1166},{"x":-650,"y":-1245},
	        {"x":-624,"y":-1301},{"x":-567,"y":-1382},{"x":-504,"y":-1406},
	        {"x":-445,"y":-1486},{"x":-364,"y":-1530},{"x":-277,"y":-1584},
	        {"x":-252,"y":-1606},{"x":-167,"y":-1633},{"x":-87,"y":-1645},
	        {"x":-11,"y":-1695},{"x":54,"y":-1698},{"x":120,"y":-1741},
	        {"x":273,"y":-1757},{"x":411,"y":-1751},{"x":472,"y":-1718},
	        {"x":546,"y":-1738},{"x":688,"y":-1712},{"x":742,"y":-1706},
	        {"x":792,"y":-1678},{"x":871,"y":-1679},{"x":926,"y":-1642},
	        {"x":984,"y":-1631},{"x":1058,"y":-1578},{"x":1081,"y":-1526},
	        {"x":1148,"y":-1479},{"x":1253,"y":-1425},{"x":1294,"y":-1327},
	        {"x":1343,"y":-1266},{"x":1376,"y":-1162},{"x":1368,"y":-1080},
	        {"x":1386,"y":-1009},{"x":1375,"y":-930},{"x":1407,"y":-865},
	        {"x":1403,"y":-790},{"x":1420,"y":-735},{"x":1413,"y":-681},
	        {"x":1400,"y":-606},{"x":1412,"y":-556},{"x":1399,"y":-494},
	        {"x":1419,"y":-440},{"x":1405,"y":-404},{"x":1404,"y":-287},
	        {"x":1377,"y":-219},{"x":1352,"y":-156},{"x":1313,"y":-114},
	        {"x":1283,"y":-30},{"x":1237,"y":-1},{"x":1209,"y":40},
	        {"x":1148,"y":67},{"x":1126,"y":101},{"x":1082,"y":101},
	        {"x":1000,"y":138},{"x":911,"y":117},{"x":842,"y":127},
	        {"x":772,"y":98},{"x":684,"y":89},{"x":630,"y":49},
	        {"x":560,"y":26},{"x":448,"y":33},{"x":339,"y":3},
	        {"x":223,"y":8},{"x":126,"y":79},{"x":111,"y":176},
	        {"x":68,"y":273},{"x":40,"y":362},{"x":43,"y":443},
	        {"x":34,"y":532}
	      ]
	    }
	  ],
	  "lines": [],
	  "finishLine": [
	    {"x":1875,"y":-1414},
	    {"x":1946,"y":-1270}
	  ]
	})");
}
