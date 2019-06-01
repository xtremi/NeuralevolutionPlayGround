#include "MapIO.h"


bool MapIO::write(const std::string& filepath, int wdim[2],
	std::vector<ObstacleSprite*>* obstacleSprites, std::vector<FoodBlock*>* foodSprites)
{
	if (!openWrite(filepath))
		return false;

	for (ObstacleSprite* os : (*obstacleSprites)) {
		glm::vec2 pos = glm::vec2(agk::GetSpriteX(os->getID()), agk::GetSpriteY(os->getID()));
		glm::vec2 size = os->getSize();

		pos.x /= (float)wdim[0];
		pos.y /= (float)wdim[1];
		size.x /= (float)wdim[0];
		size.y /= (float)wdim[1];

		std::string posstr = TOSTR(pos.x, 3) + " " + TOSTR(pos.y, 3);

		if (os->type() == ObstacleSprite::obstacle::block) {
			file_o << "BOX " << posstr;
			file_o << " " << TOSTR(size.x, 3) << " " << TOSTR(size.y, 3) << " ";
			float ang = os->getRotation();
			file_o << TOSTR(ang, 3) << std::endl;
		}
		else if (os->type() == ObstacleSprite::obstacle::circle) {
			file_o << "CIRCLE  " << posstr;
			file_o << " " << TOSTR(size.x, 3) << std::endl;
		}

	}
	for (FoodBlock* fb : (*foodSprites)) {
		glm::vec2 pos = fb->getPos();
		pos.x /= (float)wdim[0];
		pos.y /= (float)wdim[1];
		file_o << "FOOD " << TOSTR(pos.x, 3) << " " << TOSTR(pos.y, 3) << std::endl;
	}

	file_o.close();
	return true;
}

bool MapIO::read(const std::string& filepath, int wdim[2],
	std::vector<ObstacleSprite*>* obstacleSprites, std::vector<FoodBlock*>* foodSprites)
{
	if (!openRead(filepath))
		return false;

	std::string type;
	std::string px, py, sx, sy, alphastr;
	glm::vec2 pos, size;
	float alpha;

	float dimx = (float)wdim[0];
	float dimy = (float)wdim[1];

	while (!file_i.eof()) {

		//BOX FOOD OR CIRCLE
		file_i >> type;
		if (type[0] == 'B' || type[0] == 'C' || type[0] == 'F') {
			file_i >> px >> py;
			pos = glm::vec2(std::stof(px) * dimx, std::stof(py)* dimy);

			if (type[0] == 'B') {
				file_i >> sx >> sy >> alphastr;

				size = glm::vec2(std::stof(sx) * dimx, std::stof(sy) * dimy);
				alpha = std::stof(alphastr);
				glm::vec3 color = randVec3FromTo(0.0f, 1.0f);

				ObstacleBlock* ob = new ObstacleBlock(pos, size, color);
				ob->setRotation(alpha);
				obstacleSprites->push_back(ob);
			}
			else if (type[0] == 'F') {
				FoodBlock* food = new FoodBlock(pos);
				foodSprites->push_back(food);
			}
			else if (type[0] == 'C') {
				file_i >> sx;

				size = glm::vec2(std::stof(sx) * dimx, -1);
				glm::vec3 color = randVec3FromTo(0.0f, 1.0f);
				ObstacleCircle* oc = new ObstacleCircle(pos, size, color);
				obstacleSprites->push_back(oc);
			}
		}
	}

	file_i.close();
	return true;
}

