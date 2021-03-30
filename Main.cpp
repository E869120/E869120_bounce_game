#include <Siv3D.hpp> // OpenSiv3D v0.4.1
#include <iostream>
#include <algorithm>
#include <tuple>
#include <ctime>
#include <vector>
#include <time.h>
#include <stdio.h>
using namespace std;
#pragma warning (disable: 4996)

struct Ball { double px, py, vx, vy, ti; int sc, flag; };
struct Mouses { double px, py, vx, vy; };

// 全体で使う変数
int Situation = 1;
double GetLastClick = 0.0;
double PI = 3.14159265358979;

// Situation = 1 のときに使う変数
int col1[4] = { 175, 175, 175, 175 };

// Situation = 2 のときに使う変数
int Rounds = 1; double Limit[30] = { 55, 50, 35 }, Times = Limit[0], Yuuyo = 5, TotalTime = 0;
int Score[30] = { 0, 0, 0 };
int Mass[30][70] = { {120, 120, 0, 120, 0, 120, 120}, {120, 80, 70, 60, 70, 80, 120}, {190, 80, 0, 60, 0, 80, 190} };
int Pts[30][70] = { { 3, 6, 10, 10, 10, 6, 3 }, { 1, 2, 3, 5, 3, 2, 1 }, { 0, 1, 2, 2, 2, 1, 0 } };
double shisuu[70] = { 0.17, 0.17, 0.17, 0.17, 0.17, 0.17, 0.17 };
int MA[70] = { 0, 255, 255, 255, 255, 255, 0 };
int MB[70] = { 255, 255, 100, 0, 100, 255, 255 };
int MC[70] = { 0, 0, 0, 0, 0, 0, 0 };
int Z1[30] = { 10, 17, 35 };
int Z2[30] = { 0, 1, 5 };
int Z3[30] = { 0, 5, 25 };
int Z4[30] = { 0, 5, 10 };
double KANKAKU[30] = { 5.0, 2.0, 0.5 };
double KANKAKU2[30] = { 5.0, 8.0, 3.0 };
Ball ball[500], ball2[500];
Mouses MI;
vector<tuple<int, int, double>> tup;
vector<tuple<int, int, int, double>> tensuu;

// Situation = 4 のときに使う変数
int NumberOfPlayers, MyRank = 0;
tuple<int, int, int, int, string, string> HighScore[1 << 12];
String Ranker[23] = { U"F", U"E", U"D", U"C", U"C+", U"B-", U"B", U"B+", U"A-", U"A", U"A+", U"S", U"SS", U"SSS", U"Champion", U"Genius", U"Prodigy", U"Master", U"Grandmaster", U"Legend", U"Bill-Gates", U"God", U"Ultimate-God" };
String GetChar[13] = { U"0", U"1", U"2", U"3", U"4", U"5", U"6", U"7", U"8", U"9", U"-", U":", U" " };
int RankID[23] = { 0, 60, 80, 96, 112, 124, 135, 146, 157, 168, 180, 191, 200, 208, 217, 226, 235, 244, 254, 264, 275, 287, 300 };

String GetRankToScore(int x) {
	int id = 0;
	for (int i = 0; i < 23; i++) {
		if (RankID[i] <= x) id = i;
	}
	return Ranker[id];
}

string GetTime() {
	time_t now = std::time(nullptr);
	struct tm* localNow = std::localtime(&now);

	int v[5] = { localNow->tm_mon + 1, localNow->tm_mday, localNow->tm_hour,  localNow->tm_min, localNow->tm_sec };
	string u[5] = { "","","","","" };
	for (int i = 0; i < 5; i++) {
		u[i] = to_string(v[i]);
		while (u[i].size() <= 1) u[i] = "0" + u[i];
	}

	return u[0] + "-" + u[1] + " " + u[2] + ":" + u[3] + ":" + u[4];
}

