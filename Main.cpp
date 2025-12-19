#include <iostream>
#include <stdlib.h>
#include <string.h>	
#include <time.h>
#include <conio.h>
using namespace std;

#define SPELL_COST (3)		//呪文の消費MP
#define MAP_WIDTH (16)		//マップの幅
#define MAP_HEIGHT (16)		//マップの高さ
#define SCREEN_WIDTH (16)	//スクロール画面の幅
#define SCREEN_HEIGHT (12)	//スクロール画面の高さ	


//モンスターの種類の定義
enum {
	MONSTER_PLAYER,		//ゆうしゃ
	MONSTER_SLIME,		//すらいむ
	MONSTER_SHEEPN,		//ひつじん
	MONSTER_BEAST,		//けもけも
	MONSTER_DEMON,		//悪魔
	MONSTER_GHOST,		//お化け
	MONSTER_ZOMBIE,		//もうじゃ
	MONSTER_MOCHI,		//もちもん
	MONSTER_ALLIGATOR,	//げいたー
	MONSTER_CACTUS,		//ぼっさん
	MONSTER_BALANCE,	//やじろん
	MONSTER_TRAP,		//わななん
	MONSTER_GUARD,		//ばんにん
	MONSTER_BOSS,		//魔王
	MONSTER_MAX			//モンスターの種類の数
};

//キャラクターの種類を定義
enum {
	CHARACTER_PLAYER,		//プレイヤー
	CHARACTER_MONSTER,		//モンスター
	CHARACTER_MAX			//キャラクターの種類の数
};

//戦闘コマンドの種類を定義
enum {
	COMMAND_FIGHT,			//戦う
	COMMAND_SPELL,			//呪文
	COMMAND_RUN,			//逃げる
	COMMAND_MAX				//コマンドの種類の数
};

//呪文の種類を定義
enum {
	SPELL_HEAL,				//回復呪文
	SPELL_BACK,				//戻る
	SPELL_MAX				//呪文の種類の数
};

//通常コマンドの種類を定義
enum {
	NORMAL_COMMAND_BACK,	//戻る
	NORMAL_COMMAND_HEAL,	//回復
	NORMAL_COMMAND_STATES,	//ステータス
	NORMAL_COMMAND_EXP,		//経験値
	NORMAL_COMMAND_SETTING,	//設定
	NORMAL_COMMAND_MAX		//コマンドの種類の数
};

//マップの種類を定義する
enum {
	MAP_FIELD,				//フィールド
	MAP_KING_CASTLE,		//王様の城
	MAP_SMALL_HUT,			//小さい小屋
	MAP_MAZE_FOREST,		//迷いの森
	MAP_ATER_WASTELAND,		//暗黒の荒野
	MAP_BOSS_CASTLE,		//魔王の城
	MAP_MAX					//マップの種類の数
};


//キャラクターの構造体を定義する

typedef struct {
	int hp;					//HP
	int maxHp;				//最大HP
	int mp;					//MP
	int maxMp;				//最大MP
	int attack;				//攻撃力
	char name[4 * 3 + 1];	//名前(4文字*全角の3バイト+文字列終了コード1バイト)
	char aa[256];			//アスキーアート
	int ex;					//経験値
	int level;				//レベル
	int command;			//コマンド
	int target;				//攻撃対象
}CHARACTER;


//モンスターのステータスの配列を宣言する

CHARACTER monsters[MONSTER_MAX] = {
	//ゆうしゃ
	{
		10,				//HP
		10,				//最大HP
		3,				//MP
		3,				//最大MP
		3,				//攻撃力
		"ゆうしゃ",		//名前(4文字*全角の3バイト+文字列終了コード1バイト)
		"勇",			//アスキーアート
		0,				//経験値
		1,				//レベル
	},

	//すらいむ
	{
		3,				//HP
		3,				//最大HP
		0,				//MP
		0,				//最大MP
		2,				//攻撃力
		"すらいむ",		//名前(4文字*全角の3バイト+文字列終了コード1バイト)
		"（・Д・）\n"	//char aa[256] アスキーアート
		"～～～～～",
		1				//経験値
	},

	//ひつじん
	{
		5,				//HP
		5,				//最大HP
		0,				//MP
		0,				//最大MP
		5,				//攻撃力
		"ひつじん",		//名前(4文字*全角の3バイト+文字列終了コード1バイト)
		"＼～～～／\n"	//char aa[256] アスキーアート	
		"（・▼・）",
		2				//経験値
	},

	//けもけも
	{
		10,
		10,
		0,
		0,
		5,
		"けもけも",
		"　∧＿∧　\n"
		"（＞　＜）\n"
		"　＼∨／　",
		3
	},

	//あくま
	{
		15,
		15,
		0,
		0,
		10,
		"あくま",
		"　　△　△\n"
		"ψ（・皿・）",
		4
	},

	//おばけ
	{
		20,
		20,
		0,
		0,
		15,
		"おばけ",
		"　　△　　\n"
		"（◯・◯）",
		8
	},

	//もうじゃ
	{
		30,
		30,
		0,
		0,
		15,
		"もうじゃ",
		"☆∧＿∧☆\n"
		"（￥＃￥）\n"
		"☆★★★☆",
		16
	},

	//もちもん
	{
		40,
		40,
		0,
		0,
		20,
		"もちもん",
		"　　◯　　\n"
		"　（＿）　\n"
		"（・Ｏ・）",
		32
	},

	//げいたー
	{
		100,
		100,
		0,
		0,
		20,
		"げいたー",
		"　　△　△　\n"
		"　／・　・／\n"
		"　＼／＼／／\n"
		"　＼／＼／　　",
		64
	},

	//ぼっさん
	{
		60,
		60,
		0,
		0,
		30,
		"ぼっさん",
		"∧∧∧∧∧\n"
		"＜０　０＞\n"
		"＜　◆　＞\n"
		"∨∨∨∨∨",
		96
	},

	//やじろん
	{
		80,
		80,
		0,
		0,
		35,
		"やじろん",
		"　　　∧＿∧　　　\n"
		"～～｜◯　◯｜～～\n"
		"　　　＼◇／　　　\n"
		"　　　　∨　　　　",
		128
	},

	//わななん
	{
		40,
		40,
		0,
		0,
		30,
		"わななん",
		"～～～～～\n"
		"｜・△・｜\n"
		"～～～～～",
		64
	},

	//ばんにん
	{
		128,
		128,
		0,
		0,
		30,
		"ばんにん",
		"４＼　／　\n"
		"｜◯・◯　\n"
		"｜　◇　　",
		256
	},

	//まおう
	{
	256,
	256,
	0,
	0,
	40,
	"まおう",
	"　　Ａ＠Ａ\n"
	"ψ（▼皿▼）ψ",
	512
	},
};

