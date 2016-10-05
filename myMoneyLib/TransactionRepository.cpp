#include "TransactionRepository.h"

#include "minicsv.h"

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

	int TransactionRepository::LoadData(std::string filename)
	{
		// a sample list of transactions to simulate a database
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
				is >> t.date >> t.type >> t.description >> t.value >> t.balance >> t.accountName >> t.accountNumber;
				transactions.push_back(t);
			}
			return transactions.size();
		}
		return -1;
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