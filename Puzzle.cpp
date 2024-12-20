#include "Puzzle.h"
#include<assert.h>

static const int R = 50;//半径（値は書かない）
static const int OFF_X = 200;
static const int OFF_Y = 100;
static const int WIDTH = 6;
static const int HEIGHT = 5;


static const unsigned int COLORS[] =
{
	GetColor(255,0,0),//赤
	GetColor(0,255,0),//緑
	GetColor(0,0,255),//青
	GetColor(255,255,0)//黄色

};

static const int ColorNum = sizeof(COLORS) / sizeof(COLORS[0]);

Puzzle::Puzzle(GameObject* parent)
{
	//乱数で並べる
	//cの場合
	/*for (int y = 0; y<5; y++)
	{
		for (int x = 0; x < 6; x++)
		{
			field[y][x].color = rand() % ColorNum;
		}
	}*/



	for (int y = 0; y < HEIGHT; y++)
	{
		std::vector<Piece> tmp;
		for (int x = 0; x < WIDTH; x++)
		{
			Piece p;
			p.color = rand() % ColorNum;
			p.erase = 0;
			p.fallAdjust = 0;
			//p.selected = false;
			tmp.push_back(p);
		}
		field.push_back(tmp);
	}
	selX = -1;
	selY = -1;
	state = S_IDLE;

	soundErase = LoadSoundMem("Assets/Onoma-Flash10-4(High-2).mp3");
	assert(soundErase > 0);
	soundFall = LoadSoundMem("Assets/Onoma-Flash10-4(High-2).mp3");//後で適切なやついれる
	assert(soundFall > 0);

}

Puzzle::~Puzzle()
{
	if (soundErase > 0) {
		DeleteSoundMem(soundErase);
	}
	if (soundFall > 0) {
		DeleteSoundMem(soundFall);
	}
}

void Puzzle::Update()
{
	switch (state) {
	case S_IDLE:
		UpdateIdle();
		break;
	case S_MOVE:
		UpdateMove();
		break;
	case S_ERASE:
		UpdateErase();
		break;
	case S_FALL:
		UpdateFall();
		break;
	case S_ATTACK:
		UpdateAttack();
		break;
	default:
		assert(false);//ここに来るのは本来おかしい
		break;
	}
}
void Puzzle::UpdateIdle(){

	//field[0][0].selected = true;
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0){//マウスの左クリックを押した
		if (lastButton == false) {
			
			GetMousePoint(&mouseX, &mouseY);//マウスポインタの座標が入る
			for (int y = 0; y < HEIGHT; y++) {
				for (int x = 0; x < WIDTH; x++) {
					int cx = x * R * 2 + OFF_X;
					int cy = y * R * 2 + OFF_Y;
					if (mouseX >= cx - R && mouseX < cx + R &&//横幅±50
						mouseY >= cy - R && mouseY < cy + R) {//縦幅±50
						selX = x;
						selY = y;
						state = S_MOVE;
					}
				}
			}
		}
		lastButton = true;//memo 判定より後で代入すると、ひとつ前をとっておく
	}
	else {
		lastButton = false;
	}
	//ToDo きれいに消す
}


void Puzzle::Draw()
{
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			int xpos = x * R * 2 + OFF_X;
			int ypos = y * R * 2 + OFF_Y + field[y][x].fallAdjust ;
			bool alphaSet = false;
			if (x == selX && y == selY) {
				alphaSet = true;
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);//半透明にする(玉を動かすとき)
			}
			/*if (state == S_ERASE && eraseTimer < 0.1f) { //複雑すぎる
				if (field[y][x].erase > 0) {
					SetDrawBlendMode(DX_BLENDMODE_ALPHA,( 0.1f - eraseTimer / 0.1f) * 255);
				}
			}*/
			if (state == S_ERASE && field[y][x].erase > 0)//玉が消えるとき
			{
				alphaSet = true;
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, eraseAlpha);
			}
			DrawCircle(xpos, ypos, R, GetColor(255, 255, 255));
			int r = field[y][x].color;
			DrawCircle(xpos, ypos, (R-4), COLORS[r]);
			if (alphaSet) {
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//元に戻す
			}
			DrawFormatString(xpos, ypos,
				GetColor(255, 255, 255),"%d",field[y][x].erase);
		}
	}

	if (selX >= 0){
		DrawCircle(mouseX, mouseY, R + 4, GetColor(255, 255, 255));
		int r = field[selY][selX].color;
		DrawCircle(mouseX, mouseY, R,COLORS[r]);
	}
	//つまんだ場合の表示
	//DrawCircle(mouseX, mouseY, R + 4, GetColor(255, 255, 255));
	//int r = field[selY][selX].color;
	//DrawCircle(mouseX, mouseY, R, COLORS[r]);


	GetMousePoint(&mouseX, &mouseY);