//キャラクターの配列を宣言する
CHARACTER characters[CHARACTER_MAX];

//コマンドの名前を宣言する
//戦闘時のコマンド名
char commandNames[COMMAND_MAX][4 * 3 + 1] = {
	"たたかう",			//戦う
	"じゅもん",			//呪文
	"にげる"			//逃げる
};

//戦闘時に呪文を選択した際のコマンド名
char spellNames[SPELL_MAX][4 * 3 + 1] = {
	"ひーる",			//回復
	"もどる"			//戻る
};

//通常時のコマンド名
char normalcommandNames[NORMAL_COMMAND_MAX][5 * 3 + 1] = {
	"とじる",			//戻る
	"かいふく",			//回復
	"すてーたす",		//ステータス閲覧
	"けいけんち",		//必要経験値を閲覧
	"せってい"			//設定
};

//マップの地形データを宣言する
char map[MAP_MAX][MAP_HEIGHT][MAP_WIDTH + 1] = {
	//MAP_FIELD フィールド
	{
		"~~~~~~~~~~~F~~~~",
		"~~MMMMM~~MM..~~~",
		"~M...M.##..M...~",
		"~M.M.M.~~M.M.M.~",
		"~M.M...~~M...M.~",
		"~M.MMMM~~MMMM..~",
		"~M..MM.~~~~~~#~~",
		"~~M.MM~~~~CMM..~",
		"~~...MM~~M.MMM.~",
		"~...~~M~~M...M.~",
		"~..~~~K~~MMM.M.~",
		"~..~~~.~~M...M.~",
		"~......~~M.MM..~",
		"~~....~~~~....~~",
		"~~~~~~~~~~~~~~~~"
	},
	//MAP_KING_CASTLE 王様の城
	{
		"HHH.......HHH",
		"H.H.......H.H",
		"HHHHHHHHHHHHH",
		".H.........H.",
		".H.HHH.HHH.H.",
		".H.H0H.H1H.H.",
		".H.........H.",
		".HW.......WH.",
		".HY..4.3..YH.",
		"HHHHHH.HHHHHH",
		"H.H~~~#~~~H.H",
		"HHH~~~#~~~HHH",
		"............."
	},

	//小さな小屋
	{
		"HHHHHHH",
		"H.....H",
		"H..w..H",
		"H.....H",
		"HA....H",
		"HHH.HHH",
		"HHH#HHH"
	},

	//迷いの森
	{
		"XXXXXXXNXXXXXXXX",
		"X.......wXX..X.X",
		"X.XXXXXXXX..X..X",
		"X....X.....XX.XX",
		"X.XX.X.XX.XX...X",
		"X.XX.X.XX.X..X.X",
		"XX.....XX.XXX..X",
		"X..XXXXX......XX",
		"X.X...XX.XXXXXXX",
		"X.X.X.XX...XX.XX",
		"X.X......X.XX..X",
		"X.TXXX.XXX..wX.X",
		"XXX......XXXX..X",
		"X...XXXX......XX",
		"XXXXXXXX.XXXXXXX",
		"XXXXXXXXfXXXXXXX"
	},

	//暗黒の荒野
	{
		"MMMMMMMMMMMMMMMM",
		"M......~~......M",
		"M.MMMM.~~.MMMM.M",
		"M.Mw...~~.MMM..M",
		"M.MMMM.~~.MMM.MM",
		"M....M.##b..M..M",
		"MMMM.MMMMMMMMM.M",
		"MM....JMMMMM...M",
		"M..MMMMMMM...M.M",
		"~#~~~~~~~~#~~~#~",
		"M.MMMM.MMM.MMM.M",
		"M.MM...MwM.MMM.M",
		"M....MMM...MMM.M",
		"MMMM.MM..MMMMM.M",
		"M....MMBMMMT...M",
		"MnMMMMMMMMMMMMMM"
	},

	//MAP_BOSS_CASTLE 魔王の城
	{
		"HHH.......HHH",
		"H.H.......H.H",
		"HHHHHHHHHHHHH",
		".H....H....H.",
		".H..WHHHW..H.",
		".H..YH2HY..H.",
		".H.........H.",
		".H..W...W..H.",
		".H..Y...Y..H.",
		".H.........H.",
		"HHHHHH.HHHHHH",
		"H.H~~~#~~~H.H",
		"HHH~~~#~~~HHH",
		"~~~~~~#~~~~~~",
		"~~~~~~#~~~~~~",
		"............."
	}

};

int currentMap;		//現在のマップを定義する
int lastMap;		//１つ前の動作の際にいたマップを定義する
int playerX = 6;	//プレイヤーのX座標
int playerY = 12;	//プレイヤーのY座標


//関数の定義 
//ゲームを初期化する関数
void Init() {
	//プレイヤーのステータスを初期化する
	characters[CHARACTER_PLAYER] = monsters[MONSTER_PLAYER];

	//現在のマップを初期化する
	currentMap = MAP_KING_CASTLE;

	playerX = 4;	//プレイヤーのX座標を初期化する
	playerY = 6;	//プレイヤーのY座標を初期化する

}

//戦闘シーンの画面を描画する関数を宣言する
void DrawBattleScreen() {

	//画面をクリアする
	system("cls");

	//プレイヤーの名前を表示する
	cout << characters[CHARACTER_PLAYER].name << '\n';

	//プレイヤーのステータスを表示する
	cout << "ＨＰ:" << characters[CHARACTER_PLAYER].hp << "／" << characters[CHARACTER_PLAYER].maxHp << " ";
	cout << "ＭＰ:" << characters[CHARACTER_PLAYER].mp << "／" << characters[CHARACTER_PLAYER].maxMp << '\n';

	cout << '\n';

	//モンスターのアスキーアートを描画する
	cout << characters[CHARACTER_MONSTER].aa;

	//モンスターのHPを表示する
	cout << "(ＨＰ:" << characters[CHARACTER_MONSTER].hp << "／" << characters[CHARACTER_MONSTER].maxHp << ')' << '\n';

	cout << '\n';

}

