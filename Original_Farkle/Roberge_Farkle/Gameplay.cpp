#include "stdafx.h"
#include "Gameplay.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <ctime>

using namespace std;

Gameplay::Gameplay()
{

}

// This function takes no arguments but returns the number of players 
int Gameplay::numberPlayers()
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

// This function takes the name of a file and displays it on the console
void Gameplay::displayRules(string fileName)
{
	ifstream fileData(fileName);
	string line;

	while (getline(fileData, line))
	{
		cout << line << endl;
	}

	cout << endl << endl;
}

/* This function takes a reference to the current player from the vector in main()
 *
 * It uses other private functions of the Gameplay class along with the Dice struct
 * to roll the dice, confirm that there are valid scoring dice, prompts for dice to save
 * and calculate scoring.
 *
 * The function returns the score for this turn
 */  
int Gameplay::takeTurn(Player &currentPlayer)
{
	
	bool rollOver = false, turnOver = false;
	char roll;
	int numberDice = 6, rollScore = 0, keep = 0, scoreDice = 0;
	using DiceVector = vector<Dice>;
	

	while (!turnOver)
	{
		cout << "Press r to roll or q to stop\n";
		cin >> roll;
		cout << endl;

		rollOver = false;

		//force another roll if necessary due to score being less than 1000 or 
		//all dice being saved
		if (((currentPlayer.getScore() + rollScore) < 1000) || (numberDice == 0))
		{
			roll = 'r';
		}

		while ((roll == 'q' || roll == 'r') && !rollOver)
		{
			if (roll == 'q')
			{
				turnOver = true;
				rollOver = true;
				cout << "You earned " << rollScore << " points on this turn\n\n";
			}
			else if (roll == 'r')
			{
				//safety to enforce correct values
				scoreDice = 0;
				keep = 0;

				//if all dice were saved, roll all six
				if (numberDice == 0)
				{
					numberDice = 6;
				}

				//vector to hold rolled dice
				DiceVector dice(numberDice);

				for (int i = 0; i < numberDice; i++)
				{
					int value = diceRoll();
					dice[i].number = (i + 1);
					dice[i].value = value;
					cout << "Die " << dice[i].number << ":  " << dice[i].value << endl;
				}

				//if no scoring dice, player's turn is over
				if (!scoringDice(dice))
				{
					turnOver = true;
					rollOver = true;
					rollScore = 0;
					cout << "There are no scoring dice on this roll.\n" << "All points for this turn are lost\n\n";
				}
				else
				{
					cout << endl << "Available scoring dice are:" << endl;

					for (int i = 0; i < numberDice; i++)
					{
					
						if (dice[i].scoringDie == true)
						{
							scoreDice++;
							cout << "Die " << dice[i].number << ":  " << dice[i].value << endl;
						}
					}

					while (keep != 9 && scoreDice > 0)
					{
						cout << endl << "Please select the number of the dice you'd like to keep. Choose the first die in a triplet to save all three.\n" << "Press 9 when finished\n";
						cin >> keep;

						//verify that input is valid
						if (!cin)
						{
							cin.clear();
							cin.ignore();

							cout << "Invalid entry\n";
							cout << "Please select the number of the dice you'd like to keep. Choose the first die in a triplet to save all three.\n" << "Press 9 when finished\n";
							cin >> keep;
						}
						else if (keep > 0 && keep < 7)
						{
							//confirm chosen die is scoring die
							if (dice[keep - 1].scoringDie == true)
							{
								dice[keep - 1].saved = true;
								scoreDice--;

								//if chosen die is part of a triplet, save others in the triplet
								if (dice[keep - 1].matched == true)
								{
									int matched = 1;

									for (int j = 0; j < numberDice; j++)
									{
										if ((dice[j].matched == true && !dice[j].saved) && 
											(dice[keep - 1].value == dice[j].value) && matched != 3)
										{
											dice[j].saved = true;
											matched++;
											scoreDice--;
										}
									}
								}

								cout << endl << "Dice " << dice[keep - 1].number << " and any triplets have been saved." << endl << endl;
								displayDice(dice);
							}
							else
							{
								cout << "That is not a scoring die.  Please choose again\n";
							}

							if (scoreDice == 0)
							{
								cout << "There are no more scoring dice.\n";
							}
						}

						//display score for roll and game
						rollScore += calculateRollScore(dice);
						cout << "Your current total for this turn is " << rollScore << endl << endl;
						cout << "If you stop now, your game score would be " << (currentPlayer.getScore() + rollScore) << ".\n";

						if ((currentPlayer.getScore() + rollScore) < 1000)
						{
							cout << "You must have over 1000 game or roll points to stop.\n" << "You must continue rolling.\n\n";
						}
					}
				}

				//find how many dice left to roll
				int diceThisRoll = numberDice;
				for (int i = 0; i < diceThisRoll; i++)
				{
					if (dice[i].saved == true)
					{
						numberDice--;
					}

					if (numberDice == 0)
					{
						cout << "All dice have been saved.\n" << "You must continue rolling.\n";
					}
				}

				rollOver = true;
				
			}
		}
	}

	return rollScore;

}