void initballs() {
	double TLE = 1.6; if (Rounds >= 2) TLE = 1.0;

	for (int i = 0; i < Z1[Rounds - 1]; i++) {
		while (true) {
			ball[i].px = (rand() % 5200) + 400; ball[i].px *= 0.1;
			ball[i].py = -20.0;
			ball[i].vx = (rand() % 8000) - 4000; ball[i].vx *= 0.1;
			ball[i].vy = (rand() % 400); ball[i].vy *= 0.1;
			ball[i].sc = 1;
			ball[i].flag = 0;

			if (ball[i].vx < -1.0 && (ball[i].px - 30.0) / -ball[i].vx < TLE) continue;
			if (ball[i].vx > 1.0 && (570.0 - ball[i].px) / ball[i].vx < TLE) continue;
			break;
		}
	}
	for (int i = Z1[Rounds - 1]; i < Z1[Rounds - 1] + Z3[Rounds - 1]; i++) {
		while (true) {
			ball[i].px = (rand() % 5200) + 400; ball[i].px *= 0.1;
			ball[i].py = -20.0;
			ball[i].vx = (rand() % 8000) - 4000; ball[i].vx *= 0.1;
			ball[i].vy = (rand() % 400); ball[i].vy *= 0.1;
			ball[i].sc = -1;
			ball[i].flag = 0;

			if (ball[i].vx < -1.0 && (ball[i].px - 30.0) / -ball[i].vx < TLE) continue;
			if (ball[i].vx > 1.0 && (570.0 - ball[i].px) / ball[i].vx < TLE) continue;
			break;
		}
	}
	for (int i = Z1[Rounds - 1] + Z3[Rounds - 1]; i < Z1[Rounds - 1] + Z2[Rounds - 1] + Z3[Rounds - 1]; i++) {
		while (true) {
			ball[i].px = (rand() % 5200) + 400; ball[i].px *= 0.1;
			ball[i].py = -20.0;
			ball[i].vx = (rand() % 8000) - 4000; ball[i].vx *= 0.1;
			ball[i].vy = (rand() % 400); ball[i].vy *= 0.1;
			ball[i].sc = 3;
			ball[i].flag = 0;

			if (ball[i].vx < -1.0 && (ball[i].px - 30.0) / -ball[i].vx < TLE) continue;
			if (ball[i].vx > 1.0 && (570.0 - ball[i].px) / ball[i].vx < TLE) continue;
			break;
		}
	}
	int KOSUU = Z1[Rounds - 1] + Z2[Rounds - 1] + Z3[Rounds - 1];
	for (int i = 0; i < 200; i++) swap(ball[rand() % (KOSUU - 1)], ball[rand() % (KOSUU - 1)]);
	for (int i = 0; i < KOSUU; i++) { ball[i].ti = -1.0 * i * KANKAKU[Rounds - 1]; }

	for (int i = 0; i < Z4[Rounds - 1]; i++) {
		ball2[i].px = (rand() % 5200) + 400; ball2[i].px *= 0.1;
		ball2[i].py = -20.0;
		ball2[i].vx = 0;
		ball2[i].vy = 150.0;
		ball2[i].sc = 5; if (Rounds == 3) ball2[i].sc = 3;
		ball2[i].flag = 0;
		ball2[i].ti = -(5.0 + 1.0 * i * KANKAKU2[Rounds - 1]);
	}
}

void refresh(double Delta) {
	for (int i = 0; i < Z1[Rounds - 1] + Z2[Rounds - 1] + Z3[Rounds - 1]; i++) {
		ball[i].ti += Delta;
		if (ball[i].ti < 0) continue;
		ball[i].px += ball[i].vx * Delta;
		ball[i].py += ball[i].vy * Delta;
		ball[i].vy += 240.0 * Delta;
		ball[i].vx *= (1.000 - 0.3 * Delta);
		ball[i].vy *= (1.000 - 0.3 * Delta);
		if (ball[i].px < 0) { ball[i].vx *= -0.84; ball[i].px = 0.0; tup.push_back(make_tuple(ball[i].px, ball[i].py, 0)); }
		if (ball[i].px >= 600.0) { ball[i].vx *= -0.84; ball[i].px = 600.0; tup.push_back(make_tuple(ball[i].px, ball[i].py, 0)); }
	}
	for (int i = 0; i < Z4[Rounds - 1]; i++) {
		ball2[i].ti += Delta;
		if (ball2[i].ti < 0) continue;
		ball2[i].px += ball2[i].vx * Delta;
		ball2[i].py += ball2[i].vy * Delta;
	}
}

void atari(double Delta) {
	for (int i = 0; i < Z1[Rounds - 1] + Z2[Rounds - 1] + Z3[Rounds - 1]; i++) {
		if (ball[i].ti < 0 || ball[i].py > 1000 || ball[i].flag >= 1) continue;
		double bx = ball[i].px - MI.px;
		double by = ball[i].py - MI.py;
		if (bx * bx + by * by >= 900.0) continue;

		double rat = 30.0 / sqrt(bx * bx + by * by);
		ball[i].px = MI.px + bx * rat;
		ball[i].py = MI.py + by * rat;
		tup.push_back(make_tuple(ball[i].px, ball[i].py, 0));

		// 速度を計算
		double VX = 0, VY = 0;
		double BallV = sqrt(ball[i].vx * ball[i].vx + ball[i].vy * ball[i].vy);
		double MouseV = sqrt(MI.vx * MI.vx + MI.vy * MI.vy);
		double K1 = atan2(ball[i].vy, ball[i].vx + 0.000001);
		double K2 = atan2(by, bx + 0.000001);
		double K3 = atan2(MI.vy, MI.vx + 0.000001);
		double soutai_ratio1 = sin(K1 - K2 - (0.5 * PI));
		double soutai_ratio2 = sin(K3 - K2 + (0.5 * PI));
		double soutai = (BallV * soutai_ratio1 + MouseV * soutai_ratio2) * 0.4 + MouseV * soutai_ratio2;
		VX += cos(K2) * soutai;
		VY += sin(K2) * soutai;

		double zettai = BallV * sin(K2 - K1 + PI);
		VX += cos(K2 + (0.5 * PI)) * zettai;
		VY += sin(K2 + (0.5 * PI)) * zettai;
		ball[i].vx = VX;
		ball[i].vy = VY;
		ball[i].px += ball[i].vx * Delta;
		ball[i].py += ball[i].vy * Delta;
	}

	vector<tuple<int, int, double>> tup2;
	for (int i = 0; i < tup.size(); i++) {
		get<2>(tup[i]) += Delta;
		if (get<2>(tup[i]) < 0.5) tup2.push_back(tup[i]);
	}
	tup = tup2;

	vector<tuple<int, int, int, double>> tensuu2;
	for (int i = 0; i < tensuu.size(); i++) {
		get<3>(tensuu[i]) += Delta;
		if (get<3>(tensuu[i]) < 0.5) tensuu2.push_back(tensuu[i]);
	}
	tensuu = tensuu2;
}

