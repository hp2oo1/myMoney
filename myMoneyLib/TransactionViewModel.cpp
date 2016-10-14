#include "TransactionViewModel.h"


namespace myMoneyLib
{
	TransactionViewModel::TransactionViewModel()
	{
	}
	TransactionViewModel::~TransactionViewModel()
	{
	}

	void TransactionViewModel::SetDBPath(std::string filename)
	{
		repository.DBSetPath(filename);
	}

	void TransactionViewModel::CreateDB()
	{
		repository.DBCreate();
	}

	void TransactionViewModel::Load()
	{
		transactionsTotal = repository.ImportCSV(inputFilename);
	}
	void TransactionViewModel::SetInputFile(std::string filename)
	{
		inputFilename = filename;
	}
	int TransactionViewModel::GetTransactionsTotal()
	{
		return transactionsTotal;
	}

	void TransactionViewModel::Search()
	{
		if (searchTerm == "") 
		{
			transactionsFound = std::vector<Transaction>();
		}
		else
		{
			transactionsFound = repository.SearchTransactions(searchColumn, searchTerm);
		}
	}
	void TransactionViewModel::SetSearchColumn(std::string column)
	{
		searchColumn = column;
	}
	void TransactionViewModel::SetSearchTerm(std::string term)
	{
		searchTerm = term;
	}
	std::vector<Transaction> TransactionViewModel::GetSearchResults()
	{
		return transactionsFound;
	}
}