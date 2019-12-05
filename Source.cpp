//Created by Bruslenko Alexander, feel free to use or modify
#include <cstdlib> // srand() rand()
#include "Block.h"

using namespace sf;

const int fps = 30; //Framerate
int moveFrame = 30; //Frame on which block will move
int framenow = 0;
int score = 0;   //Score
bool pause = false;

std::vector<std::vector<Vector2i>> blocks;	//Existing blocks

Texture blockTexture;	//Block Texture
Texture bgTex;			//Grid Texture
Block movingBlock;		//Moving Block
Block futureBlock;		//Future Block

RenderWindow window(VideoMode(Grid::size.x* Grid::blockSize + Grid::offset.x,
	Grid::size.y* Grid::blockSize + Grid::offset.y), "TETRIS");	//Window

namespace Graphics {
	Font font;	//Стиль текста
	Text text;	//Текст

	Sprite bgSprite;		//Спрайт игрового поля
	RectangleShape backgroundColor(Vector2f(10000, 10000));	//Фоновый цвет сверху
	RectangleShape UIcolor(Vector2f(Grid::offset.x, window.getSize().y)); //Фоновый цвет справа
}

void InitializeGraphic() {	//Инициализация графики
	Graphics::backgroundColor.setPosition(Vector2f(0,0));
	Graphics::backgroundColor.setFillColor(Color(98, 117, 93));
	Graphics::UIcolor.setPosition(Vector2f(window.getSize().x - Grid::offset.x, 0));
	Graphics::UIcolor.setFillColor(Color(98, 117, 93));

	if (!blockTexture.loadFromFile("res/black.png")) {
		std::cout << "FAIL TO LOAD BLOCK TEXTURE" << std::endl;
		system("pause");
	}

	if (!bgTex.loadFromFile("res/background.png")) {
		std::cout << "FAIL TO LOAD BACKGROUND" << std::endl;
		system("pause");
	}
	if (!Graphics::font.loadFromFile("res/EFNMacStyle8.ttf")) {
		std::cout << "FAIL TO LOAD FONT" << std::endl;
		system("pause");
	}

	Graphics::text.setFont(Graphics::font);
	Graphics::text.setFillColor(Color(0,0,0));
	Graphics::text.setPosition(window.getSize().x - Grid::offset.x, 128);
	Graphics::text.setString("Score: \n" + std::to_string(score) + "\nSpeed: \n" + "1" + "\nNext Block");

	Graphics::bgSprite.setTexture(bgTex);
	Graphics::bgSprite.setPosition(0, Grid::offset.y);
	Graphics::bgSprite.setTextureRect(IntRect(0, 0, 768, 1600));
}

void InitializeBlocks() { //Инициализация блоков

	std::vector<Vector2i> temp;	//Г
	temp.push_back(Vector2i(0, 0));
	temp.push_back(Vector2i(0, 1));
	temp.push_back(Vector2i(0, -1));
	temp.push_back(Vector2i(1, -1));
	blocks.push_back(temp);

	temp.clear();	//Г reverse
	temp.push_back(Vector2i(0, 0));
	temp.push_back(Vector2i(0, 1));
	temp.push_back(Vector2i(0, -1));
	temp.push_back(Vector2i(-1, -1));
	blocks.push_back(temp);

	temp.clear();		//г
	temp.push_back(Vector2i(0, 0));
	temp.push_back(Vector2i(0, 1));
	temp.push_back(Vector2i(1, 1));
	blocks.push_back(temp);

	temp.clear();	// --
	temp.push_back(Vector2i(0, 0));
	temp.push_back(Vector2i(0, -1));
	temp.push_back(Vector2i(0, 1));
	temp.push_back(Vector2i(0, 2));
	blocks.push_back(temp);

	temp.clear();	// _-
	temp.push_back(Vector2i(0, 0));
	temp.push_back(Vector2i(-1,0));
	temp.push_back(Vector2i(0, 1));
	temp.push_back(Vector2i(1, 1));
	blocks.push_back(temp);

	temp.clear();	// -_
	temp.push_back(Vector2i(0, 0));
	temp.push_back(Vector2i(1, 0));
	temp.push_back(Vector2i(0, 1));
	temp.push_back(Vector2i(-1, 1));
	blocks.push_back(temp);

	temp.clear(); // o
	temp.push_back(Vector2i(0, 0));
	temp.push_back(Vector2i(1, 0));
	temp.push_back(Vector2i(1, 1));
	temp.push_back(Vector2i(0, 1));
	blocks.push_back(temp);

	temp.clear();	//_|_
	temp.push_back(Vector2i(0, 0));
	temp.push_back(Vector2i(1, 0));
	temp.push_back(Vector2i(-1, 0));
	temp.push_back(Vector2i(0, -1));
	blocks.push_back(temp);
}

