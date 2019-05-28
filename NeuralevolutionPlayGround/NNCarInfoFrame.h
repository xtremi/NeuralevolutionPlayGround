#pragma once
#include "UI/UIFrame.h"
#include "convinient_functions.h"

#define LETTER_KEY_SIZE			8.0f
#define LETTER_KEY_TEXT_SIZE	6.0f

class NNCarInfoFrameElement : public UIFrame
{
public:
	NNCarInfoFrameElement(const glm::vec2& frameElSize);
	void wflash();
	void aflash();
	void sflash();
	void dflash();

	void setId(int id);
	void setLife(float life);
	void setDistance(float dist);
	void setVisitedCells(int n);

	void toogleSelected();

	bool hitTest(const glm::vec2& pos);

	void reset();
	void markDead();

private:
	SpriteAndText* w_sprite = NULL;
	SpriteAndText* a_sprite = NULL;
	SpriteAndText* s_sprite = NULL;
	SpriteAndText* d_sprite = NULL;

	bool isSelected = false;

	bool wflashed = false;
	bool aflashed = false;
	bool sflashed = false;
	bool dflashed = false;

	void postProcess();
};

class NNCarInfoFrame : public UIFrame 
{
public:
	NNCarInfoFrame(const glm::vec2& pos, const glm::vec2& _frameElSize, int _nrows);

	NNCarInfoFrameElement* addFrameElement();

	

private:
	int nframes = 0;
	int nrows = 0;
	glm::vec2 botpos;
	glm::vec2 frameElSize;

};



