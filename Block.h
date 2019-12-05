#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

enum Direction { Up, Left, Down, Right };

using namespace sf;

namespace Grid	//Game Grid
{
	extern std::vector<sf::Sprite> sprites;	//Spries

	int const blockSize = 32;
	Vector2i const size(10, 22); //Grid Size
	Vector2i const offset(192, 64); //Offset for text

	static void Clear() { //Clear game grid
		sprites.erase(sprites.begin(), sprites.end());
	}

    static bool CheckBorders(Vector2i pos) { //Check borders
		if (pos.x < 0 || pos.x >(size.x - 1) * blockSize || pos.y > (size.y - 1) * blockSize + offset.y) {
			return true;
		}
		return false;
	}

	static bool CheckCollision(Vector2i pos) { //Check collision
		for (int i = 0; i < sprites.size(); i++)
		{
			Vector2f v = sprites[i].getPosition();
			if (pos.x == v.x && pos.y == v.y) {
				std::cout << "COLLISION!" << std::endl;
				return true;
			}
		}
		return CheckBorders(pos);
	}

	static void DeleteLines(std::vector<int> yCoord) {	//Destroy row of blocks

		for (int i = 0; i < yCoord.size(); i++) {

			for (int index = 0; index < sprites.size(); index++) {
				if (sprites[index].getPosition().y == yCoord[i]) {
					sprites.erase(sprites.begin() + index);
					index--;
				}
			}

			for (int j = 0; j < sprites.size(); j++)
			{
				if (sprites[j].getPosition().y < yCoord[i]) {
					sprites[j].move(0, Grid::blockSize);
				}
			}
		}
	}

	static int UpdateGrid() { //Check if there are block to destroy

		std::vector<int> toDeleteY;
		std::vector<int> allYCoords;

		for (int i = 0; i < sprites.size(); i++) {
			allYCoords.push_back(sprites[i].getPosition().y);
			if (sprites[i].getPosition().y < 0) { return -1; }
		}

		for (int i = 0; i <= Grid::size.y+1; i++) {		
			int c = std::count(allYCoords.begin(), allYCoords.end(), i * Grid::blockSize);
			if (c == size.x) {
				toDeleteY.push_back(i * Grid::blockSize);
			}
		}
		if (!toDeleteY.empty()) {
			DeleteLines(toDeleteY);
			return toDeleteY.size();
		}
		return 0;
	}
};

class Cube {	//Unit of block
private:
	Vector2i localPosition;	//Local position
	Sprite sprite; //Sprite

public:
	Sprite& GetSprite() {	//Sprite of unit
		return sprite;
	}

	Vector2i GetLocalPosition() {	//Current local position
		return localPosition;
	}

	Cube(Vector2i localPos, Sprite s) {		//Creation of cube
		sprite = s;
		sprite.setPosition(localPos.x * Grid::blockSize, localPos.y * Grid::blockSize);
		localPosition = Vector2i(localPos.x * Grid::blockSize, localPos.y * Grid::blockSize);
	}

	void SetLocalPosition(Vector2i position);	//Change local position
};

class Block	{	//Block
public:
	std::vector<Cube> cubes;	//Units of which block composed

	void SetPosition(Vector2i pos);	//Set position of block
	
	bool Move(Direction dir); 	//Move Blcok

	bool Rotate(Direction dir);	//Rotate Block
	
	std::vector<Sprite> GetSprites();	//Get sprite which block contains

	Block(std::vector<Cube> _cubes) {
		cubes = _cubes;
	}
	Block() {
	}
};