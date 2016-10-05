#pragma once

#include "TransactionRepository.h"

namespace myMoneyLib
{
	class TransactionViewModel
	{
		TransactionRepository		repository;
	public:
		std::string					searchTerm;
		std::vector<Transaction>	transactionsFound;
	public:
		TransactionViewModel();
		~TransactionViewModel();

		void Search();
	};
}