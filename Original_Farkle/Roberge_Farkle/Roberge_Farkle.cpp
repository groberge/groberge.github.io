// Roberge_Farkle.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include "Gameplay.h"
#include "Player.h"

int main()
{
	Gameplay game;
	int numPlayers, i = 0, j = 0, rollScore = 0, previousScore = 0;
	bool gameOver = false, rollOver = false;
	using PlayerVector = vector<Player>;
	string playerName;

	//used in diceRoll()
	srand(time(0));

	game.displayRules("FarkleRules.txt");
	numPlayers = game.numberPlayers();

	//create game players
	PlayerVector play(numPlayers);

	while (i < numPlayers)
	{
		cout << endl << "What is the name of player " << i + 1 << "?\n";
		cin >> playerName;

		play[i].setName(playerName);
		i++;
	}

	while (gameOver == false)
	{

		if (play[j].getWinner() == false)
		{
			//current player takes their turn
			cout << "\n" << play[j].getName() << "'s turn\n\n";
			rollScore = game.takeTurn(play[j]);

			//set score after turn
			previousScore = play[j].getScore();
			play[j].setScore(previousScore + rollScore);
			
			for (int k = 0; k < numPlayers; k++)
			{
				cout << play[k].getName() << " has " << play[k].getScore() << " points.\n";
			}

			//check if player's score if over 10,000
			//if yes, set winner bit
			game.winningPlayer(play[j]);

			j++;

			if (j >= numPlayers)
			{
				j = 0;
			}

		}
		else
		{
			//if we've gotten here, all players have had a turn
			//after the first person went over 10,000
			gameOver = true;
		}
	}

	game.calculateWinner(play);

    return 0;
}


