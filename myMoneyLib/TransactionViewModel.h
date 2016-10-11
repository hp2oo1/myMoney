#pragma once

#include "TransactionRepository.h"

namespace myMoneyLib
{
	class TransactionViewModel
	{
		TransactionRepository		repository;
		
		std::string					inputFilename;
		int							transactionsTotal;
		
		std::string					searchTerm;
		bool						caseInsensitive;
		std::vector<Transaction>	transactionsFound;
	public:
		TransactionViewModel();
		~TransactionViewModel();

		void SetDB(std::string filename);
		void Load();
		void SetInputFile(std::string filename);
		int  GetTransactionsTotal();

		void Search();
		void SetSearchTerm(std::string search);
		void SetCaseInsensitive(bool insensitive);
		std::vector<Transaction> GetSearchResults();
	};
}