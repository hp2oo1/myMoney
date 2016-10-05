#pragma once

#include "Transaction.h"

#include <vector>

namespace myMoneyLib
{
	class TransactionRepository
	{
		std::vector<Transaction> transactions;
	public:
		TransactionRepository();
		~TransactionRepository();

		int LoadData(std::string filename);

		std::vector<Transaction> SearchTransactions(std::string);
	};
}