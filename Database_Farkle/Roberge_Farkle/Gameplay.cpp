#include "stdafx.h"
#include "Gameplay.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <ctime>
#include <chrono>

using namespace std::chrono;
using namespace std;


Gameplay::Gameplay()
{

}

/*	Gets the number of players
*	Input:
*	- none
*	Output:
*	- number of players for the game
*/int Gameplay::numberPlayers()
{
	int numPlayers = 0;

	cout << "How many players will be playing Farkle?\n" << "Please choose a number greater than 2.\n";
		
	cin >> numPlayers;
	
	//verify that the entry is valid
	while (!cin  || (numPlayers < 3))
	{
		cin.clear();
		cin.ignore();
		
		cout << "Invalid entry\n";
		cout << "How many players will be playing Farkle?\n"  << "Please choose a number greater than 2.\n";
		cin >> numPlayers;
	}

	return numPlayers;
}

/*	Prompts the player to roll or quit
*	Input:
*	- reference to current player
*	- vector of players
*	- reference to the current turnScore
*	- reference to the number of dice being rolled
*	Output:
*	- char to represent whether the player is rolling or not
*/
char Gameplay::promptForRoll(Player& currentPlayer, vector<Player> gamePlayers, const int& turnScore, const int& numberDice)
{
	char roll = 'r';

	if (numberDice == 6)
	{
		cout << "First roll of turn.  Can't skip turn\n\n";
		roll = 'r';
	}
	else if (numberDice == 0)
	{
		// if the player score is > winning score, check to make sure that they've got the highest score and 
		// if so, give the option to quit.
		if (currentPlayer.getScore() >= winningScore)
		{
			if (currentPlayer.getName() == calculateWinner(gamePlayers).getName())
			{
				cout << "You're in the lead.  Do you want to keep rolling?\n";
				cout << "Press r to keep rolling or q to quit\n";
				cin >> roll;

				while (!cin || !(roll == 'r' || roll == 'q'))
				{
					cin.clear();
					cin.ignore();
					cout << "Invalid input\n";
					cout << "Press r to keep rolling or q to quit\n";
					cin >> roll;
				}
			}
		}
		else
		{
			cout << "All dice have been saved.  You must keep rolling\n\n";
			roll = 'r';
		}
	}
	// user must roll until they hit 1000 to get into the game
	else if ((currentPlayer.getScore() + turnScore) < 1000)
	{
		cout << "Score is less than 1000.  You must keep rolling\n";
		roll = 'r';
	}
	else
	{
		cout << "Press r to keep rolling or q to quit\n";
		cin >> roll;

		while (!cin || !(roll == 'r' || roll == 'q'))
		{
			cin.clear();
			cin.ignore();
			cout << "Invalid input\n";
			cout << "Press r to keep rolling or q to quit\n";
			cin >> roll;
		}
	}

	return roll;
}

/*	Performs the actions of a player turn in the game
*	Input:
*	- reference to current player
*	- constanst reference to the vector of players
*	Output:
*	- the score earned during the turn
*/
int Gameplay::takeTurn(Player& currentPlayer, const vector<Player>& gamePlayers)
{
	int numberDice = 6, rollScore, turnScore = 0;
	vector<int> dice;

	while (true)
	{
		// ask the user if they want to roll
		char roll = promptForRoll(currentPlayer, gamePlayers, turnScore, numberDice);

		if (roll == 'q')
		{
			cout << "\n" << currentPlayer.getName() << "'s turn has ended\n\n";
			return turnScore;
		}
		else if (roll == 'r')
		{	
			cout << "Rolling dice\n";

			// if all dice have been saved, reset number of dice to six
			if (numberDice == 0)
			{
				numberDice = 6;
			}
			dice = diceRoll(numberDice);
			
			// calculate the score for this roll
			pair<int,int> scoreAndDice = scoreDice(dice, numberDice);
			rollScore = scoreAndDice.first;
			numberDice = scoreAndDice.second;

			if (rollScore == 0)
			{
				turnScore = 0;
				cout << "No scoring dice" << endl << "End of " << currentPlayer.getName() << "'s turn\n\n";
				return turnScore;
			}
			else
			{
				turnScore += rollScore;
				cout << "\nYour current roll score is " << turnScore << endl;
				cout << "If you stop rolling now, your total would be " << currentPlayer.getScore() + turnScore << endl << endl;
			}
			
		}
	}		

	return turnScore;
}