Block NewBlock() {	//Новый блок

	std::vector<Vector2i> localCords = blocks[rand() % blocks.size()];	//Случайный блок
	std::vector<Cube> cubes;	//Будущие кубы

	for (int i = 0; i < localCords.size(); i++) {
		Sprite sprite(blockTexture, IntRect(0, 0, Grid::blockSize, Grid::blockSize));
		Cube cube(localCords[i], sprite);
		cubes.push_back(cube);
	}

	Block newBlock(cubes);	//Создаем блок из кубов

	newBlock.SetPosition(Vector2i(window.getSize().x - Grid::offset.x + 64, 320));
	return newBlock;
}

void RestartGame() {	//Перезапуск игры
	Grid::Clear();
	moveFrame = 30;
	score = 0;
	framenow = 0;
	window.setFramerateLimit(fps);
}

void CheckGrid() {	//Проверка игрового поля
	int result = Grid::UpdateGrid();

	if (result < 0) {
		RestartGame();
		score = 0;
	}
	else {
		switch (result) {
		case 1:
			score += 100;
			break;
		case 2:
			score += 300;
			break;
		case 3:
			score += 600;
			break;
		case 4:
			score += 1000;
			break;
		}
	}

	int speed = 1;
	if (score >= 2000) {
		speed = 4;
		moveFrame = 10;
	}
	else if (score >= 1500) {
		speed = 3;
		moveFrame = 15;
	}
	else if (score >= 1000) {
		speed = 2;
		moveFrame = 20;
	}

	Graphics::text.setString("Score: \n" + std::to_string(score) + "\nSpeed: \n" + std::to_string(speed) + "\nNext Block");
}

void Render() {	//Рендер
	window.clear();		//Очищаем экран

	window.draw(Graphics::backgroundColor); //Фон
	window.draw(Graphics::bgSprite);		//Игровое поле
	window.draw(Graphics::UIcolor);			//Фон интерфейса
	window.draw(Graphics::text);			//Текст

	for (int i = 0; i < Grid::sprites.size(); i++)
	{
		window.draw(Grid::sprites[i]);	//Спрайты на поле
	}

	std::vector <Sprite> sprites = movingBlock.GetSprites();

	for (int i = 0; i < sprites.size(); i++)
	{
		window.draw(sprites[i]);	//Движущийся блок
	}

	sprites = futureBlock.GetSprites();
	for (int i = 0; i < sprites.size(); i++)
	{
		window.draw(sprites[i]);	//Будущий блок
	}
	window.display();	//Отобразить
}

int WinMain()		//Use main for DEBUG, WinMain for Reelase
{
	InitializeGraphic();
	InitializeBlocks();
	RestartGame();

	futureBlock = NewBlock();
	movingBlock = NewBlock();

	movingBlock.SetPosition(Vector2i(160,0));
	
	//Resizing Window based on resolution, well it doesnt work that well
	double width = sf::VideoMode::getDesktopMode().width / 1920.0;
	double height = sf::VideoMode::getDesktopMode().height / 1080.0;
	window.setSize(Vector2u(window.getSize().x * width, window.getSize().y * height));

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) {
				window.close();
			}
			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Escape) {
					pause = !pause;
				}
				if (!pause) {
					if (event.key.code == Keyboard::A) {
						movingBlock.Rotate(Left);
					}
					if (event.key.code == Keyboard::D) {
						movingBlock.Rotate(Right);
					}
					if (event.key.code == Keyboard::Right) {
						movingBlock.Move(Right);
					}
					if (event.key.code == Keyboard::Left) {
						movingBlock.Move(Left);
					}
					if (event.key.code == Keyboard::Down) {
						framenow = moveFrame;
					}
				}
			}
		}
		if (!pause) {
			framenow += 1;

			if (framenow >= moveFrame) {

				if (movingBlock.Move(Down) == false) {
					std::vector<Sprite> sprites = movingBlock.GetSprites();
					for (int i = 0; i < sprites.size(); i++) {
						Grid::sprites.push_back(sprites[i]);
					}
					movingBlock = futureBlock;
					movingBlock.SetPosition(Vector2i(160, 0));
					CheckGrid();
					futureBlock = NewBlock();
				}
				framenow = 0;
			}
		}
		Render();
	}
	return 1;
}