//レベルアップを判定する関数
void levelUp(int level, int ex) {
	int exp = 1, HpG = 0, MpG = 0, attG = 0;
	if (level <= 10) {
		for (int i = 1; i <= level; i++) {
			exp = 2 * exp;
		}
	}
	else {
		exp = 1024 + ((level - 10) * 512);
	}

	if (ex >= exp) {
		do {
			//戦闘シーンの画面を再描画する関数を呼び出す
			DrawBattleScreen();

			cout << "れべるあっぷ！\n";
			cout << "れべるが" << characters[CHARACTER_PLAYER].level << "から";
			HpG = (4 + (rand() % 3));
			MpG = (1 + (rand() % 3));
			attG = (1 + (rand() % 3));
			characters[CHARACTER_PLAYER].maxHp += HpG;
			characters[CHARACTER_PLAYER].maxMp += MpG;
			characters[CHARACTER_PLAYER].attack += attG;
			characters[CHARACTER_PLAYER].level += 1;
			cout << characters[CHARACTER_PLAYER].level << "になった！\n";

			_getch();

			cout << "ゆうしゃは　ＨＰが" << HpG << "ふえた！\n";
			cout << "ＭＰが" << MpG << " こうげきりょくが" << attG << "ふえた！\n";

			//プレイヤーのHPとMPを回復させる
			characters[CHARACTER_PLAYER].hp = characters[CHARACTER_PLAYER].maxHp;
			characters[CHARACTER_PLAYER].mp = characters[CHARACTER_PLAYER].maxMp;

			if (characters[CHARACTER_PLAYER].level <= 10) {
				for (int i = 1; i <= characters[CHARACTER_PLAYER].level; i++) {
					exp = 2 * exp;
				}
			}
			else {
				exp += 512;
			}
		} while (ex >= exp);
	}
	else {
		cout << "つぎの　れべるあっぷまで\n" << exp - ex << "の　けいけんちがひつよう！\n";
	}
	return;
}

//コマンドを選択する関数を宣言する
void SelectCommand() {

	//プレイヤーのコマンドを初期化する
	characters[CHARACTER_PLAYER].command = COMMAND_FIGHT;

	//コマンドが決定されるまでループする
	while (1) {
		//戦闘画面を描画する関数をよびだす
		DrawBattleScreen();

		//コマンドの一覧を表示する
		for (int i = 0; i < COMMAND_MAX; i++) {
			//選択中のコマンドなら
			if (i == characters[CHARACTER_PLAYER].command) {
				//カーソルを描写する
				cout << "＞";
			}
			//選択中のコマンドでなければ
			else {
				//全角スペースを描画する
				cout << "　";
			}

			//コマンドの名前を表示する
			cout << commandNames[i] << '\n';
		}
		//入力されたキーによって分岐する
		switch (_getch()) {
		case 'w':		//wキーが押されたら
			//上のコマンドに切り替える
			characters[CHARACTER_PLAYER].command--;
			break;

		case 's':		//sキーが押されたら
			//下のコマンドに切り替える
			characters[CHARACTER_PLAYER].command++;
			break;

		default:		//上記以外のキーが押されたら
			return;		//関数を抜ける
		}

		//カーソルを上下にループさせる
		characters[CHARACTER_PLAYER].command = (COMMAND_MAX + characters[CHARACTER_PLAYER].command) % COMMAND_MAX;
	}
}

//呪文を選択する関数を宣言する
void SelectSPELL() {

	//プレイヤーのコマンドを初期化する
	characters[CHARACTER_PLAYER].command = SPELL_HEAL;

	//コマンドが決定されるまでループする
	while (1) {
		//戦闘画面を描画する関数をよびだす
		DrawBattleScreen();

		//コマンドの一覧を表示する
		for (int i = 0; i < SPELL_MAX; i++) {
			//選択中のコマンドなら
			if (i == characters[CHARACTER_PLAYER].command) {
				//カーソルを描写する
				cout << "＞";
			}
			//選択中のコマンドでなければ
			else {
				//全角スペースを描画する
				cout << "　";
			}

			//コマンドの名前を表示する
			cout << spellNames[i] << '\n';
		}
		//入力されたキーによって分岐する
		switch (_getch()) {
		case 'w':		//wキーが押されたら
			//上のコマンドに切り替える
			characters[CHARACTER_PLAYER].command--;
			break;

		case 's':		//sキーが押されたら
			//下のコマンドに切り替える
			characters[CHARACTER_PLAYER].command++;
			break;

		default:		//上記以外のキーが押されたら
			return;		//関数を抜ける
		}

		//カーソルを上下にループさせる
		characters[CHARACTER_PLAYER].command = (SPELL_MAX + characters[CHARACTER_PLAYER].command) % SPELL_MAX;
	}
}


