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
		char *sql;

		DBOpen();

		/* Create SQL statement */
		sql = "DROP TABLE TRANSACTIONS; "  \
		    "CREATE TABLE TRANSACTIONS("  \
			"ID INT PRIMARY KEY      NOT NULL," \
			"DATE           TEXT     NOT NULL," \
			"TYPE           TEXT     NOT NULL," \
			"DESCRIPTION    TEXT     NOT NULL," \
			"VALUE          REAL     NOT NULL," \
			"BALANCE        REAL     NOT NULL," \
			"ACCOUNTNAME    CHAR(50) NOT NULL," \
			"ACCOUNTNUMBER  CHAR(50) NOT NULL);";

		/* Execute SQL statement */
		rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
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
		int count = transactions.size();

		char *zErrMsg = 0;
		int rc;
		char sql[1024];
		char sqlBlock[1024*101];
		long blockSize = 1024*100; // sqlite3.c(10672):# define SQLITE_MAX_SQL_LENGTH 1000000000

		DBOpen();

		strcpy_s(sqlBlock, "INSERT INTO TRANSACTIONS (ID,DATE,TYPE,DESCRIPTION,VALUE,BALANCE,ACCOUNTNAME,ACCOUNTNUMBER) VALUES ");
		for (int i = 0; i < count; ++i)
		{
			Transaction& t = transactions[i];

			// Create SQL statement
			sprintf_s(sql, "(%d,'%s','%s','%s',%f,%f,'%s','%s'), ", i+1, t.date.c_str(), t.type.c_str(), t.description.c_str(), t.value, t.balance, t.accountName.c_str(), t.accountNumber.c_str());
			strcat_s(sqlBlock, sql);

			if (strlen(sqlBlock) > blockSize || i == count-1)
			{
				sqlBlock[strlen(sqlBlock)-2] = ';';
				
				// Execute SQL statement
				rc = sqlite3_exec(db, sqlBlock, NULL, 0, &zErrMsg);
				if (rc != SQLITE_OK) {
					fprintf(stderr, "SQL error: %s\n", zErrMsg);
					sqlite3_free(zErrMsg);
				}
				else {
					fprintf(stdout, "Tranactions imported successfully\n");
				}

				strcpy_s(sqlBlock, "INSERT INTO TRANSACTIONS (ID,DATE,TYPE,DESCRIPTION,VALUE,BALANCE,ACCOUNTNAME,ACCOUNTNUMBER) VALUES ");
			}
		}

		DBClose();
		return count;
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