#ifdef _DEBUG //開発中はデバッグ　提出時はリリース

	DrawFormatString(0, 0, GetColor(255, 255, 255), "%d %d",
		mouseX,mouseY);
#endif
}
void Puzzle::UpdateMove()
{
	GetMousePoint(&mouseX, &mouseY);
	//入れ替えるか判定
	//今のマウスが指している玉(newX newY)を特定
	
	int newX = -1;
	int newY = -1;

	//マウスが触れた玉を探す
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			int cx = x * R * 2 + OFF_X;
			int cy = y * R * 2 + OFF_Y;
			//範囲内なのでここをカーソルとする
			if (mouseX >= cx - R && mouseX < cx + R &&//横幅±50
				mouseY >= cy - R && mouseY < cy + R) {//縦幅±50
				newX = x;
				newY = y;
				
			}
		}
	}
	//これが、selX,selYと違うのであれば入れ替える
	if (newX >= 0 && (newX != selX || newY != selY)) {
		//ToDo field[newY][newX]とfield[selY][selX]を入れ替える
		auto tmp = field[newY][newX];
		field[newY][newX] = field[selY][selX];
		field[selY][selX] = tmp;
		selX = newX;
		selY = newY;
	}

	if ((GetMouseInput() & MOUSE_INPUT_LEFT) == 0)//離した
	{
		selX = -1;
		
		if (CanErase()) {//三つ以上並んだブロックがあるなら
			eraseTimer = 0;
			PlaySoundMem(soundErase, DX_PLAYTYPE_BACK);
			state = S_ERASE;
		}
		else {
			state = S_IDLE;
		}
		
	}
}

void Puzzle::UpdateErase()
{
	
	eraseTimer += 1.0f / 60.0f;//erasetimer < 0.1f までにきえる
	
	eraseAlpha = 255 + -255 * eraseTimer / 0.1f;//255からだんだん0へ
	//今の値＝初期値＋変化量* 今の時間 / 最大時間
	if (eraseAlpha < 0.0f)
		eraseAlpha = 0.0f;
	if (eraseTimer > 1.0f) {
		PrepareFall();
		state = S_FALL;//落ちる
		fallSoundRequested = false;
	}
	
}

void Puzzle::UpdateFall()
{
	bool fallEnd = true;//全部が落ちた

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			//fallAdjust <0　であれば徐々に増やす
			Piece& c = field[y][x];
			if (c.fallAdjust < 0) {
				fallEnd = false;
				c.fallAdjust += 5.0f;
				if (c.fallAdjust > 0) {
					c.fallAdjust = 0;
				}
			}
			
		}
	}
	//全員が○○という条件は「一人でも偽だったら」と考える
	//state = S_IDLE;
	if (fallEnd) {//全部が落ち切った
		if (fallSoundRequested == false){
			PlaySoundMem(soundFall, DX_PLAYTYPE_BACK);
			fallSoundRequested = true;
		}
		else {
			if (CheckSoundMem(soundFall) == 0) {
				if (CanErase())
				{//三つ以上並んだブロックがあるなら

					eraseTimer = 0;
					PlaySoundMem(soundErase, DX_PLAYTYPE_BACK);
					state = S_ERASE;
				}
				else {
					state = S_IDLE;//eraseを作ってないので
				}
			}
		}
	}
}

void Puzzle::UpdateAttack()
{
}