//戦闘シーンの関数
void Battle(int _monster) {

	//モンスターのステータスを初期化
	characters[CHARACTER_MONSTER] = monsters[_monster];

	//プレイヤーの攻撃対象をモンスターに設定する
	characters[CHARACTER_PLAYER].target = CHARACTER_MONSTER;

	//モンスターの攻撃対象をプレイヤーに設定する
	characters[CHARACTER_MONSTER].target = CHARACTER_PLAYER;

	//戦闘シーンの画面を描画する関数の呼び出し
	DrawBattleScreen();

	//戦闘シーンの最初のメッセージを表示する
	cout << characters[CHARACTER_MONSTER].name << "が　あらわれた！" << '\n';

	//入力を待つ
	_getch();

	//戦闘が終わるまでループする
	while (1) {

		//コマンドを選択する関数を呼び出す
		SelectCommand();

		//各キャラクターを反復する
		for (int i = 0; i < CHARACTER_MAX; i++) {
			//戦闘シーンの画面を描写する関数を呼び出す
			DrawBattleScreen();

			//選択されたコマンドで分岐する
			switch (characters[i].command) {
			case COMMAND_FIGHT:		//戦う
			{
				//攻撃するメッセージの表示
				cout << characters[i].name << "の　こうげき！" << '\n';
				//キーボード入力を待つ
				_getch();

				//敵に与えるダメージの計算
				int damage = 1 + rand() % characters[i].attack;

				//敵にダメージを与える
				characters[characters[i].target].hp -= damage;

				//敵のHPが負の値になったかどうかを判定する
				if (characters[characters[i].target].hp < 0) {
					//敵のHPを0にする
					characters[characters[i].target].hp = 0;
				}

				//戦闘シーンの画面を再描画する関数を呼び出す
				DrawBattleScreen();

				//敵に与えたダメージのメッセージを表示する
				cout << characters[characters[i].target].name << "に　" << damage << "の　ダメージ！\n";

				//キーボード入力を待つ
				_getch();
				break;
			}
			case COMMAND_SPELL:		//呪文	
				SelectSPELL();
				switch (characters[i].command) {
				case SPELL_BACK:
					i = 1;
					break;
				case SPELL_HEAL:

					//MPが足りるかどうかを判定する
					if (characters[i].mp < SPELL_COST) {

						//MPが足りないメッセージを表示する
						cout << "ＭＰが　たりない！\n";

						//キーボード入力を待つ
						_getch();

						//呪文を唱える処理を抜ける
						break;
					}


					//MPを消費させる
					characters[i].mp -= SPELL_COST;

					//画面を再描画する
					DrawBattleScreen();

					//呪文を唱えたメッセージを表示する
					cout << characters[i].name << "は　ひーるを　となえた！\n";

					//キーボード入力を待つ
					_getch();

					//HPを回復させる
					characters[i].hp = characters[i].maxHp;

					//戦闘シーンの画面を再描画する
					DrawBattleScreen();

					//HPが回復したメッセージを表示する
					cout << characters[i].name << "のきずが　かいふくした！\n";

					//キーボード入力を待つ
					_getch();

					break;
				}
				break;

			case COMMAND_RUN:		//逃げる
				switch (_monster) {
				case MONSTER_TRAP:
				case MONSTER_GUARD:
				case MONSTER_BOSS:
					cout << "このせんとうは　にげられない！\n";
					_getch();
					break;

				default:
					//逃げ出したメッセージを表示する
					cout << characters[i].name << "は　にげだした！\n";

					//キーボードの入力を待つ
					_getch();

					//戦闘処理を抜ける
					return;

					break;
				}

				break;
			}

			//攻撃対象を倒したかどうかを判定する
			if (characters[characters[i].target].hp <= 0) {
				//攻撃対象によって処理を分岐させる
				switch (characters[i].target) {
					//プレイヤーなら
				case CHARACTER_PLAYER:

					//プレイヤーが死んだメッセージを表示する
					cout << "あなたは　しにました";

					break;

					//モンスターなら
				case CHARACTER_MONSTER:

					//モンスターのアスキーアートを何も表示しないように書き換える
					strcpy_s(characters[characters[i].target].aa, "\n");

					//戦闘シーンの画面を再描画する関数を呼び出す
					DrawBattleScreen();

					//モンスターを倒したメッセージを表示する
					cout << characters[characters[i].target].name << "を　たおした！\n";
					_getch();

					//手に入れた経験値を表示する
					characters[i].ex += characters[characters[i].target].ex;
					cout << "ゆうしゃは" << characters[characters[i].target].ex << "の\n" << "けいけんちを　てにいれた！\n";
					_getch();

					//レベルアップしたかどうかを判定する
					levelUp(characters[i].level, characters[i].ex);

					break;
				}

				//キーボード入力を待つ
				_getch();

				//戦闘シーンの関数を抜ける
				return;
			}
		}
	}
}

