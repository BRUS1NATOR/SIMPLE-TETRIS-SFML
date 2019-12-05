#include "Block.h"

namespace Grid	//Game grid
{
	 std::vector<sf::Sprite> sprites;
}

extern std::vector<sf::Sprite> sprites;	

void Block::SetPosition(Vector2i pos) {
	for (int i = 0; i < cubes.size(); i++) {
		cubes[i].GetSprite().setPosition(pos.x + cubes[i].GetLocalPosition().x, pos.y + cubes[i].GetLocalPosition().y);
	}
};

bool Block::Move(Direction dir) {	//Move block
	Vector2i move(0, 0);

	switch (dir)
	{
	case Left:
		move.x = -Grid::blockSize;
		break;
	case Down:
		move.y = Grid::blockSize;
		break;
	default:
		move.x = Grid::blockSize;
	}

	for (int i = 0; i < cubes.size(); i++) {
		Vector2i v = Vector2i(cubes[i].GetSprite().getPosition());
		v += move;
		if (Grid::CheckCollision(v)) {
			return false;
		}
	}
	for (int i = 0; i < cubes.size(); i++) {
		cubes[i].GetSprite().move(move.x, move.y);
	}
	return true;
}

bool Block::Rotate(Direction dir) {
	std::vector<Vector2i> newLocalPositions;
	Vector2i centerPosition = Vector2i(cubes[0].GetSprite().getPosition());

	for (int i = 0; i < cubes.size(); i++) {
		Vector2i vLocal = cubes[i].GetLocalPosition();

		int tempX = vLocal.x;
		if (dir == Left) {
			vLocal.x = vLocal.y;
			vLocal.y = -tempX;
		}
		else {
			vLocal.x = -vLocal.y;
			vLocal.y = tempX;
		}

		if (Grid::CheckCollision(centerPosition + vLocal)) {
			return false;
		}
		newLocalPositions.push_back(vLocal);
	}

	for (int i = 0; i < newLocalPositions.size(); i++) {
		cubes[i].SetLocalPosition(newLocalPositions[i]);
	}
	return true;
}

std::vector<Sprite> Block::GetSprites() {
	int s = cubes.size();
	std::vector<Sprite> sprites;
	for (int i = 0; i < cubes.size(); i++) {
		sprites.push_back(cubes[i].GetSprite());
	}
	return sprites;
}

void Cube::SetLocalPosition(Vector2i position) {
	Vector2i v = Vector2i(sprite.getPosition().x - localPosition.x, sprite.getPosition().y - localPosition.y);	//Get center of block
	localPosition = position;
	v += position;
	sprite.setPosition(v.x, v.y);
}