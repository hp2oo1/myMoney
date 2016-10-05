#include "TransactionViewModel.h"


namespace myMoneyLib
{
	TransactionViewModel::TransactionViewModel()
	{
	}

	TransactionViewModel::~TransactionViewModel()
	{
	}

	void TransactionViewModel::Search()
	{
		if (searchTerm == "") 
		{
			transactionsFound = std::vector<Transaction>();
		}
		else
		{
			transactionsFound = repository.SearchTransactions(searchTerm);
		}
	}
}