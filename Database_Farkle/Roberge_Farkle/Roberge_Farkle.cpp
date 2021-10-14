// Roberge_Farkle.cpp : This is a dice game that is played with more than 2 players.
// The rules are displayed at the start of the game 
// Each player takes turns until one of the players reaches 10,000 points.
// At that time, each other player gets one additional turn to try and surpass that player.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include "Gameplay.h"
#include "Player.h"
#include "sqlite3.h"
#include "highScore.h"

using namespace std;

int main()
{
	Gameplay game;
	int numPlayers, i = 0, j = 0, turnScore;
	bool gameOver = false;
	using PlayerVector = vector<Player>;
	string playerName;

	HighScore highScoreDb;

	highScoreDb.openDb();
	highScoreDb.createTable();

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

		char choice = 'n';

		while (choice == 'n')
		{
			cout << "\nThis player has these stats. If there are none, you are a new player...Welcome!!\n\n";

			highScoreDb.getAll(playerName);
			cout << "\nWould you like to continue as this player? y or n\n";
			cin >> choice;

			while (choice != 'y' && choice != 'n')
			{
				cout << "Invalid choice\n";
				cout << "Would you like to continue as this player? y or n\n";
				cin >> choice;
			}
			if (choice == 'n')
			{
				cout << endl << "What is the name of player " << i + 1 << "?\n";
				cin >> playerName;
			}
			else
			{
				break;
			}
		}

		game_players[i].setName(playerName);
		highScoreDb.addPlayer(playerName);

		i++;
	}

	// close database until needed
	highScoreDb.closeDb();

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

	Player winningPlayer = game.calculateWinner(game_players);
	cout << "Congratulations!! " << winningPlayer.getName() << " is our winner!!\n\n";

	// Reopen database to update
	highScoreDb.openDb();

	for (i = 0; i < numPlayers; i++)
	{
		int currentHighScore = 0;
		string name = game_players[i].getName();
		int score = game_players[i].getScore();
		string winners_name = winningPlayer.getName();

		game.updateDatabase(highScoreDb, name, score, winners_name);		

	}

	// deallocate memory from vector
	PlayerVector().swap(game_players);

	highScoreDb.closeDb();
	system("pause");

    return 0;
}