bool Puzzle::CanErase()
{
	eraseTimer += 1.0f / 60.0f;


	//全部のPiece:eraseを0にする
	//field[0][0]から横に３つ並んでいる→colorの値が同じか？
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			field[y][x].erase = 0;
			/*field[y][x].erase1 = 0;
			field[y][x].erase2 = 0;
			field[y][x].erase3 = 0;
			field[y][x].erase4 = 0;*/
		}
	}

	/*if (field[0][2 +0].color == field[0][2 +1].color && field[0][2 + 0].color == field[0][2+2].color)
	{
		field[0][2 + 0].erase = 1;
		field[0][2 + 1].erase = 1;
		field[0][2 + 2].erase = 1;
	}*/
	
	//３つ並んでるなら、そこのeraseを1にする
	int eraseNumber = 1;
	for (int y = 0; y < HEIGHT; y++) {//横に並んだものを消す
		for (int x = 0; x < WIDTH-2; x++) {
			if (field[y][x].color == field[y][x + 1].color && field[y][x].color == field[y][x + 2].color)
			{
				/*switch (field[y][x].color)
				{
				case 0:
					field[y][x].erase1 = 1;
					field[y][x + 1].erase1 = 1;
					field[y][x + 2].erase1 = 1;
					break;
				case 1:
					field[y][x].erase2 = 1;
					field[y][x + 1].erase2 = 1;
					field[y][x + 2].erase2 = 1;
					break;
				case 2:
					field[y][x].erase3 = 1;
					field[y][x + 1].erase3 = 1;
					field[y][x + 2].erase3 = 1;
					break;
				case 3:
					field[y][x].erase4 = 1;
					field[y][x + 1].erase4 = 1;
					field[y][x + 2].erase4 = 1;
					break;
				default:
					break;
				}*/

				field[y][x + 0].erase = eraseNumber;
				field[y][x + 1].erase = eraseNumber;
				field[y][x + 2].erase = eraseNumber;
				eraseNumber++;
			}
		}
	}

	for (int y = 0; y < HEIGHT-2; y++) {//縦に並んだものを消す
		for (int x = 0; x < WIDTH; x++) {
			if (field[y][x].color == field[y + 1][x].color && field[y][x].color == field[y + 2][x].color)

			{
				/*switch (field[y][x].color)
				{
				case 0:
					field[y][x].erase1 = 1;
					field[y + 1][x].erase1 = 1;
					field[y + 2][x].erase1 = 1;
					break;
				case 1:
					field[y][x].erase2 = 1;
					field[y + 1][x].erase2 = 1;
					field[y + 2][x].erase2 = 1;
					break;
				case 2:
					field[y][x].erase3 = 1;
					field[y + 1][x].erase3 = 1;
					field[y + 2][x].erase3 = 1;
					break;
				case 3:
					field[y][x].erase4 = 1;
					field[y + 1][x].erase4 = 1;
					field[y + 2][x].erase4 = 1;
					break;
				default:
					break;
				}*/
				field[y + 0][x].erase = eraseNumber;
				field[y + 1][x].erase = eraseNumber;
				field[y + 2][x].erase = eraseNumber;
				eraseNumber++;
			}
		}
	}
	///３つ以上並んでいるものが一つ以上あればreturn true
	if (eraseNumber > 1)
		return true;
	return false;
}

void Puzzle::PrepareFall()
{
	//落とす処理　縦の二個め以降は２つ落ちるようになる
	for (int x = 0; x < WIDTH; x++) {
		int erased = 0;
		for (int y = HEIGHT - 1; y >= 0; y--) {//いつもと逆順
			if (field[y][x].erase > 0) {
				erased++;//消えてる数
			}
			else {//消えてないピース
				field[y + erased][x] = field[y][x];//erasedの数だけ落ちる
				field[y + erased][x].fallAdjust = -R * 2 * erased;//falladjustに代入する
			}
		}
		//落ちた空間に新しいピースを補充
		for (int y = 0; y < erased; y++) {
			field[y][x].color = rand() % ColorNum;
			field[y][x].erase = 0;
			field[y][x].fallAdjust = -R * 2 * erased;
		}
	}
}