//マップを描画する処理を記述する関数を宣言する
void DrawMap() {
	//画面をクリアする
	system("cls");

	switch (currentMap) {

	case MAP_MAZE_FOREST://迷いの森

		//描画する全ての行を反復する
		for (int y = playerY - 1; y <= playerY + 1; y++) {
			//描画する全ての列を反復する
			for (int x = playerX - 1; x <= playerX + 1; x++) {
				//対象の座標がプレイヤーの座標と等しいかどうかの判定
				if ((x == playerX) && (y == playerY)) {
					//プレイヤーのアスキーアートを描画する
					cout << "勇";
				}
				else if ((x < 0) || (x >= MAP_WIDTH)
					|| (y < 0) || (y >= MAP_HEIGHT)
					|| (map[currentMap][y][x] == '\0')) {

					//マップの種類によって分岐する
					switch (currentMap) {
					case MAP_MAZE_FOREST:	cout << "Ｘ"; break;	//迷いの森の外は木
					}
				}
				else {	//上記の状態以外なら
					//マップの種類によって分岐する
					switch (map[currentMap][y][x]) {
					case '.':	cout << "．"; break;	//平地
					case '#':	cout << "＃"; break;	//橋
					case 'F':	cout << "■"; break;	//迷いの森
					case 'X':	cout << "Ｘ"; break;	//木
					case 'N':
					case 'f':
						cout << "＊"; break;	//迷いの森出入口
					case 'T':	cout << "◆"; break;	//宝箱
					case 't':	cout << "◇"; break;	//空箱
					case 'w':	cout << "火"; break;	//火
					}
				}
			}
			//1行描画するごとに改行する
			cout << "\n";
		}

		break;


	case MAP_ATER_WASTELAND://暗黒の荒野
		//描画する全ての行を反復する
		for (int y = playerY - 3; y <= playerY + 3; y++) {
			//描画する全ての列を反復する
			for (int x = playerX - 3; x <= playerX + 3; x++) {
				//対象の座標がプレイヤーの座標と等しいかどうかの判定
				if ((x == playerX) && (y == playerY)) {
					//プレイヤーのアスキーアートを描画する
					cout << "勇";
				}
				else if ((x < 0) || (x >= MAP_WIDTH)
					|| (y < 0) || (y >= MAP_HEIGHT)
					|| (map[currentMap][y][x] == '\0')) {

					//マップの種類によって分岐する
					switch (currentMap) {
					case MAP_ATER_WASTELAND:	cout << "Ｍ"; break;	//暗黒の荒野の外は山
					}
				}
				else {	//上記の状態以外なら
					//マップの種類によって分岐する
					switch (map[currentMap][y][x]) {
					case '~':	cout << "～"; break;	//海
					case '.':	cout << "．"; break;	//平地
					case 'M':	cout << "Ｍ"; break;	//山
					case '#':	cout << "＃"; break;	//橋
					case 'n':	cout << "■"; break;	//迷いの森
					case 'B':	cout << "魔"; break;	//魔王の城
					case 'w':	cout << "火"; break;	//火
					case 'b':	cout << "番"; break;	//中ボスの番人
					case 'T':	cout << "◆"; break;	//宝箱
					case 't':	cout << "◇"; break;	//空箱
					case 'J':	cout << "◆"; break;	//宝箱トラップ
					}
				}
			}
			//1行描画するごとに改行する
			cout << "\n";
		}
		break;


	default:
		//描画する全ての行を反復する
		for (int y = playerY - SCREEN_HEIGHT / 2; y < playerY + SCREEN_HEIGHT / 2; y++) {
			//描画する全ての列を反復する
			for (int x = playerX - SCREEN_WIDTH / 2; x < playerX + SCREEN_WIDTH / 2; x++) {
				//対象の座標がプレイヤーの座標と等しいかどうかの判定
				if ((x == playerX) && (y == playerY)) {
					//プレイヤーのアスキーアートを描画する
					cout << "勇";
				}
				else if ((x < 0) || (x >= MAP_WIDTH)
					|| (y < 0) || (y >= MAP_HEIGHT)
					|| (map[currentMap][y][x] == '\0')) {

					//マップの種類によって分岐する
					switch (currentMap) {
					case MAP_FIELD:	cout << "～"; break;	//フィールドの外は海
					case MAP_KING_CASTLE:	cout << "．"; break;	//王様の城の外は平地
					case MAP_BOSS_CASTLE:	cout << "．"; break;	//魔王の城の外は平地
					}
				}
				else {	//上記の状態以外なら
					//マップの種類によって分岐する
					switch (map[currentMap][y][x]) {
					case '~':	cout << "～"; break;	//海
					case '.':	cout << "．"; break;	//平地
					case 'M':	cout << "Ｍ"; break;	//山
					case '#':	cout << "＃"; break;	//橋
					case 'F':	cout << "■"; break;	//迷いの森
					case 'C':	cout << "△"; break;	//小さい小屋
					case 'K':	cout << "王"; break;	//王様の城
					case 'H':	cout << "□"; break;	//壁
					case 'W':	cout << "炎"; break;	//炎
					case 'Y':	cout << "Ｙ"; break;	//燭台
					case '0':	cout << "王"; break;	//王
					case '1':	cout << "姫"; break;	//姫
					case '2':	cout << "魔"; break;	//魔王
					case '3':	cout << "兵"; break;	//兵士
					case '4':	cout << "兵"; break;	//兵士
					case 'w':	cout << "火"; break;	//火
					case 'A':	cout << "◆"; break;	//宝箱
					}
				}
			}
			//1行描画するごとに改行する
			cout << "\n";
		}
		break;
	}
	cout << "\n";

	//プレイヤーの名前を表示する
	cout << characters[CHARACTER_PLAYER].name << "　";

	//プレイヤーのレベルを表示する
	cout << "れべる" << characters[CHARACTER_PLAYER].level << "\n";

	//プレイヤーのステータスを表示する
	cout << "ＨＰ：" << characters[CHARACTER_PLAYER].hp << "／" << characters[CHARACTER_PLAYER].maxHp << " ";
	cout << "ＭＰ：" << characters[CHARACTER_PLAYER].mp << "／" << characters[CHARACTER_PLAYER].maxMp << "\n";

	cout << "\n";
}

//操作方法を示す関数
void control() {
	cout << "┼－－－－－－－－－－－－－┼\n";
	cout << "｜Ｗきーで　うえにいどう 　｜\n";
	cout << "｜Ｓきーで　したにいどう　 ｜\n";
	cout << "｜Ａきーで　ひだりにいどう ｜\n";
	cout << "｜Ｄきーで　みぎにいどう　 ｜\n";
	cout << "｜Ｃきーで　こまんどを　　 ｜\n";
	cout << "｜ひょうじ　するよ！　　　 ｜\n";
	cout << "｜かいわを　すすめるには　 ｜\n";
	cout << "｜すぺーすきーをおして　　 ｜\n";
	cout << "｜ください！　　　　　　　 ｜\n";
	cout << "┼－－－－－－－－－－－－－┼\n";
	_getch();
	return;
}

//勇者のHPを戦闘時以外で回復させる関数
void heal() {
	//MPが足りるかどうかを判定する
	if (characters[CHARACTER_PLAYER].mp < SPELL_COST) {

		//MPが足りないメッセージを表示する
		cout << "ＭＰが　たりないため\n" << "かいふくが　できません。\n";

		//キーボード入力を待つ
		_getch();

		return;
	}
	else if (characters[CHARACTER_PLAYER].hp == characters[CHARACTER_PLAYER].maxHp) {

		//回復する必要がないメッセージを表示する
		cout << "ＨＰが　まんたんのため\n" << "かいふくする　ひつようがない。\n";

		_getch();

		return;
	}

	//MPを消費させる
	characters[CHARACTER_PLAYER].mp -= SPELL_COST;

	//HPを回復させる
	characters[CHARACTER_PLAYER].hp = characters[CHARACTER_PLAYER].maxHp;

	cout << "ＨＰが　かいふくした！\n";

	//キーボード入力を待つ
	_getch();

	return;
}

//勇者のステータスを表示する関数
void status() {
	cout << "さいだいＨＰは" << characters[CHARACTER_PLAYER].maxHp << "\n";
	cout << "さいだいＭＰは" << characters[CHARACTER_PLAYER].maxMp << "\n";
	cout << "こうげきりょくは" << characters[CHARACTER_PLAYER].attack << "\n";
	_getch();
	return;
}

//ゲームをプレイするにあたってやって欲しい変更点を表示させる関数
void settingtoplay() {
	cout << "フォントは　みさきフォント\n";
	cout << "フォントサイズは　３６\n";
	cout << "がめんバッファと　ウィンドウ\n" << "サイズのはばは　３４\n";
	cout << "ウィンドウサイズの　たかさは\n" << "２０に　せっていしてから\n";
	cout << "ゲームをたのしんでください！\n";
	return;
}