void all_init() {
	Rounds = 1; Times = Limit[0]; Yuuyo = 5;
	for (int i = 0; i < 3; i++) Score[i] = 0;
	for (int i = 0; i < 100; i++) { ball[i] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0, 0 }; /*ball2[i].clear();*/ }
	for (int i = 0; i < 4; i++) col1[i] = 175;
	for (int i = 0; i < 3; i++) shisuu[i] = 0.17;
	initballs();
}

void Main() {
	srand((unsigned)time(NULL));
	Scene::SetBackground(Color(0, 0, 0));

	const Font font120(120);
	const Font font80(80);
	const Font font44(44);
	const Font font34(34);
	const Font font28(28);
	const Font font21(21);
	const Font font18(18);
	const Font font12(12);
	const Texture tex1(Emoji(U"🎈"));

	FILE* in = freopen("ranking.txt", "r", stdin);
	cin >> NumberOfPlayers;
	for (int i = 0; i < NumberOfPlayers; i++) {
		cin >> get<0>(HighScore[i]) >> get<1>(HighScore[i]) >> get<2>(HighScore[i]);
		cin >> get<3>(HighScore[i]) >> get<4>(HighScore[i]) >> get<5>(HighScore[i]);
	}

	initballs();

	while (System::Update()) {
		// 状態 1 : 待ち受け画面
		TotalTime += Scene::DeltaTime();
		double MouseX = Cursor::PosF().x;
		double MouseY = Cursor::PosF().y;
		double ZURE = TotalTime * 30.0;
		while (ZURE >= 600.0) ZURE -= 600.0;

		if (Situation == 1) {
			font120(U"BOUNCE").draw(160, 15);
			font18(U"時は 2019 年 11 月 2 日。空を見上げると、なんと黄金の球が空から降ってくる！").draw(70, 180);
			font18(U"しかし、黄金の球は簡単には回収できない。真ん中に落とさないと割れてしまう。").draw(70, 200);
			font18(U"これはあなたが大富豪になる、最後にして最大のチャンス。どれだけ回収できるのか？").draw(50, 220);

			if (MouseX >= 40 && MouseX <= 380 && MouseY >= 300 && MouseY <= 380) col1[0] = max(col1[0] - 9, 31);
			else col1[0] = min(col1[0] + 9, 175);
			Rect(40, 300, 340, 80).draw(Color(col1[0], col1[0], 255));

			if (MouseX >= 420 && MouseX <= 760 && MouseY >= 300 && MouseY <= 380) col1[1] = max(col1[1] - 9, 31);
			else col1[1] = min(col1[1] + 9, 175);
			Rect(420, 300, 340, 80).draw(Color(col1[1], 255, col1[1]));

			if (MouseX >= 40 && MouseX <= 380 && MouseY >= 410 && MouseY <= 490) col1[2] = max(col1[2] - 9, 31);
			else col1[2] = min(col1[2] + 9, 175);
			Rect(40, 410, 340, 80).draw(Color(255, 255, col1[2]));

			if (MouseX >= 420 && MouseX <= 760 && MouseY >= 410 && MouseY <= 490) col1[3] = max(col1[3] - 9, 31);
			else col1[3] = min(col1[3] + 9, 175);
			Rect(420, 410, 340, 80).draw(Color(255, col1[3], col1[3]));

			if (Scene::Time() - GetLastClick >= 0.07 && MouseL.down() == true) {
				GetLastClick = Scene::Time();
				if (MouseX >= 40 && MouseX <= 380 && MouseY >= 300 && MouseY <= 380) Situation = 2;
				if (MouseX >= 420 && MouseX <= 760 && MouseY >= 300 && MouseY <= 380) Situation = 3;
				if (MouseX >= 40 && MouseX <= 380 && MouseY >= 410 && MouseY <= 490) Situation = 4;
				if (MouseX >= 420 && MouseX <= 760 && MouseY >= 410 && MouseY <= 490) break;
			}

			font44(U"あそぶ").draw(140, 310, Palette::Black);
			font44(U"あそびかた").draw(480, 310, Palette::Black);
			font44(U"ランキング").draw(100, 420, Palette::Black);
			font44(U"おわる").draw(520, 420, Palette::Black);
			font28(U"マウスで操作・選択をしてください").draw(160, 527, ColorF(Periodic::Sine0_1(1s)));
		}

		// 状態 2 : ゲーム画面
		if (Situation == 2) {
			// 画面左側
			Rect(0, 0, 600, 600).draw(Color(25, 25, 25));

			int DX = 310, DY = 250;
			if (3 < Yuuyo && Yuuyo <= 5 && Rounds <= 3) {
				int VV = min(255.0, Yuuyo * 256 - 743);
				font80(U"ラウンド").draw(130, 190, Color(VV, VV, VV, 175));
				font80(Rounds).draw(450, 190, Color(VV, VV, VV, 175));
			}
			if (2 < Yuuyo && Yuuyo <= 3) { Circle(DX, DY, 100).drawPie(0, ToRadians((Yuuyo - 2.00) * 360.0), Color(255, 255, 0, 150)); Circle(DX, DY, 80).drawPie(0, ToRadians((Yuuyo - 2.00) * 360.0), Color(25, 25, 25)); }
			if (1 < Yuuyo && Yuuyo <= 2) { Circle(DX, DY, 100).drawPie(0, ToRadians((Yuuyo - 1.00) * 360.0), Color(255, 100, 0, 150)); Circle(DX, DY, 80).drawPie(0, ToRadians((Yuuyo - 1.00) * 360.0), Color(25, 25, 25)); }
			if (0 < Yuuyo && Yuuyo <= 1) { Circle(DX, DY, 100).drawPie(0, ToRadians((Yuuyo - 0.00) * 360.0), Color(255, 0, 0, 150)); Circle(DX, DY, 80).drawPie(0, ToRadians((Yuuyo - 0.00) * 360.0), Color(25, 25, 25)); }
			if (2 < Yuuyo && Yuuyo <= 3) { font120(U"3").draw(270, 170, Color(255, 255, 0, 150)); }
			if (1 < Yuuyo && Yuuyo <= 2) { font120(U"2").draw(270, 170, Color(255, 255, 0, 150)); }
			if (0 < Yuuyo && Yuuyo <= 1) { font120(U"1").draw(270, 170, Color(255, 255, 0, 150)); }

			if (Yuuyo < 0 && (Rounds == 3 && Times < 10)) {
				double d = Times; while (d >= 1.0) d -= 1.0;
				for (int i = 0; i < 10; i++) {
					Rect(i * 6, i * 6, 600 - i * 12, 550 - i * 12).draw(Color(d * (255 - i * 12) + (1.0 - d) * 25.0, 25, 25));
				}
				Circle(DX, DY, 100).drawPie(0, ToRadians(d * 360.0), Color(255, 0, 0, 150));
				Circle(DX, DY, 80).drawPie(0, ToRadians(d * 360.0), Color(25, 25, 25));
				if (9 < Times && Times <= 10) { font120(U"10").draw(240, 170, Color(255, 0, 0, 150)); }
				if (8 < Times && Times <= 9) { font120(U"9").draw(270, 170, Color(255, 0, 0, 150)); }
				if (7 < Times && Times <= 8) { font120(U"8").draw(270, 170, Color(255, 0, 0, 150)); }
				if (6 < Times && Times <= 7) { font120(U"7").draw(270, 170, Color(255, 0, 0, 150)); }
				if (5 < Times && Times <= 6) { font120(U"6").draw(270, 170, Color(255, 0, 0, 150)); }
				if (4 < Times && Times <= 5) { font120(U"5").draw(270, 170, Color(255, 0, 0, 150)); }
				if (3 < Times && Times <= 4) { font120(U"4").draw(270, 170, Color(255, 0, 0, 150)); }
				if (2 < Times && Times <= 3) { font120(U"3").draw(270, 170, Color(255, 0, 0, 150)); }
				if (1 < Times && Times <= 2) { font120(U"2").draw(270, 170, Color(255, 0, 0, 150)); }
				if (0 < Times && Times <= 1) { font120(U"1").draw(270, 170, Color(255, 0, 0, 150)); }
			}
			else if (Yuuyo < 0 && Times < 3) {
				double d = Times; while (d >= 1.0) d -= 1.0;
				Circle(DX, DY, 100).drawPie(0, ToRadians(d * 360.0), Color(255, 0, 0, 150));
				Circle(DX, DY, 80).drawPie(0, ToRadians(d * 360.0), Color(25, 25, 25));
				if (2 < Times && Times <= 3) { font120(U"3").draw(270, 170, Color(255, 0, 0, 150)); }
				if (1 < Times && Times <= 2) { font120(U"2").draw(270, 170, Color(255, 0, 0, 150)); }
				if (0 < Times && Times <= 1) { font120(U"1").draw(270, 170, Color(255, 0, 0, 150)); }
			}

			// 球の表示
			if (Rounds <= 3 && Yuuyo < 0) {
				refresh(Scene::DeltaTime());
				for (int i = 0; i < Z4[Rounds - 1]; i++) {
					if (ball2[i].ti < 0 || ball2[i].py >= 620) continue;
					if (Rounds == 2) tex1.scaled(0.6).drawAt(ball2[i].px, ball2[i].py, ColorF(1.0, max(0.0, 0.6 - 0.0004 * ball2[i].flag)));
					if (Rounds == 3) tex1.scaled(0.45).drawAt(ball2[i].px, ball2[i].py, ColorF(1.0, max(0.0, 0.6 - 0.0004 * ball2[i].flag)));
				}
				for (int i = 0; i < Z1[Rounds - 1] + Z2[Rounds - 1] + Z3[Rounds - 1]; i++) {
					if (ball[i].ti < 0 || ball[i].py > 1000) continue;
					for (int j = 3; j >= 1; j--) {
						Circle(ball[i].px - 0.02 * j * ball[i].vx, ball[i].py - 0.02 * j * ball[i].vy, 5).draw(Color(200 - 50 * j, 25, 25, 150));
					}
					if (ball[i].sc == -1) Circle(ball[i].px, ball[i].py, 5).draw(Color(255, 0, 0, max(0, 150 - ball[i].flag / 30)));
					if (ball[i].sc == 1) Circle(ball[i].px, ball[i].py, 5).draw(Color(255, 255, 0, max(0, 150 - ball[i].flag / 30)));
					if (ball[i].sc == 3) Circle(ball[i].px, ball[i].py, 5).draw(Color(0, 255, 0, max(0, 150 - ball[i].flag / 30)));
				}
			}
			for (int i = 0; i < tup.size(); i++) {
				Circle(get<0>(tup[i]), get<1>(tup[i]), 30.0 * get<2>(tup[i])).draw(Color(0, 255, 80, 60.0 - 120.0 * get<2>(tup[i])));
			}
			for (int i = 0; i < tensuu.size(); i++) {
				if (get<0>(tensuu[i]) >= 10) {
					font18(U"+").draw(get<1>(tensuu[i]) - 25.0, get<2>(tensuu[i]) - 50.0 * get<3>(tensuu[i]) - 9.0, ColorF(0.0, 1.0, 0.0, 1.0 - 1.5 * get<3>(tensuu[i])));
					font18(U"点").draw(get<1>(tensuu[i]) + 5.0, get<2>(tensuu[i]) - 50.0 * get<3>(tensuu[i]) - 9.0, ColorF(0.0, 1.0, 0.0, 1.0 - 1.5 * get<3>(tensuu[i])));
					font18(get<0>(tensuu[i])).draw(get<1>(tensuu[i]) - 15.0, get<2>(tensuu[i]) - 50.0 * get<3>(tensuu[i]) - 9.0, ColorF(0.0, 1.0, 0.0, 1.0 - 1.5 * get<3>(tensuu[i])));
				}
				else if (get<0>(tensuu[i]) > 0) {
					font18(U"+").draw(get<1>(tensuu[i]) - 20.0, get<2>(tensuu[i]) - 50.0 * get<3>(tensuu[i]) - 9.0, ColorF(0.0, 1.0, 0.0, 1.0 - 1.5 * get<3>(tensuu[i])));
					font18(U"点").draw(get<1>(tensuu[i]) + 0.0, get<2>(tensuu[i]) - 50.0 * get<3>(tensuu[i]) - 9.0, ColorF(0.0, 1.0, 0.0, 1.0 - 1.5 * get<3>(tensuu[i])));
					font18(get<0>(tensuu[i])).draw(get<1>(tensuu[i]) - 10.0, get<2>(tensuu[i]) - 50.0 * get<3>(tensuu[i]) - 9.0, ColorF(0.0, 1.0, 0.0, 1.0 - 1.5 * get<3>(tensuu[i])));
				}
				else if (get<0>(tensuu[i]) < 0) {
					font18(U"-").draw(get<1>(tensuu[i]) - 20.0, get<2>(tensuu[i]) - 50.0 * get<3>(tensuu[i]) - 9.0, ColorF(1.0, 0.0, 0.0, 1.0 - 1.5 * get<3>(tensuu[i])));
					font18(U"点").draw(get<1>(tensuu[i]) + 0.0, get<2>(tensuu[i]) - 50.0 * get<3>(tensuu[i]) - 9.0, ColorF(1.0, 0.0, 0.0, 1.0 - 1.5 * get<3>(tensuu[i])));
					font18(-get<0>(tensuu[i])).draw(get<1>(tensuu[i]) - 10.0, get<2>(tensuu[i]) - 50.0 * get<3>(tensuu[i]) - 9.0, ColorF(1.0, 0.0, 0.0, 1.0 - 1.5 * get<3>(tensuu[i])));
				}
			}

			// マウスの表示
			if (Rounds <= 3) {
				double wx = MI.px, wy = MI.py;
				MI.px = MouseX; MI.px = max(0.0, MI.px); MI.px = min(600.0, MI.px);
				MI.py = MouseY; MI.py = max(0.0, MI.py); MI.py = min(600.0, MI.py);
				MI.vx = 0.6 * (MI.px - wx) / Scene::DeltaTime();
				MI.vy = 0.6 * (MI.py - wy) / Scene::DeltaTime();
				Circle(MI.px, MI.py, 25).draw(Color(255, 255, 255));
			}

			// 当たり判定
			if (Rounds <= 3) {
				atari(Scene::DeltaTime());
			}

			// 風船の当たり判定
			for (int i = 0; i < Z4[Rounds - 1]; i++) {
				if (ball2[i].flag >= 1) ball2[i].flag += 10000.0 * Scene::DeltaTime();
				if (ball2[i].flag >= 1 || ball2[i].ti < 0 || ball2[i].py > 620) continue;
				for (int j = 0; j < Z1[Rounds - 1] + Z2[Rounds - 1] + Z3[Rounds - 1]; j++) {
					if (ball[j].px < -5 || ball[j].px > 605 || ball[j].py < -5 || ball[j].py > 605) continue;
					double gx = ball[j].px - ball2[i].px;
					double gy = ball[j].py - ball2[i].py;

					double threshold = 2100.0; if (Rounds == 3) threshold = 1250.0;
					if (gx * gx + gy * gy <= threshold) {
						tensuu.push_back(make_tuple(ball2[i].sc, ball[j].px, ball[j].py, 0));
						ball2[i].flag = 2;
						Score[Rounds - 1] += ball2[i].sc;
						Score[Rounds - 1] = max(Score[Rounds - 1], 0);
						Score[Rounds - 1] = min(Score[Rounds - 1], 100);
					}
				}
			}

			// スコア判定
			if (Rounds <= 3 && Yuuyo < 0) {
				for (int i = 0; i < Z1[Rounds - 1] + Z2[Rounds - 1] + Z3[Rounds - 1]; i++) {
					if (ball[i].py >= 550.0 && ball[i].flag == 0) {
						ball[i].flag = 1; int sums = 0; pair<int, int> maxs = make_pair(0, -1);

						if (ball[i].px >= 0 && ball[i].px <= 600) {
							double I = ball[i].px - ZURE; if (I < 0) I += 600.0;
							for (int j = 0; j < 7; j++) {
								int cl = sums, cr = sums + Mass[Rounds - 1][j];
								if (cl <= (int)I && (int)I <= cr) maxs = max(maxs, make_pair(Pts[Rounds - 1][j], j));
								sums += Mass[Rounds - 1][j];
							}
						}

						if (maxs.second != -1) {
							tensuu.push_back(make_tuple(maxs.first * ball[i].sc, ball[i].px, ball[i].py, 0));
							Score[Rounds - 1] += maxs.first * ball[i].sc;
							Score[Rounds - 1] = max(Score[Rounds - 1], 0);
							Score[Rounds - 1] = min(Score[Rounds - 1], 100);
							shisuu[maxs.second] = 0.9;
						}
					}
					if (ball[i].flag >= 1) ball[i].flag += (int)(10000.0 * Scene::DeltaTime());
				}
			}
			for (int i = 0; i < 7; i++) shisuu[i] = max(0.17, shisuu[i] - Scene::DeltaTime());

			// 下のバーの表示
			if (Rounds <= 3 && (Rounds >= 2 || Yuuyo < 3)) {
				int lens = 50, cp = Rounds;
				if (Yuuyo > 0) { lens = 50 - 50.0 * min(Yuuyo, 5.0 - Yuuyo); if (Yuuyo > 3) cp--; }
				if (Rounds == 3 && Times <= 1.0) { lens = 50.0 * Times; }
				lens = max(lens, 0); cp = max(cp, 1);

				if (cp <= 3) {
					int sums = 0;
					for (int i = 0; i < 7; i++) {
						if (Mass[cp - 1][i] == 0) continue;
						
						int median = ZURE + (sums + Mass[cp - 1][i] / 2);
						int median2 = (ZURE - 600.0) + (sums + Mass[cp - 1][i] / 2);
						Rect((ZURE - 600.0) + sums, 600 - lens, Mass[cp - 1][i], lens).draw(ColorF(1.0 * MA[i] / 255.0, 1.0 * MB[i] / 255.0, 1.0 * MC[i] / 255.0, shisuu[i]));
						Rect(ZURE + sums, 600 - lens, Mass[cp - 1][i], lens).draw(ColorF(1.0 * MA[i] / 255.0, 1.0 * MB[i] / 255.0, 1.0 * MC[i] / 255.0, shisuu[i]));
						
						font21(Pts[cp - 1][i]).draw(median - 12 - 6 * to_string(Pts[cp - 1][i]).size(), 611 - lens, Color(255, 255, 255));
						font21(U"点").draw(median - 12 + 6 * to_string(Pts[cp - 1][i]).size(), 611 - lens, Color(255, 255, 255));
						
						font21(Pts[cp - 1][i]).draw(median2 - 12 - 6 * to_string(Pts[cp - 1][i]).size(), 611 - lens, Color(255, 255, 255));
						font21(U"点").draw(median2 - 12 + 6 * to_string(Pts[cp - 1][i]).size(), 611 - lens, Color(255, 255, 255));

						sums += Mass[cp - 1][i];
					}
				}
			}

			// 最終結果の表示
			if (Rounds == 4) {
				font44(U"得点").draw(60, 60);
				font44(U"点").draw(490, 195);
				font120(Score[0] + Score[1] + Score[2]).draw(450 - 65 * to_string(Score[0] + Score[1] + Score[2]).size(), 110);

				font44(U"ランク").draw(60, 300);
				String W = GetRankToScore(Score[0] + Score[1] + Score[2]);
				if (W.size() <= 7) font120(W).draw(300 - 33 * W.size(), 350);
				else font80(W).draw(300 - 22 * W.size(), 350);

				font28(U"マウス左クリックで順位表へ").draw(105, 527, ColorF(Periodic::Sine0_1(1s) * 0.9 + 0.1, 0.1, 0.1));
				if (Scene::Time() - GetLastClick >= 0.07 && MouseL.down() == true) {
					// 情報を入手
					string X = GetTime();
					get<0>(HighScore[NumberOfPlayers]) = Score[0] + Score[1] + Score[2];
					get<1>(HighScore[NumberOfPlayers]) = Score[0];
					get<2>(HighScore[NumberOfPlayers]) = Score[1];
					get<3>(HighScore[NumberOfPlayers]) = Score[2];
					get<4>(HighScore[NumberOfPlayers]) = X.substr(0, 5);
					get<5>(HighScore[NumberOfPlayers]) = X.substr(6, 8);
					MyRank = 0;
					for (int j = 0; j < NumberOfPlayers; j++) {
						if (HighScore[j] > HighScore[NumberOfPlayers]) MyRank++;
					}
					NumberOfPlayers++;
					sort(HighScore, HighScore + NumberOfPlayers);
					reverse(HighScore, HighScore + NumberOfPlayers);

					// 成績の書き込み
					FILE* out = freopen("ranking.txt", "w", stdout);
					cout << NumberOfPlayers << endl;
					for (int j = 0; j < NumberOfPlayers; j++) {
						cout << get<0>(HighScore[j]) << " " << get<1>(HighScore[j]) << " " << get<2>(HighScore[j]) << " ";
						cout << get<3>(HighScore[j]) << " " << get<4>(HighScore[j]) << " " << get<5>(HighScore[j]) << endl;
					}

					GetLastClick = Scene::Time();
					Situation = 5;
				}
			}

			// 画面右側
			Rect(600, 0, 200, 80).draw(Color(0, 0, 0));
			Rect(600, 80, 200, 520).draw(Color(10, 10, 10));
			font34(U"ステータス").draw(614, 16, Color(255, 255, 255));

			font21(U"ラウンド").draw(607, 100, Color(255, 255, 255));
			font21(U"残り").draw(607, 230, Color(255, 255, 255));
			font21(U"得点").draw(607, 360, Color(255, 255, 255));
			font18(U"秒").draw(775, 281, Color(255, 255, 255));
			font18(U"点").draw(775, 411, Color(255, 255, 255));
			font44(min(Rounds, 3)).draw(737, 123, Color(255, 255, 255));
			font44((int)(Times + 0.999)).draw(762 - 25 * to_string((int)(Times + 0.999)).size(), 253, Color(255, 255, 255));
			font44(Score[0] + Score[1] + Score[2]).draw(762 - 25 * to_string(Score[0] + Score[1] + Score[2]).size(), 383, Color(255, 255, 255));

			Color col1 = Color(0, 255, 0);
			if (Rounds == 2) col1 = Color(255, 255, 0);
			if (Rounds >= 3) col1 = Color(255, 0, 0);
			Rect(608, 179, 184, 18).draw(Color(255, 255, 255));
			Rect(610, 181, 180, 14).draw(Color(10, 10, 10));
			Rect(610, 181, 180 * min(Rounds, 3) / 3, 14).draw(col1);

			Color col2 = Color(0, 255, 0);
			if (Times / Limit[Rounds - 1] <= 1.0 / 3.0) col2 = Color(255, 255, 0);
			if (Times / Limit[Rounds - 1] <= 1.0 / 6.0) col2 = Color(255, 0, 0);
			Rect(608, 309, 184, 18).draw(Color(255, 255, 255));
			Rect(610, 311, 180, 14).draw(Color(10, 10, 10));
			if (Times >= 0) {
				if (Times / Limit[Rounds - 1] <= 1.0 / 6.0) Rect(610, 311, 180 * Times / Limit[Rounds - 1], 14).draw(ColorF(1.0, 0.0, 0.0, Periodic::Sine0_1(1s)));
				else Rect(610, 311, 180 * Times / Limit[Rounds - 1], 14).draw(col2);
			}

			Rect(608, 439, 184, 18).draw(Color(255, 255, 255));
			Rect(610, 441, 180, 14).draw(Color(10, 10, 10));
			for (int i = 0; i < 3; i++) {
				int TotalScore = Score[0] + Score[1] + Score[2];
				if (TotalScore < 100 * i) continue;

				int rem = min(100, TotalScore - 100 * i);
				double I = 1.0; if (TotalScore >= 200) I = Periodic::Sine0_1(1s);
				Color col3 = Color(255, 255, 0, 255.0 * I);
				if (i == 1) col3 = Color(255, 0, 0, 255.0 * I);
				if (i == 2) col3 = Color(136, 0, 255, 255.0 * I);
				Rect(610, 441, 180 * rem / 100, 14).draw(col3);
			}

			font21(U"ラウンド1: ").draw(607, 500); font18(U"点").draw(775, 500);
			font21(U"ラウンド2: ").draw(607, 525); font18(U"点").draw(775, 525);
			font21(U"ラウンド3: ").draw(607, 550); font18(U"点").draw(775, 550);
			if (Rounds <= 1) font21(U"??").draw(740, 500); else font21(Score[0]).draw(764 - 12 * to_string(Score[0]).size(), 500);
			if (Rounds <= 2) font21(U"??").draw(740, 525); else font21(Score[1]).draw(764 - 12 * to_string(Score[1]).size(), 525);
			if (Rounds <= 3) font21(U"??").draw(740, 550); else font21(Score[2]).draw(764 - 12 * to_string(Score[2]).size(), 550);

			// 時間調整
			if (Rounds <= 3) {
				if (Yuuyo > 0) Yuuyo -= Scene::DeltaTime();
				else Times -= Scene::DeltaTime();
				if (Times < 0) {
					tup.clear(); tensuu.clear();
					Rounds += 1;
					Times += Limit[Rounds - 1];
					Yuuyo = 5.0;
					if (Rounds <= 3) initballs();
				}
			}
		}

		// 状態 3 : ルール説明画面
		if (Situation == 3) {
			font80(U"ルール説明").draw(200, 15);
			Rect(28, 158, 364, 274).draw(Color(255, 255, 255));
			Rect(30, 160, 360, 270).draw(Color(0, 0, 0));

			int dx[13] = { 60,  64,  68,  72,  87, 102, 117, 132, 147, 162, 178, 194, 210 };
			int dy[13] = { 160, 170, 190, 220, 200, 190, 190, 200, 220, 250, 290, 340, 390 };
			for (int i = 0; i < 13; i++) {
				Color des = Color(100, 100, 0); if (i == 0) des = Color(255, 255, 0);
				Circle(dx[i], dy[i] + 20, 5).draw(des);
			}
			Circle(66, 259, 15).draw(Color(255, 255, 255));

			Rect(30, 400, 90, 30).draw(ColorF(0.0, 0.0, 1.0, 0.5));
			Rect(120, 400, 60, 30).draw(ColorF(1.0, 1.0, 0.0, 0.5));
			Rect(180, 400, 60, 30).draw(ColorF(1.0, 0.0, 0.0, 0.5));
			Rect(240, 400, 60, 30).draw(ColorF(1.0, 1.0, 0.0, 0.5));
			Rect(300, 400, 90, 30).draw(ColorF(0.0, 0.0, 1.0, 0.5));

			font18(U"2点").draw(60, 403, Color(255, 255, 255));
			font18(U"5点").draw(135, 403, Color(255, 255, 255));
			font18(U"10点").draw(190, 403, Color(255, 255, 255));
			font18(U"5点").draw(255, 403, Color(255, 255, 255));
			font18(U"2点").draw(330, 403, Color(255, 255, 255));

			Line(66, 259, 250, 239).draw(Color(255, 255, 255));
			font18(U"ここを").draw(255, 228, Color(255, 255, 255));
			font18(U"うごかします").draw(255, 252, Color(255, 255, 255));
			Line(194, 360, 270, 330).draw(Color(255, 255, 255));
			font18(U"たまが").draw(275, 320, Color(255, 255, 255));
			font18(U"おちてきます").draw(275, 344, Color(255, 255, 255));

			font21(U"① あなたは、白色の丸い板をマウス").draw(430, 165);
			font21(U"で動かすことができます。").draw(456, 190);
			font21(U"② 上から球が落ちてくるので、板を").draw(430, 230);
			font21(U"上手く操作して、球を高い点数").draw(456, 255);
			font21(U"のところに落としてください。").draw(456, 280);
			font21(U"③ 　　・　　を入れると得点が増え、").draw(430, 320); font21(U"黄球").draw(456, 320, ColorF(1.0, 1.0, 0.0)); font21(U"緑球").draw(519, 320, ColorF(0.0, 1.0, 0.0));
			font21(U"　　を入れると得点が減ります。").draw(456, 345); font21(U"赤球").draw(456, 345, ColorF(1.0, 0.0, 0.0));
			font21(U"④ 300 点満点目指して頑張ろう！").draw(430, 385);
			font21(U"あなたは何点取れるかな？").draw(456, 410);

			font28(U"マウス左クリックで戻る").draw(220, 527, ColorF(Periodic::Sine0_1(1s)));
			if (Scene::Time() - GetLastClick >= 0.07 && MouseL.down() == true) {
				GetLastClick = Scene::Time();
				Situation = 1;
			}
		}

		// 状態 4 : ランキング画面
		if (Situation == 4 || Situation == 5) {
			int ends = 447; if (Situation == 5 && MyRank >= 10) ends = 480;

			font80(U"ランキング").draw(200, 15);
			font21(U"順位").draw(61, 140, Color(255, 255, 0));
			font21(U"得点").draw(207, 140, Color(255, 255, 0));
			font21(U"ランク").draw(414, 140, Color(255, 255, 0));
			font21(U"プレイ時刻").draw(595, 140, Color(255, 255, 0));
			Line(54, 169, 746, 169).draw(3, Color(255, 255, 0));
			Line(110, 140, 110, ends).draw(1, Color(255, 255, 255));
			Line(350, 140, 350, ends).draw(1, Color(255, 255, 255));
			Line(550, 140, 550, ends).draw(1, Color(255, 255, 255));

			for (int i = 0; i < 11; i++) {
				if (i == 10 && (MyRank < 10 || Situation == 4)) break;
				int ranks = i; if (i == 10) ranks = MyRank;

				int ID[5] = { ranks + 1, get<0>(HighScore[ranks]), get<1>(HighScore[ranks]), get<2>(HighScore[ranks]), get<3>(HighScore[ranks]) };
				int posx[7] = { 85, 317, 143, 201, 259, 450, 645 };

				int zahyou = 174 + i * 27; if (i == 10) zahyou = 453;
				Color MyColor = Color(255, 255, 255); if (Situation == 5 && MyRank == ranks) MyColor = Color(255, 0, 0);
				for (int j = 0; j < 5; j++) {
					font21(ID[j]).draw(posx[j] - 6 * to_string(ID[j]).size(), zahyou, MyColor);
				}
				font21(GetRankToScore(get<0>(HighScore[ranks]))).draw(posx[5] - 6 * GetRankToScore(get<0>(HighScore[ranks])).size(), zahyou, MyColor);

				string V = get<4>(HighScore[ranks]) + " " + get<5>(HighScore[ranks]);
				for (int j = 0; j < 14; j++) {
					int rem = 0;
					if ('0' <= V[j] && V[j] <= '9') rem = (V[j] - '0');
					if (V[j] == '-') rem = 10;
					if (V[j] == ':') rem = 11;
					if (V[j] == ' ') rem = 12;
					font21(GetChar[rem]).draw(posx[6] - 6 * V.size() + 12 * j, zahyou, MyColor);
				}
				font21(U"+").draw(167, zahyou, MyColor);
				font21(U"+").draw(224, zahyou, MyColor);
				font21(U"=").draw(281, zahyou, MyColor);
			}
			if (Situation == 5) {
				if (MyRank >= 10) Line(54, 450, 746, 450).draw(2, Color(255, 0, 0));
				if (MyRank < 10) font28(U"ランクインおめでとうございます！").draw(180, 452, Color(255, 0, 0));
				font12(U"現在のプレイ者数：").draw(640, 490, Color(150, 150, 150));
				font12(NumberOfPlayers).draw(740, 490, Color(150, 150, 150));
			}

			if (Situation == 4) font28(U"マウス左クリックで戻る").draw(220, 527, ColorF(Periodic::Sine0_1(1s)));
			if (Situation == 5) font28(U"マウス左クリックで終了").draw(220, 527, ColorF(Periodic::Sine0_1(1s)));
			if (Scene::Time() - GetLastClick >= 0.07 && MouseL.down() == true) {
				GetLastClick = Scene::Time();
				if (Situation == 5) { all_init(); }
				Situation = 1;
			}
		}
	}
}