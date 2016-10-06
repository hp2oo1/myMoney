#include "AbstractCriteria.h"

namespace myMoneyLib
{
	AbstractCriteria::AbstractCriteria()
	{
	}


	AbstractCriteria::~AbstractCriteria()
	{
	}

	std::vector<Transaction> myMoneyLib::AbstractCriteria::meetCriteria(const std::vector<Transaction>& transactions)
	{
		return std::vector<Transaction>();
	}
}