//押されたキーをチェックする関数
void stringcheck() {
	while (1) {
		if (_getch() == ' ') {
			break;
		}
	}
	return;
}

//モンスターとの戦闘を発生させる関数
void encount() {
	switch (currentMap) {
	case MAP_FIELD://フィールド
		//敵と遭遇したかどうかを判定する
		if (rand() % 16 == 0) {
			//雑魚モンスターとの戦闘を発生させる
			Battle(MONSTER_SLIME);		//スライム
			break;
		}
		else if (rand() % 32 == 0) {
			Battle(MONSTER_SHEEPN);		//ひつじん
			break;
		}
		else if (rand() % 64 == 0) {
			Battle(MONSTER_BEAST);		//けもけも
			break;
		}
		else if (rand() % 128 == 0) {
			Battle(MONSTER_DEMON);		//悪魔
			break;
		}

		break;

	case MAP_MAZE_FOREST://迷いの森
		if (rand() % 16 == 0) {
			Battle(MONSTER_DEMON);		//悪魔
			break;
		}
		else if (rand() % 32 == 0) {
			Battle(MONSTER_GHOST);		//お化け
			break;
		}
		else if (rand() % 64 == 0) {
			Battle(MONSTER_ZOMBIE);		//もうじゃ
			break;
		}
		else if (rand() % 128 == 0) {
			Battle(MONSTER_MOCHI);		//もちもん
			break;
		}

		break;

	case MAP_ATER_WASTELAND://暗黒の荒野
		if (rand() % 16 == 0) {
			Battle(MONSTER_MOCHI);		//もちもん
			break;
		}
		else if (rand() % 32 == 0) {
			Battle(MONSTER_ALLIGATOR);		//げいたー
			break;
		}
		else if (rand() % 64 == 0) {
			Battle(MONSTER_CACTUS);		//ぼっさん
			break;
		}
		else if (rand() % 128 == 0) {
			Battle(MONSTER_BALANCE);		//やじろん
			break;
		}

		break;
	}
	return;
}

//通常コマンドの動作をする関数
void SelectNORMAL_COMMAND() {

	//プレイヤーのコマンドを初期化する
	characters[CHARACTER_PLAYER].command = NORMAL_COMMAND_BACK;

	//コマンドが決定されるまでループする
	while (1) {

		DrawMap();

		//コマンドの一覧を表示する
		for (int i = 0; i < NORMAL_COMMAND_MAX; i++) {
			//選択中のコマンドなら
			if (i == characters[CHARACTER_PLAYER].command) {
				//カーソルを描写する
				cout << "＞";
			}
			//選択中のコマンドでなければ
			else {
				//[全角スペースを描画する
				cout << "　";
			}

			//コマンドの名前を表示する
			cout << normalcommandNames[i] << '\n';
		}
		//入力されたキーによって分岐する
		switch (_getch()) {
		case 'w':		//wキーが押されたら
			//上のコマンドに切り替える
			characters[CHARACTER_PLAYER].command--;
			break;

		case 's':		//sキーが押されたら
			//下のコマンドに切り替える
			characters[CHARACTER_PLAYER].command++;
			break;

		default:		//上記以外のキーが押されたら
			return;		//関数を抜ける
		}

		//カーソルを上下にループさせる
		characters[CHARACTER_PLAYER].command = (NORMAL_COMMAND_MAX + characters[CHARACTER_PLAYER].command) % NORMAL_COMMAND_MAX;
	}
}


