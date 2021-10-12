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
	int numPlayers, i = 0, j = 0, turnScore;
	bool gameOver = false;
	using PlayerVector = vector<Player>;
	string playerName;

	//used in diceRoll()
	srand(time(0));

	game.displayRules("FarkleRules.txt");
	numPlayers = game.numberPlayers();

	//create game players
	PlayerVector game_players(numPlayers);

	while (i < numPlayers)
	{
		cout << endl << "What is the name of player " << i + 1 << "?\n";
		cin >> playerName;

		game_players[i].setName(playerName);
		i++;
	}

	while (gameOver == false)
	{

		if (game_players[j].getWinner() == false)
		{
			//current player takes their turn
			cout << "\n\n*****************************\n";
			cout << game_players[j].getName() << "'s turn\n";
			cout << "*****************************\n\n";
			
			turnScore = game.takeTurn(game_players[j], game_players);

			//set score after turn
			game_players[j].setScore(game_players[j].getScore() + turnScore);

			//display points
			for (int k = 0; k < numPlayers; k++)
			{
				cout << game_players[k].getName() << " has " << game_players[k].getScore() << " points.\n";
			}

			//check if player's score if over 10,000
			//if yes, set winner bit
			game.winningPlayer(game_players[j]);

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

	cout << "Congratulations!! " << game.calculateWinner(game_players).getName() << " is our winner!!\n";

	// deallocate memory from vector
	PlayerVector().swap(game_players);

    return 0;
}


