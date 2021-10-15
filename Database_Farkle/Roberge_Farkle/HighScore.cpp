#include "stdafx.h"
#include "highScore.h"
#include "Player.h"
#include <iostream>
using namespace std;


HighScore::HighScore()
{

}

/*	Opens the database
*	Input:
*	- None
*	Output:
*	- None
*/
void HighScore::openDb()
{
	rc = sqlite3_open("highScore.db", &db);

	if (rc)
	{
		cout << "Can't open database\n" << sqlite3_errmsg(db);
	}
}

/*	Creates the table to hold the players if it doesn't already exist
*	Input:
*	- None
*	Output:
*	- None
*/
void HighScore::createTable()
{
	char* error;

	// Check if table exists prior to creating it
	const char* sqlCreateTable = "CREATE TABLE IF NOT EXISTS Players (id INTEGER PRIMARY KEY AUTOINCREMENT, name nvarchar(50) not null, highscore int, wins int, losses int, CONSTRAINT name_unique UNIQUE (name))";
	rc = sqlite3_exec(db, sqlCreateTable, NULL, NULL, &error);
	if (rc)
	{
		cout << "Can't create table\n" << sqlite3_errmsg(db);
	}
}

/*	Closes the database
*	Input:
*	- None
*	Output:
*	- None
*/
void HighScore::closeDb()
{
	rc = sqlite3_close(db);
	if (rc)
	{
		cout << "Can't close database\n" << sqlite3_errmsg(db);
	}
}

/*	Adds a player to the database if they don't already exist
*	Input:
*	- reference to the player's name
*	Output:
*	- None
*/
void HighScore::addPlayer(const string& playername)
{
	char* error;
	string command;

	// If the player already exists, the conflict will be ignored and player will not be added
	command = "INSERT OR IGNORE INTO Players (name, highscore, wins, losses) VALUES ('" + playername + "', 0, 0, 0)";
	rc = sqlite3_exec(db, command.c_str(), NULL, NULL, &error);
	if (rc)
	{
		cout << "Player creation error\n" << sqlite3_errmsg(db);
	}
}

/*	Updates the highscore, number of wins or number of losses for a player
*	Input:
*	- field to update
*	- value for highscore, wins or losses
*	- player's name
*	Output:
*	- None
*/
void HighScore::updateStats(const string& field, int value, string name)
{
	sqlite3_stmt* res;
	char* sql = NULL;

	// translate string to int value
	int column;

	if (field == "highScore")
	{
		column = 3;
	}
	else if (field == "wins")
	{
		column = 4;
	}
	else if (field == "losses")
	{
		column = 5;
	}
	else
	{
		cout << "Invalid field";
		return;
	}

	// Set the required field
	switch (column)
	{
	case 3:
		sql = "UPDATE Players SET highscore = ? WHERE name = ?";
		break;
	case 4:
		sql = "UPDATE Players SET wins = ? WHERE name = ?";
		break;
	case 5:
		sql = "UPDATE Players SET losses = ? WHERE name = ?";
		break;
	}

	// Using prepare_v2 and bind to create the sql statement using variants

	// Compile the sql statement
	rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

	if (rc == SQLITE_OK) {

		// Bind the variables into the variants in the query
		if (sqlite3_bind_int(res, 1, value) != SQLITE_OK)
		{
			cout << "Could not bind integer\n" << sqlite3_errmsg(db) << endl;
		}
		if (sqlite3_bind_text(res, 2, name.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
		{
			cout << "Count not bind name\n" << sqlite3_errmsg(db) << endl;
		}
		// Evaluate the statement
		if (sqlite3_step(res) != SQLITE_DONE)
		{
			cout << "Could not execute statement in update stats\n" << sqlite3_errmsg(db) << endl;
		}
	}
	else {

		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
	}
	sqlite3_finalize(res);

}

/*	Gets the current high score, wins or losses for a player
*	Input:
*	- field to get data from
*	- player's name
*	Output:
*	- value of the requested field
*/
int HighScore::getStats(const string& field, string name)
{
	sqlite3_stmt* res;
	int value;

	char* sql = NULL;

	// translate string to int value
	int column;

	if (field == "highScore")
	{
		column = 3;
	}
	else if (field == "wins")
	{
		column = 4;
	}
	else if (field == "losses")
	{
		column = 5;
	}
	else
	{
		cout << "Invalid field";
		return 0;
	}

	// Set the required field
	switch (column)
	{
	case 3:
		sql = "SELECT highscore from Players WHERE name = ?";
		break;
	case 4:
		sql = "SELECT wins from Players WHERE name = ?";
		break;
	case 5:
		sql = "SELECT losses from Players WHERE name = ?";
		break;
	}
	// Compile the sql statement
	rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

	if (rc == SQLITE_OK) {

		// Bind the variable into the variant in the query
		if (sqlite3_bind_text(res, 1, name.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
		{
			cout << "Count not bind playername" << sqlite3_errmsg(db) << endl;
		}
		// Evaluate the statement
		sqlite3_step(res);
	}
	else {

		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
	}
	value = sqlite3_column_int(res, 0);
	sqlite3_finalize(res);
	return value;

}

/*	Used to get the results from SELECT queries
*	Input:
*	- pointer to data provided in fourth argument in sqlite3_exec statement
*	- number of columns in a row
*	- array of pointers to strings to represent the fields in a row
*	- array of pointers to strings to represent column names
*	Output:
*	- None
*/
static int callback(void* data, int argc, char** argv, char** azColName)
{
	int i;

	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}

	printf("\n");
	return 0;
}

/*	Query the database for highscore, wins and losses
*	Input:
*	- reference to player's name
*	Output:
*	- None
*/
void HighScore::getAll(const std::string& name)
{
	char* error = NULL;
	string sql = "SELECT highscore,wins,losses from Players WHERE name = '" + name + "'";

	// Run the sqlite query above
	sqlite3_exec(db, sql.c_str(), callback, NULL, &error);

	if (error)
	{
		cout << "Error during query: " << error << endl;

		// Free the memory used by error
		sqlite3_free(error);
	}


}