//プログラムの開始
int main() {
	//乱数をシャッフルする
	srand((unsigned int)time(NULL));

	cout << "┼－－－－－－－－－－－－－－－┼\n";
	cout << "｜げーむを　はじめる　まえに！ ｜\n";
	cout << "┼－－－－－－－－－－－－－－－┼\n";
	cout << "すぺーすきーを　おして　すすむ\n";
	stringcheck();		//押されたキーをチェックする関数

	//画面をクリアする
	system("cls");

	settingtoplay();		//ゲームをプレイするにあたってやって欲しい変更点を表示させる関数
	cout << "\n" << "せっていが　おわったら\n" << "すぺーすきーを　おして　すすむ\n";
	stringcheck();		//押されたキーをチェックする関数

	//画面をクリアする
	system("cls");

	cout << "┼－－－－－－－－－－－－－－－┼\n";
	cout << "｜むかしの　ゆうしゃの　たびじ ｜\n";
	cout << "┼－－－－－－－－－－－－－－－┼\n";

	cout << "\n" << "\n" << "すぺーすきーを　おしたら\n" << "はじまるよ！\n";
	stringcheck();		//押されたキーをチェックする関数

	//ゲームを初期化する関数の呼び出し
	Init();

	DrawMap();

	cout << "＊「ゆうしゃよ　よくぞ　まいった！\n";
	cout << "きたにある　まよいの　もりをぬけ\n" << "そのおくに　いる　まおうをたおし\n";
	cout << "せかいを　すくってくれ！」\n";
	cout << "すぺーすきーを　おして　すすむ\n";
	stringcheck();		//押されたキーをチェックする関数

	DrawMap();

	cout << "＊「そうさ　ほうほうは\n" << "Ｑぼたんを　おすと\n" << "かくにんできるぞ！\n";
	cout << "では　よろしくたのむぞ！」\n";
	cout << "すぺーすきーを　おして　すすむ\n";
	stringcheck();		//押されたキーをチェックする関数

	//メインループ
	while (1) {
		//マップを描画する関数を呼び出す
		DrawMap();

		int lastPlayerX = playerX;		//プレイヤーの移動前のX座標を宣言する
		int lastPlayerY = playerY;		//プレイヤーの移動前のY座標を宣言する
		lastMap = currentMap;			//プレイヤーの移動前のマップを宣言する


		//入力されたキーで分岐する
		switch (_getch()) {
		case 'w':	playerY--; break;	//wキーで上移動
		case 's':	playerY++; break;	//sキーで下移動
		case 'a':	playerX--; break;	//aキーで左移動
		case 'd':	playerX++; break;	//dキーで右移動
		case 'q':	control(); break;	//qキーで操作方法を表示
		case 'c':
			SelectNORMAL_COMMAND();		//通常コマンドの動作をする関数

			DrawMap();

			switch (characters[CHARACTER_PLAYER].command) {
			case NORMAL_COMMAND_HEAL:	heal(); break;
			case NORMAL_COMMAND_STATES:	status(); break;
			case NORMAL_COMMAND_EXP:
				levelUp(characters[CHARACTER_PLAYER].level, characters[CHARACTER_PLAYER].ex);
				_getch();
				break;						//zキーで次のレベルアップに必要な経験値を表示
			case NORMAL_COMMAND_SETTING:
				settingtoplay();		//ゲームをプレイするにあたってやって欲しい変更点を表示させる関数
				_getch();
				break;
			}
			break;
		}

		//マップの外にでたかを判定する
		if ((playerX < 0) || (playerX >= MAP_WIDTH)				//X座標がマップの範囲外
			|| (playerY < 0) || (playerY >= MAP_HEIGHT)			//Y座標がマップの範囲外
			|| (map[currentMap][playerY][playerX] == '\0')) {	//未設定のマス

			//現在のマップによって分岐する
			switch (currentMap) {
			case MAP_KING_CASTLE:	//王様の城
				//フィールドマップに切り替える
				currentMap = MAP_FIELD;

				playerX = 6;	//プレイヤーのX座標を設定する
				playerY = 12;	//プレイヤーのY座標を設定する

				break;

			case MAP_BOSS_CASTLE:	//魔王の城
				//フィールドマップに切り替える
				currentMap = MAP_ATER_WASTELAND;

				playerX = 7;	//プレイヤーのX座標を設定する
				playerY = 13;	//プレイヤーのY座標を設定する

				break;

			case MAP_SMALL_HUT:		//小さな小屋
				currentMap = MAP_FIELD;

				playerX = 10;
				playerY = 9;

				break;
			}
		}

		//移動先のマスの種類によって分岐させる
		switch (map[currentMap][playerY][playerX]) {
		case 'K':	//王様の城
			//王様の城にマップを切り替える
			currentMap = MAP_KING_CASTLE;

			playerX = 6;	//プレイヤーのX座標を設定する
			playerY = 12;	//プレイヤーのY座標を設定する

			break;

		case 'B':	//魔王の城
			//魔王の城にマップを切り替える
			currentMap = MAP_BOSS_CASTLE;

			playerX = 6;	//プレイヤーのX座標を設定する
			playerY = 12;	//プレイヤーのY座標を設定する

			break;

		case 'F':	//迷いの森（フィールドから）
			currentMap = MAP_MAZE_FOREST;

			playerX = 8;
			playerY = 14;

			cout << "このもりは　しかいが　わるく\n" << "しゅういが　あまり　みわたせない\n";
			_getch();

			break;

		case 'f':	//フィールド（迷いの森から）
			currentMap = MAP_FIELD;

			playerX = 11;
			playerY = 1;

			break;

		case 'N':	//暗黒の荒野（迷いの森から）
			currentMap = MAP_ATER_WASTELAND;

			playerX = 1;
			playerY = 14;

			cout << "すなあらしの　えいきょうで\n" << "しかいが　すこし　せまくなっている\n";
			_getch();

			break;

		case 'n':	//迷いの森(暗黒の荒野から)
			currentMap = MAP_MAZE_FOREST;

			playerX = 7;
			playerY = 1;

			cout << "このもりは　しかいが　わるく\n" << "しゅういが　あまり　みわたせない\n";
			_getch();

			break;

		case 'C':	//小さな小屋
			currentMap = MAP_SMALL_HUT;

			playerX = 3;
			playerY = 5;

			break;

		case '0':	//王様
			//王様の会話メッセージを表示する
			cout << "＊「ゆうしゃよ　よくぞ　まいった！\n";
			cout << "きたにある　まよいの　もりをぬけ\n" << "そのおくに　いる　まおうをたおし\n";
			cout << "せかいを　すくってくれ！」\n";

			_getch();

			playerX = lastPlayerX;	//プレイヤーのX座標を移動前に戻す
			playerY = lastPlayerY;	//プレイヤーのY座標を移動前に戻す

			DrawMap();

			cout << "＊「そうさ　ほうほうは\n" << "Ｑぼたんを　おすと\n" << "かくにんできるぞ！\n";
			cout << "では　よろしくたのむぞ！」\n";

			_getch();

			break;

		case '1':	//姫様
			//姫の会話メッセージを表示する
			cout << "＊「かみに　いのりを　ささげます。\n" << "おお　かみよ！\n" << "ゆうしゃさまに　しゅくふくを！\n";
			_getch();	//キーボード入力待ち状態にする

			playerX = lastPlayerX;	//プレイヤーのX座標を移動前に戻す
			playerY = lastPlayerY;	//プレイヤーのY座標を移動前に戻す

			DrawMap();

			//プレイヤーのHPを回復させる
			characters[CHARACTER_PLAYER].hp = characters[CHARACTER_PLAYER].maxHp;

			//プレイヤーのMPを回復させる
			characters[CHARACTER_PLAYER].mp = characters[CHARACTER_PLAYER].maxMp;

			cout << "ＨＰと　ＭＰが　かいふくした！\n";
			_getch();

			break;

		case '2':	//魔王
			//魔王の会話メッセージを表示する
			cout << "＊「おろかな　にんげんよ！\n" << "わが　やぼうを　はばむものは\n" << "このよから　けしさってくれる！\n";
			_getch();	//キーボード入力待ち状態にする
			//魔王との戦闘を発生させる
			Battle(MONSTER_BOSS);

			//魔王が死んだかどうかを判定する
			if (characters[CHARACTER_MONSTER].hp <= 0) {
				//画面をクリアする
				system("cls");

				//エンディングのメッセージを表示する
				cout << "　まおうは　ほろび　せかいは\n" << "めつぼうのききから　すくわれた！\n" << "\n";
				cout << "　おうは　ふれをだし　ゆうしゃを\n" << "さがしもとめたが、だれも\n" << "みたものは　いなかったという…\n";
				cout << "\n" << "\n" << "　　　　ＴＨＥ　ＥＮＤ";

				//キーボード入力待ち状態にする
				_getch();

				//ゲームを終了する
				return 0;
			}

			break;

		case '3':		//兵士
			cout << "＊「まよいの　もりは　ひじょうに\n";
			cout << "きけん　ですので\n" << "きをつけてください！\n";
			_getch();

			playerX = lastPlayerX;	//プレイヤーのX座標を移動前に戻す
			playerY = lastPlayerY;	//プレイヤーのY座標を移動前に戻す

			DrawMap();

			cout << "そういえば　ひがしにある\n" << "ちいさなこやに\n";
			cout << "こもんじょが　あるとか　ないとか…\n";
			_getch();

			playerX = lastPlayerX;	//プレイヤーのX座標を移動前に戻す
			playerY = lastPlayerY;	//プレイヤーのY座標を移動前に戻す

			DrawMap();

			cout << "そんなはなしは　どうでも\n" << "いいですよね。\n";
			cout << "ゆうしゃさま　どうかごぶじで！」\n";
			_getch();

			break;


		case '4':		//兵士２人目
			cout << "＊「てきとそうぐう　したさいは\n" << "かならず　にげることが\n";
			cout << "できるてきや　にげられない\n" << "てきがいます。\n";

			_getch();

			playerX = lastPlayerX;	//プレイヤーのX座標を移動前に戻す
			playerY = lastPlayerY;	//プレイヤーのY座標を移動前に戻す

			DrawMap();

			cout << "まおうやその　てさきは\n" << "にげることが　できないてき\n";
			cout << "であるため　じゅんびを\n" << "してから　たたかうことを\n";
			cout << "おすすめします！\n";
			_getch();

			playerX = lastPlayerX;	//プレイヤーのX座標を移動前に戻す
			playerY = lastPlayerY;	//プレイヤーのY座標を移動前に戻す

			DrawMap();

			cout << "ゆうしゃさま　どうかごぶじで！」\n";
			_getch();
			break;

		case 'w':		//小さい小屋の中の火と迷いの森内にある火
			cout << "あたたかい　ひに　つつまれた\n" << "ＨＰとＭＰ　がかいふく　した！\n";
			_getch();

			//プレイヤーのHPを回復させる
			characters[CHARACTER_PLAYER].hp = characters[CHARACTER_PLAYER].maxHp;

			//プレイヤーのMPを回復させる
			characters[CHARACTER_PLAYER].mp = characters[CHARACTER_PLAYER].maxMp;

			break;

		case 'T':		//宝箱
			cout << "たからばこを　みつけた！\n" << "たからばこを　あけると・・・\n";
			switch (rand() % 3) {
			case 0:
				cout << "ＨＰが　５ じょうしょうした！\n";
				characters[CHARACTER_PLAYER].maxHp += 5;
				//プレイヤーのHPを回復させる
				characters[CHARACTER_PLAYER].hp = characters[CHARACTER_PLAYER].maxHp;
				_getch();
				break;

			case 1:
				cout << "ＭＰが　３　じょうしょうした！\n";
				characters[CHARACTER_PLAYER].maxMp += 3;
				//プレイヤーのMPを回復させる
				characters[CHARACTER_PLAYER].mp = characters[CHARACTER_PLAYER].maxMp;
				_getch();
				break;

			case 2:
				cout << "こうげきが\n" << "３　じょうしょうした！\n";
				characters[CHARACTER_PLAYER].attack += 3;
				_getch();
				break;
			}
			map[currentMap][playerY][playerX] = 't';
			break;

		case 'J':		//宝箱トラップ
			cout << "たからばこを　みつけた！\n" << "たからばこを　あけると・・・\n";
			cout << "なかから　もんすたーが\n" << "でてきた！\n";
			_getch();

			Battle(MONSTER_TRAP);		//わななん

			if (characters[CHARACTER_PLAYER].hp >= 1) {
				map[currentMap][playerY][playerX] = 't';
			}

			break;

		case 'b':		//中ボスの番人
			cout << "＊「まおうさまの　じゃまを\n" << "するものは　だれであろうと\n" << "とおしはさせない！」\n";
			_getch();

			Battle(MONSTER_GUARD);		//ばんにん

			if (characters[CHARACTER_PLAYER].hp >= 1) {
				cout << "＊「まおうさま\n" << "もうしわけ　ありません…。\n";
				_getch();
				map[currentMap][playerY][playerX] = '.';
			}

			break;

		case 'A':		//宝箱
			cout << "たからばこを　みつけた！\n" << "たからばこを　あけると・・・\n";
			_getch();
			cout << "なかから　ふるい　かみきれが\n" << "みつかった！\n";
			_getch();

			playerX = lastPlayerX;	//プレイヤーのX座標を移動前に戻す
			playerY = lastPlayerY;	//プレイヤーのY座標を移動前に戻す

			DrawMap();

			cout << "ふるい　かみきれには\n" << "こうかいてある。\n";
			cout << "ＬＲＲＬＲ　ＬＬ\n";
			cout << "このさきは　かみがきれて\n" << "しまっており　よむことができない\n";
			_getch();

			break;

		}

		//移動先のマスの種類によって分岐させる
		switch (map[currentMap][playerY][playerX]) {
		case '.':		//平地
		case '#':		//海
			if (((playerX != lastPlayerX) || (playerY != lastPlayerY)) && (currentMap == lastMap)) {
				encount();
			}
			break;

		default:		//上記以外のマス
			playerX = lastPlayerX;	//プレイヤーのX座標を移動前に戻す
			playerY = lastPlayerY;	//プレイヤーのY座標を移動前に戻す
			break;
		}
		//プレイヤーが死んだかどうかを判定する
		if (characters[CHARACTER_PLAYER].hp <= 0) {

			//現在のマップを初期化する
			currentMap = MAP_KING_CASTLE;

			playerX = 4;	//プレイヤーのX座標を初期化する
			playerY = 6;	//プレイヤーのY座標を初期化する

			//プレイヤーのHPを回復させる
			characters[CHARACTER_PLAYER].hp = characters[CHARACTER_PLAYER].maxHp;

			//プレイヤーのMPを回復させる
			characters[CHARACTER_PLAYER].mp = characters[CHARACTER_PLAYER].maxMp;

			//画面を再描画する
			DrawMap();

			//王様のメッセージを表示する
			cout << "＊「おお　ゆうしゃよ！\n" << "かみが　そなたを　すくわれた！\n" << "ゆうしゃに　えいこう　あれ！\n";

			//キーボードの入力待ち状態にする
			_getch();
		}
	}
}
