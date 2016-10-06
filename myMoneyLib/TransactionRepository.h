#pragma once

#include "Transaction.h"
#include "AbstractCriteria.h"

#include <vector>
#include <memory>

namespace myMoneyLib
{
	class TransactionRepository
	{
		std::vector<Transaction>			transactions;
		std::shared_ptr<AbstractCriteria>	criteriaOnDescription;
	public:
		TransactionRepository();
		~TransactionRepository();

		int LoadData(std::string filename);

		std::vector<Transaction> SearchTransactions(std::string searchTerm, bool caseInsensitive);

		std::vector<Transaction> FilterTransactionsOnDescription(std::string searchTerm, bool caseInsensitive);
	};
}