#include "NNCarInfoFrame.h"


NNCarInfoFrame::NNCarInfoFrame(const glm::vec2& pos, const glm::vec2& _frameElSize, int _nrows) : UIFrame(pos)
{
	botpos		= glm::vec2(0.0f);
	frameElSize = _frameElSize;
	nrows		= _nrows;
}


NNCarInfoFrameElement* NNCarInfoFrame::addFrameElement() {
	NNCarInfoFrameElement* el = new NNCarInfoFrameElement(frameElSize);
	nframes++;
	addFrame(el, std::to_string(nframes), botpos, true);
	botpos += glm::vec2(0.0f, frameElSize.y);
	if((nframes % nrows) == 0){
		botpos.x += frameElSize.x;
		botpos.y = 0.0f;
	}
	realignSprites();
	return el;
}


void NNCarInfoFrameElement::wflash() {
	if(!wflashed){
		w_sprite->setColor(glm::vec3(1, 0, 0));
		wflashed = true;
	}
}
void NNCarInfoFrameElement::aflash() {
	if (!aflashed) {
		a_sprite->setColor(glm::vec3(1, 0, 0));
		aflashed = true;
	}
}
void NNCarInfoFrameElement::sflash() {
	if (!sflashed) {
		s_sprite->setColor(glm::vec3(1, 0, 0));
		sflashed = true;
	}
}
void NNCarInfoFrameElement::dflash() {
	if (!dflashed) {
		d_sprite->setColor(glm::vec3(1, 0, 0));
		dflashed = true;
	}
}

NNCarInfoFrameElement::NNCarInfoFrameElement(const glm::vec2& frameElSize) : UIFrame()
{
	Sprite* background = new Sprite(glm::vec3(0.8f), glm::vec2(0.0f), glm::vec2(frameElSize.x, frameElSize.y - 10.0f), AgkHAlign::left, AgkVAlign::top);
	Sprite* border = new Sprite(glm::vec3(0.2f), glm::vec2(0.0f), glm::vec2(frameElSize.x, 10.0f), AgkHAlign::left, AgkVAlign::top);
	addSprite(background, "background", glm::vec2(0.0f, 10.0f), true);
	addSprite(border, "border", glm::vec2(0.0f, 0.0f), true);
	background->setDepth(12);
	border->setDepth(12);

	glm::vec2 spriteSize(LETTER_KEY_SIZE);
	float textSize = LETTER_KEY_TEXT_SIZE;
	glm::vec3 spriteColor(1, 1, 0);
	glm::vec3 textColor(1, 0, 0);
	
	w_sprite = new SpriteAndText(spriteColor, "W", textColor, spriteSize, textSize, glm::vec2(0), AgkHAlign::center, AgkVAlign::center);
	a_sprite = new SpriteAndText(spriteColor, "A", textColor, spriteSize, textSize, glm::vec2(0), AgkHAlign::center, AgkVAlign::center);
	s_sprite = new SpriteAndText(spriteColor, "S", textColor, spriteSize, textSize, glm::vec2(0), AgkHAlign::center, AgkVAlign::center);
	d_sprite = new SpriteAndText(spriteColor, "D", textColor, spriteSize, textSize, glm::vec2(0), AgkHAlign::center, AgkVAlign::center);
	w_sprite->setDepth(11);
	a_sprite->setDepth(11);
	s_sprite->setDepth(11);
	d_sprite->setDepth(11);

	float d = spriteSize.x*1.1f;
	glm::vec2 keypos(8.0f, 16.0f);
	addSprite(w_sprite, "w", keypos + glm::vec2(d, 0.0f), true);
	addSprite(a_sprite, "a", keypos + glm::vec2(0.0f, d), true);
	addSprite(s_sprite, "s", keypos + glm::vec2(d, d), true);
	addSprite(d_sprite, "d", keypos + glm::vec2(2*d,d), true);

	TextSprite* idSprite = new TextSprite("0", glm::vec3(0), glm::vec2(0), 20.0f, AgkHAlign::right, AgkVAlign::top);
	addSprite(idSprite, "ID", glm::vec2(frameElSize.x - 3.0f, 8.0f), true);
	idSprite->setDepth(11);
	
	TextSprite* lifeSprite = new TextSprite("0", glm::vec3(0), glm::vec2(0), 15.0f);
	addSprite(lifeSprite, "life", glm::vec2(40.0f, 10.f), true);
	lifeSprite->setDepth(11);

	TextSprite* distSprite = new TextSprite("0", glm::vec3(0), glm::vec2(0), 15.0f);
	addSprite(distSprite, "dist", glm::vec2(40.0f, 20.f), true);
	distSprite->setDepth(11);

	TextSprite* visCellsSprite = new TextSprite("0", glm::vec3(0), glm::vec2(0), 15.0f);
	addSprite(visCellsSprite, "cells", glm::vec2(70.0f, 20.f), true);
	visCellsSprite->setDepth(11);
}

void NNCarInfoFrameElement::reset() {
	sprites["background"]->setColor(glm::vec3(0.8f));
}
void NNCarInfoFrameElement::markDead() {
	sprites["background"]->setColor(glm::vec3(0.2f));
}
void NNCarInfoFrameElement::toogleSelected() {
	AgkSprite* hlsprite[5] = { sprites["border"] , sprites["ID"],
	sprites["life"], sprites["dist"], sprites["cells"] };
	
	glm::vec3 col;
	if (!isSelected) {
		isSelected = true;
		col = glm::vec3(1, 0.2, 0.2);
	}
	else {
		isSelected = false;
		col = glm::vec3(0.2f);
	}

	for (int i = 0; i < 5; i++) {
		hlsprite[i]->setColor(col);
	}

}

void NNCarInfoFrameElement::setId(int id) {
	std::string idstr = std::to_string(id);
	TextSprite* ts = (TextSprite*)sprites["ID"];
	ts->setText(&idstr[0]);
}

void NNCarInfoFrameElement::setLife(float life) {
	std::string lifestr = TOSTR(100.0f*life, 0) + "%";
	TextSprite* ts = (TextSprite*)sprites["life"];
	ts->setText(&lifestr[0]);
}

void NNCarInfoFrameElement::setDistance(float dist) {
	std::string diststr = TOSTR(dist, 0);
	TextSprite* ts = (TextSprite*)sprites["dist"];
	ts->setText(&diststr[0]);
}

void NNCarInfoFrameElement::setVisitedCells(int n) {
	std::string cellstr = TOSTR(n, 0);
	TextSprite* ts = (TextSprite*)sprites["cells"];
	ts->setText(&cellstr[0]);
}

bool NNCarInfoFrameElement::hitTest(const glm::vec2& pos) {
	Sprite* bck = (Sprite*)sprites["background"];
	return bck->hitTest(pos);
}

void NNCarInfoFrameElement::postProcess() {

	if(wflashed)
		w_sprite->setColor(glm::vec3(1, 1, 0));
	if (aflashed)
		a_sprite->setColor(glm::vec3(1, 1, 0));
	if (sflashed)
		s_sprite->setColor(glm::vec3(1, 1, 0));
	if (dflashed)
		d_sprite->setColor(glm::vec3(1, 1, 0));

	wflashed = aflashed = sflashed = dflashed = false;
}