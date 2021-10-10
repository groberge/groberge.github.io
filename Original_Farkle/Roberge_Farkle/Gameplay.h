#pragma once

#include "Player.h"
#include <vector>

class Gameplay
{
private:
	int numberDice, diceKeep, rollScore;

	struct Dice
	{
		int value;
		int number;
		int scoringValue = 0;
		bool saved = false, scoringDie = false, matched = false, calculated = false;
	};

	int diceRoll();
	bool threeOfAKind(vector<Dice>& allDice);
	bool scoringDice(vector<Dice>& allDice);
	int calculateRollScore(vector<Dice>& allDice);
	void displayDice(vector<Dice>& allDice);

public:
	void displayRules(string);
	Gameplay();
	int numberPlayers();
	int takeTurn(Player& p);
	void winningPlayer(Player& p);
	void calculateWinner(vector<Player>& allPlayers);
};