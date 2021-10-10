#include "stdafx.h"
#include "Player.h"


Player::Player()
{
	winningPlayer = false;
	playerScore = 0;
}

void Player::setName(string name)
{
	playerName = name;
}

string Player::getName()
{
	return playerName;
}

void Player::setWinner(bool winner)
{
	winningPlayer = winner;
}

bool Player::getWinner()
{
	return winningPlayer;
}

void Player::setScore(int score)
{
	playerScore = score;
}

int Player::getScore()
{
	return playerScore;
}