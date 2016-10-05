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

		std::vector<Transaction> SearchTransactions(std::string);
	};
}