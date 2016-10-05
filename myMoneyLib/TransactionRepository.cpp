#include "TransactionRepository.h"

#include "minicsv.h"

#include <stdio.h>
#include <ctype.h>

namespace myMoneyLib
{
	TransactionRepository::TransactionRepository()
	{
		// a sample list of transactions to simulate a database
		mini::csv::ifstream is("D:/Peng_other_stuff/projects/myMoney/Data/sample.csv");
		is.set_delimiter(',', "$$");
		is.enable_trim_quote_on_str(true, '\"');
		if (is.is_open())
		{
			Transaction t;
			is.read_line(); // skip head line
			while (is.read_line())
			{
				is	>> t.date >> t.type >> t.description >> t.value >> t.balance >> t.accountName >> t.accountNumber;
				transactions.push_back(t);
			}
		}
	}

	TransactionRepository::~TransactionRepository()
	{
	}

	std::vector<Transaction> TransactionRepository::SearchTransactions(std::string searchTerm)
	{
		std::transform(searchTerm.begin(), searchTerm.end(), searchTerm.begin(), ::tolower);
		
		std::vector<Transaction> results;
		for (int i = 0; i < transactions.size(); ++i)
		{
			Transaction &t = transactions[i];
			std::string description = t.description;
			std::transform(description.begin(), description.end(), description.begin(), ::tolower);
			std::size_t found = description.find(searchTerm);
			if (found != std::string::npos)
			{
				results.push_back(t);
			}
		}

		return results;
	}
}