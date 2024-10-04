#include "STAGE.h"
#include"GAME10_GAME.h"
#include"CONTAINER.h"
#include "../../libOne/inc/libOne.h"
#include"GAME10_GAME.h"
#include"HANDGUN_BULLETS.h"
#include"WEAPONS.h"
STAGE::STAGE(GAME10_GAME* game) :SCENE(game) {}
STAGE::~STAGE() {
}
void STAGE::init() {
	Stage = game()->container()->stage();
	game()->player()->init();
	game()->time()->init();
	game()->handgunBullets()->init();
}
void STAGE::goalStage() {
	Stage.bworldX = NULL;
	Stage.fworldX = NULL;
	game()->handgunBullets()->AllKill();//弾丸を全部消す
	game()->player()->stageGoal();
}
void STAGE::update() {
	//ステージの強制スクロール（スピードはプレイヤーのスピードの依存する）
	if (Stage.gPos.x - Stage.fworldX > width / 2 
		&& (int)game()->player()->playerData().Opos.x == (int)game()->player()->playerData().Pos.x) {
		Stage.fworldX += game()->player()->playerData().speed;
		if (game()->player()->playerData().speed >= Stage.backMx) {
			Stage.bworldX += game()->player()->playerData().speed - Stage.backMx;
		}
		game()->time()->timeCount();//強制スクロール中のみ時間が進む
	}
	//ゴールテープが画面の中央に来た時、プレイヤーだけ動くようにするために少しだけプレイヤーを動かす。
	else if(game()->player()->playerData().Opos.x == game()->player()->playerData().Pos.x){
		game()->player()->playerMove();
	}

	//プレイヤーがgoalしたときの処理
	if (game()->player()->playerData().Pos.x > width) {
		goalStage();
	}
	game()->player()->update();
	game()->handgunBullets()->update();
}
void STAGE::create() {
	Stage = game()->container()->stage();
	game()->player()->create();
	game()->PlayerHp_gauge()->create();
}
void STAGE::draw() {
	clear(255);
	printSize(200);
	image(Stage.stageImg, Stage.imgPos.x, Stage.imgPos.y);
	for (int i = 0; i < Stage.bImgNum; i++) {
		image(Stage.backImg, (Stage.backPos.x + Stage.ImgLen * i) - Stage.bworldX, Stage.backPos.y);
	}
	for (int i = 0; i < Stage.fImgNum; i++) {
		image(Stage.frontImg, (Stage.frontPos.x + Stage.ImgLen * i) - Stage.fworldX, Stage.frontPos.y);
	}
	for (int j = 0; j < Stage.LaneLenNum; j++) {
		for (int i = 0; i < Stage.LaneNum; i++) {
			image(Stage.Lanes[i].LaneImg, (Stage.Lanes[i].Pos.x + width * j) - Stage.fworldX, Stage.Lanes[i].Pos.y);
		}
	}
	image(Stage.GoalImg, Stage.gPos.x - Stage.fworldX, Stage.gPos.y);
	game()->player()->draw();
	game()->time()->draw();
	game()->handgunBullets()->draw();
	fill(255);
}
void STAGE::nextScene() {
	if (game()->time()->nowTime() <= NULL) {
		game()->changeScene(GAME10_GAME::RESULT_ID);
	}
}
