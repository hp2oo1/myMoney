#include "TransactionRepository.h"

#include "minicsv.h"
#include "sqlite3.h"

#include <stdio.h>
#include <ctype.h>

namespace myMoneyLib
{
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
		sql = "CREATE TABLE TRANSACTIONS("  \
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
		char *zErrMsg = 0;
		int rc;
		char sql[4096];

		// import a list of transactions to database
		mini::csv::ifstream is(filename.c_str());
		is.set_delimiter(',', "$$");
		is.enable_trim_quote_on_str(true, '\"');
		is.enable_terminate_on_blank_line(false);
		if (is.is_open())
		{
			DBOpen();

			Transaction t;
			bool isHeadLine = true;
			int count = 0;
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
				count++;

				t.description.erase(0, 1);
				t.accountName.erase(0, 1);
				t.accountNumber.erase(0, 1);

				/* Create SQL statement */
				sprintf_s(sql, "INSERT INTO TRANSACTIONS (ID,DATE,TYPE,DESCRIPTION,VALUE,BALANCE,ACCOUNTNAME,ACCOUNTNUMBER) " \
					"VALUES (%d,'%s','%s','%s',%f,%f,'%s','%s'); ",
					count, t.date.c_str(), t.type.c_str(), t.description.c_str(), t.value, t.balance, t.accountName.c_str(), t.accountNumber.c_str());

				/* Execute SQL statement */
				rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
				if (rc != SQLITE_OK) {
					fprintf(stderr, "SQL error: %s\n", zErrMsg);
					sqlite3_free(zErrMsg);
				}
				else {
					fprintf(stdout, "Tranactions imported successfully\n");
				}
			}

			DBClose();
			return count;
		}
		return -1;
	}

	std::vector<Transaction> TransactionRepository::SearchTransactions(std::string searchTerm, bool caseInsensitive)
	{
		if (caseInsensitive)
		{
			std::transform(searchTerm.begin(), searchTerm.end(), searchTerm.begin(), ::tolower);
		}

		std::vector<Transaction> results;
		std::size_t found;
		for (int i = 0; i < transactions.size(); ++i)
		{
			Transaction &t = transactions[i];
			if (caseInsensitive)
			{
				std::string description = t.description;
				std::transform(description.begin(), description.end(), description.begin(), ::tolower);
				found = description.find(searchTerm);
			}
			else
			{
				found = t.description.find(searchTerm);
			}
			//
			if (found != std::string::npos)
			{
				results.push_back(t);
			}
		}

		return results;
	}
	std::vector<Transaction> TransactionRepository::FilterTransactionsOnDescription(std::string searchTerm, bool caseInsensitive)
	{
		return std::vector<Transaction>();
	}
}