/*	Calculates the score for the current roll and removes any dice that are kept
*	Input:
*	- vector of integers representing the dice
*	- the number of dice being used
*	Output:
*	- a pair of integers representing the score on this roll and the number of dice remaining for the next roll
*/
pair<int, int> Gameplay::scoreDice(vector<int> dice, int numDice)
{
	pair<int, int> scoreAndDice;
	int score = 0;
	char keep = 'n';

	// Display dice values with counts and return map of dice
	map<int,int> diceMap = displayDice(dice);

	// Iterate through map of dice and find scoring dice
	for (auto it = diceMap.begin(); it != diceMap.end(); ++it)
	{
		bool scoringDice = false;

		// first is the value of the die
		// second is the count of those values

		if (it->second == 6)
		{
			cout << "You have 2 triplets of " << it->first << "s!\n";
			scoringDice = true;
			cout << "Automatically saving all dice.\n";
			score += calculateTripletScore(it->first);
			numDice = 0;
		}
		else if (it->second >= 3)
		{
			cout << "You have a triplet of " << it->first << "s!\n";

			cout << "Would you like to keep this triplet?  y or n\n";
			cin >> keep;

			while (keep != 'y' && keep != 'n')
			{
				cout << "Invalid response. Please choose y or n\n";
				cin >> keep;
			}
			if (keep == 'y')
			{
				score += calculateTripletScore(it->first);
				numDice -= 3;
				cout << "You now have " << numDice << " dice remaining.\n\n";
			}
			// if there are more than three ones or fives, we need to score those also
			if (it->second > 3)
			{
				if (it->first == 1)
				{
					int ones = it->second - 3;
					cout << "You have " << ones << " additional ones!\n";

					cout << "Would you like to keep your 1s? y or n\n";
					cin >> keep;
					while (keep != 'y' && keep != 'n')
					{
						cout << "Invalid response. Please choose y or n\n";
						cin >> keep;
					}
					if (keep == 'y')
					{
						score += (ones * 100);
						numDice -= ones;
						cout << "You now have " << numDice << " dice remaining.\n\n";
					}
				}
				else if (it->first == 5)
				{
					int fives = it->second - 3;
					cout << "You have " << fives << " additional fives!\n";
					cout << "Would you like to keep your 5s? y or n\n";
					cin >> keep;
					while (keep != 'y' && keep != 'n')
					{
						cout << "Invalid response. Please choose y or n\n";
						cin >> keep;
					}
					if (keep == 'y')
					{
						score += (fives * 50);
						numDice -= fives;
						cout << "You now have " << numDice << " dice remaining.\n\n";
					}
				}
			}

			
			scoringDice = true;
		}
		// check for individual scoring dice
		else
		{
			if (it->first == 1)
			{
				cout << "Would you like to keep your " << it->second << " 1s? y or n\n";
				cin >> keep;
				while (keep != 'y' && keep != 'n')
				{
					cout << "Invalid response. Please choose y or n\n";
					cin >> keep;
				}
				if (keep == 'y')
				{
					score += (it->second * 100);
					numDice -= it->second;
					cout << "You now have " << numDice << " dice remaining.\n\n";
				}
			}			
			else if (it->first == 5)
			{
				cout << "Would you like to keep your " << it->second << " 5s? y or n\n";
				cin >> keep;
				while (keep != 'y' && keep != 'n')
				{
					cout << "Invalid response. Please choose y or n\n";
					cin >> keep;
				}
				if (keep == 'y')
				{
					score += (it->second * 50);
					numDice -= it->second;
					cout << "You now have " << numDice << " dice remaining.\n\n";
				}
			}
		}
	}
	// pair to hold the score for the roll and the number of dice left to roll
	scoreAndDice = make_pair(score, numDice);

	return scoreAndDice;	
}

