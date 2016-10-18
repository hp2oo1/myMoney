#include "TransactionRepository.h"

#include "minicsv.h"
#include "sqlite3.h"

#include <stdio.h>
#include <ctype.h>
#include <sstream>
#include <algorithm>

namespace myMoneyLib
{
	/////////////////////
	// local functions //
	/////////////////////

	void replaceAll(std::string& str, const std::string& from, const std::string& to) {
	    if(from.empty())
	        return;
	    size_t start_pos = 0;
	    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
	        str.replace(start_pos, from.length(), to);
	        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	    }
	}

	int callback(void *data, int argc, char **argv, char **azColName)
	{
		std::stringstream ss;
		for (int i = 1; i < argc; ++i)
		{
			ss << argv[i];
		}

		Transaction t;
		ss >> t.date >> t.type >> t.description >> t.value >> t.balance >> t.accountName >> t.accountNumber;

		std::vector<myMoneyLib::Transaction>* ts = reinterpret_cast<std::vector<myMoneyLib::Transaction>* >(data);			
		ts->push_back(t);

		return 0;
	}

	std::vector<Transaction> ImportCSV_(std::string filename)
	{
		std::vector<Transaction> transactions;

		// import a list of transactions to database
		mini::csv::ifstream is(filename.c_str());
		is.set_delimiter(',', "$$");
		is.enable_trim_quote_on_str(true, '\"');
		is.enable_terminate_on_blank_line(false);
		if (is.is_open())
		{
			Transaction t;
			bool isHeadLine = true;
			while (is.read_line())
			{
				// skip head line (first non-blank line)
				if (isHeadLine)
				{
					isHeadLine = false;
					is.read_line();
				}

				// load data line
				is >> t.date >> t.type >> t.description >> t.value >> t.balance >> t.accountName >> t.accountNumber;

				// remove first quote character
				t.description.erase(0, 1);
				t.accountName.erase(0, 1);
				t.accountNumber.erase(0, 1);
				// double single quote to make sqlite3 insert happy
				replaceAll(t.description, "'", "''");

				transactions.push_back(t);
			}
		}

		return transactions;
	}
	/////////////////////

	TransactionRepository::TransactionRepository()
	{
	}

	TransactionRepository::~TransactionRepository()
	{
	}

	void TransactionRepository::DBSetPath(std::string path)
	{
		dbPath = path;
	}

	void TransactionRepository::DBCreate()
	{
		char *zErrMsg = 0;
		int rc;
		std::string sql;

		DBOpen();

		/* Create SQL statement */
		
		// clear all
		sql = "DROP TABLE IF EXISTS TRANSACTIONS; "  \
			"DROP TABLE IF EXISTS CATEGORIES; "  \
			"DROP TABLE IF EXISTS TRANSACTION_CATEGORIES; ";

		// transactions table
		sql += "CREATE TABLE IF NOT EXISTS TRANSACTIONS("  \
			"TRANSACTION_ID INT PRIMARY KEY," \
			"DATE DATE NOT NULL," \
			"TYPE CHAR(10) NOT NULL," \
			"DESCRIPTION TEXT NOT NULL," \
			"VALUE REAL NOT NULL," \
			"BALANCE REAL NOT NULL," \
			"ACCOUNTNAME CHAR(255) NOT NULL," \
			"ACCOUNTNUMBER CHAR(255) NOT NULL); ";

		// categories table
		sql += "CREATE TABLE IF NOT EXISTS CATEGORIES(" \
			"CATEGORY_ID INT PRIMARY KEY," \
		    "NAME CHAR(255) NOT NULL); ";

		// transactions_categories table
		sql += "CREATE TABLE IF NOT EXISTS TRANSACTION_CATEGORIES(" \
			"TRANSACTION_ID INT PRIMARY KEY," \
			"CATEGORY_ID INT NOT NULL); ";

		/* Execute SQL statement */
		rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &zErrMsg);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else {
			fprintf(stdout, "Table created successfully\n");
		}

		DBClose();
	}

	void TransactionRepository::DBOpen()
	{
		char *zErrMsg = 0;
		int rc;

		/* Open database */
		rc = sqlite3_open(dbPath.c_str(), &db);
		if (rc) {
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		}
		else {
			fprintf(stderr, "Opened database successfully\n");
		}
	}

	void TransactionRepository::DBClose()
	{
		sqlite3_close(db);
	}

	int TransactionRepository::ImportCSV(std::string filename)
	{
		std::vector<Transaction> transactions = ImportCSV_(filename);
		int total = transactions.size();

		char *zErrMsg = 0;
		int rc;
		char sql[1024];
		char sqlBlock[1024*101];
		size_t blockSize = 1024*100; // sqlite3.c(10672):# define SQLITE_MAX_SQL_LENGTH 1000000000

		DBOpen();

		const char* sqlCommon = "INSERT INTO TRANSACTIONS (TRANSACTION_ID,DATE,TYPE,DESCRIPTION,VALUE,BALANCE,ACCOUNTNAME,ACCOUNTNUMBER) VALUES ";

		strcpy_s(sqlBlock, sqlCommon);
		for (int i = 0; i < total; ++i)
		{
			Transaction& t = transactions[i];
			
			// Convert to SQL date format
			int dd, mm, yyyy;
			sscanf_s(t.date.c_str(), "%02d/%02d/%04d", &dd, &mm, &yyyy);
			char date[11];
			sprintf_s(date, "%04d-%02d-%02d", yyyy, mm, dd);

			// Create SQL statement
			sprintf_s(sql, "(%d,'%s','%s','%s',%f,%f,'%s','%s'), ", i+1, date, t.type.c_str(), t.description.c_str(), t.value, t.balance, t.accountName.c_str(), t.accountNumber.c_str());
			strcat_s(sqlBlock, sql);

			if (strlen(sqlBlock) > blockSize || i == total-1)
			{
				sqlBlock[strlen(sqlBlock)-2] = ';';
				
				// Execute SQL statement
				rc = sqlite3_exec(db, sqlBlock, NULL, 0, &zErrMsg);
				if (rc != SQLITE_OK) {
					fprintf(stderr, "SQL error: %s\n", zErrMsg);
					sqlite3_free(zErrMsg);
					total = -1;
					break;
				}
				else {	
					fprintf(stdout, "Tranactions imported successfully\n");
				}

				strcpy_s(sqlBlock, sqlCommon);
			}
		}

		DBClose();
		return total;
	}

	std::vector<Transaction> TransactionRepository::SearchTransactions(std::string searchColumn, std::string searchTerm)
	{
		std::vector<myMoneyLib::Transaction> results;

		char *zErrMsg = 0;
		int rc;
		char sql[4096];

		DBOpen();

		/* Create SQL statement */
		sprintf_s(sql, "SELECT * FROM TRANSACTIONS WHERE %s LIKE '%s%s%s'", searchColumn.c_str(), "%", searchTerm.c_str(), "%");
		
		/* Execute SQL statement */
		rc = sqlite3_exec(db, sql, callback, &results, &zErrMsg);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else {
			fprintf(stdout, "Tranactions searched successfully\n");
		}

		DBClose();
		return results;
	}
}