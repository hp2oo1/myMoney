#pragma once

#include "AbstractCriteria.h"

namespace myMoneyLib
{
	class CriteriaOnDescription :
		public AbstractCriteria
	{
	public:
		CriteriaOnDescription();
		~CriteriaOnDescription();

		std::vector<Transaction> meetCriteria(const std::vector<Transaction>& transactions);
	};
}