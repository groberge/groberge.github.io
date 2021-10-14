#pragma once

#include "stdafx.h"
#include "sqlite3.h"
#include <string>

class HighScore
{
private:
	sqlite3* db = NULL;
	char* zErrMsg = 0;
	int rc = 0;

public:
	HighScore();
	void openDb();
	void createTable();
	void closeDb();
	void addPlayer(const std::string& playername);
	void updateStats(const std::string& column, int value, std::string name);
	int getStats(const std::string& field, std::string name);
	void getAll(const std::string& name);
};