/*	Displays the rules from a file
*	Input:
*	- name of the file to be displayed
*	Output:
*	- none
*/
void Gameplay::displayRules(string fileName)
{
	ifstream fileData(fileName);
	string line;

	if (fileData.fail())
	{
		cout << "File does not exist";
	}
	else
	{
		while (getline(fileData, line))
		{
			cout << line << endl;
		}
	}
	cout << endl << endl;
}

/*	Calculates the value of a triplet of dice
*	Input:
*	- the face value of the die in the triplet
*	Output:
*	- the scoring value of the triplet
*/
int Gameplay::calculateTripletScore(int triplet)
{
	int value;

	// three ones is 1000 points, all others are 100*value
	if (triplet == 1)
	{
		value = 1000;
	}
	else
	{
		value = (triplet * 100);
	}

	return value;
}

/*	Gets a random value between 1 and 6 for each of the dice and puts the value in a vector
*	Input:
*	- the number of dice to roll
*	Output:
*	- a vector of integers representing the number of dice rolled
*/
vector<int> Gameplay::diceRoll(int numberOfDice)
{
	vector<int> dice;
	for (int i = 0; i < numberOfDice; i++)
	{
		int roll = rand() % 6 + 1;
		dice.push_back(roll);
	}

	return dice;
}


/*	Iterates the vector of dice and counts the number of instances of each possible die value and adds them to a map
*	Input:
*	- reference to a vector of integers representing the dice
*	Output:
*	- a map of integers with the first value being the face value of the die and the second being the count of those values
*/
map<int,int> Gameplay::displayDice(vector<int>& dice)
{
	map<int, int> diceMap;

	for (int i = 1; i <= 6; i++)
	{
		int diceCount = count(dice.begin(), dice.end(), i);
		// if we have a number of dice with each number, add it to the diceMap
		if (diceCount > 0)
		{
			cout << "You have " << diceCount << " " << i << "s\n";
			diceMap.insert(pair<int, int>(i, diceCount));
		}
	}

	cout << endl;

	return diceMap;
}

/*	This function checks to see if the current player's score is above the winning score and set the winningPlayer bit to true if so
*	Input:
*	- reference to the current player
*	Output:
*	- none
*/
void Gameplay::winningPlayer(Player &currentPlayer)
{
	if (currentPlayer.getScore() >= winningScore)
	{
		currentPlayer.setWinner(true);
	}
}

/*	Calculates the player with the highest score
*	Input:
*	- reference to a vector of players
*	Output:
*	- the player with the highest score
*/
Player Gameplay::calculateWinner(vector<Player> &allPlayers)
{
	int playerSize = allPlayers.size();
	Player winner = allPlayers[0];

	for (int i = 1; i < playerSize; i++)
	{
		if (allPlayers[i].getScore() >= winner.getScore())
		{
			winner = allPlayers[i];
		}
	}

	return winner;
}

/*	Checks the database for high score and updates if needed. Also updates wins and losses
*	Input:
*	- reference to the database
*	- reference to the player's name
*	- reference to the player's score
*	- reference to the winner's name
*	Output:
*	- None
*/
void Gameplay::updateDatabase(HighScore& highScoreDb, const string& name, const int& score, const string& winners_name)
{
	int currentHighScore = highScoreDb.getStats("highScore", name);
	cout << "Current high score for " << name << " is " << currentHighScore << endl;
	if (score > currentHighScore)
	{

		highScoreDb.updateStats("highScore", score, name);
		cout << "Congratulations " << name << "!!  Your new high score is " << score << "!\n\n";
	}

	if (name == winners_name)
	{
		highScoreDb.updateStats("wins", (highScoreDb.getStats("wins", name) + 1), name);
	}
	else
	{
		highScoreDb.updateStats("losses", (highScoreDb.getStats("losses", name) + 1), name);
	}

	cout << name << "'s record is now " << highScoreDb.getStats("wins", name) << " wins and " << highScoreDb.getStats("losses", name) << " losses.\n\n";
}
