#pragma once

#include "Transaction.h"

#include <vector>

namespace myMoneyLib
{
	class AbstractCriteria
	{
	public:
		AbstractCriteria();
		virtual ~AbstractCriteria();

		virtual std::vector<Transaction> meetCriteria(const std::vector<Transaction>& transactions);
	};
}