// This function performs the dice roll and returns the value for an individual die
int Gameplay::diceRoll() 
{

	int roll = 0;

	roll = rand() % 6 + 1;

	return roll;
}

// This function takes a reference to a vector of Dice and detects if there are any 
// triplets and returns a bool as true if there are
bool Gameplay::threeOfAKind(vector<Dice> &allDice)
{
	int match = 0;
	int tripleValue = 0;
	bool triple = false;
	int numDice = allDice.size();

	for (int i = 0; i < numDice; i++)
	{
		match = 0;
		Dice &d = allDice[i];

		if (!d.matched) {

			for (int j = i + 1; j < numDice; j++)
			{
				Dice &e = allDice[j];

				if (!e.matched)
				{ 
					if (d.value == e.value)
					{
						match++;

						//only take the first three matched values
						if (match >= 2)
						{
							tripleValue = d.value;
							d.scoringDie = true;
							d.matched = true;
							triple = true;
							
							//only set the scoring value on the first one
							if (d.value == 1)
							{
								d.scoringValue = 1000;
							}
							else
							{
								d.scoringValue = (d.value * 100);
							}

							//set matched and scoring for the next two dice after the scoring die
							for (int k = 0; k < numDice; k++)
							{
								Dice &f = allDice[k];

if ((f.value == tripleValue) && (match >= 0))
								{
									f.scoringDie = true;
									f.matched = true;
									match--;
								}
							}
						}
					}
				}
			}
		}
	}

	return triple;
}

// This function takes a reference to a vector of Dice and finds and marks all non-triplet 
// scoring dice and returns a bool of true if scoring dice are found
bool Gameplay::scoringDice(vector<Dice> &allDice)
{
	bool scoring = false;
	int numDice = allDice.size();

	if (threeOfAKind(allDice))
	{
		scoring = true;
	}

	for (int i = 0; i < numDice; i++)
	{
		Dice &d = allDice[i];

		if (!d.matched)
		{
			switch (d.value) {
			case 1 :
				scoring = true;
				d.scoringDie = true;
				d.scoringValue = 100;
				break;
			case 5:
				scoring = true;
				d.scoringDie = true;
				d.scoringValue = 50;
				break;
			default:
				d.scoringDie = false;
				d.scoringValue = 0;
			}
		}
	}

	return scoring;
}

// This function takes a reference to a vector of Dice and calculates the score of the roll 
// by getting the scoring value of all saved dice and returns an integer value
int Gameplay::calculateRollScore(vector<Dice> &allDice)
{
	int rollScore = 0;
	int numDice = allDice.size();

	for (int i = 0; i < numDice; i++)
	{
		Dice &d = allDice[i];

		if (d.saved && !d.calculated)
		{
			rollScore += d.scoringValue;
			d.calculated = true;
		}
	}

	return rollScore;
}

// This function takes a reference to a vector of Dice and displays the remaining dice
// that the user can choose to save
void Gameplay::displayDice(vector<Dice>& allDice)
{
	int numDice = allDice.size();


	cout << "Remaining dice to choose from:\n";

	for (int i = 0; i < numDice; i++)
	{
		if (!allDice[i].saved)
		{
			cout << "Die " << allDice[i].number << ":  " << allDice[i].value << endl;
		}
	}
}

// This function takes a reference to a Player to see if their score if over 10,000
// It doesn't return a value but sets the winner bit if true
void Gameplay::winningPlayer(Player &currentPlayer)
{
	if (currentPlayer.getScore() > 10000)
	{
		currentPlayer.setWinner(true);
	}
}

// This function takes a reference to a vector of Players to find who had the highest
// score and displays the winner
void Gameplay::calculateWinner(vector<Player> &allPlayers)
{
	int playerSize = allPlayers.size();
	Player winner = allPlayers[0];

	for (int i = 0; i < playerSize; i++)
	{
		if (allPlayers[i].getScore() > winner.getScore())
		{
			winner = allPlayers[i];
		}
	}

	cout << "Congratulations!  The winner is: " << winner.getName() << "!\n";
}
