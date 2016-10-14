#pragma once

#include "Transaction.h"

#include "sqlite3.h"

#include <vector>
#include <memory>

namespace myMoneyLib
{
	class TransactionRepository
	{
		std::string							dbPath;
		sqlite3								*db;
	
	public:
		TransactionRepository();
		~TransactionRepository();

		void DBSetPath(std::string path);
		void DBCreate();
		void DBOpen();
		void DBClose();

		int ImportCSV(std::string filename);

		std::vector<Transaction> SearchTransactions(std::string searchColumn, std::string searchTerm);
	};
}