#include "Transaction.h"


namespace myMoneyLib
{
	Transaction::Transaction()
	{
	}

	Transaction::~Transaction()
	{
	}

	std::ostream & Transaction::print(std::ostream & os) const
	{
		os << date << "|" << type << "|" << description << "|" << value << "|" << balance << "|" << accountName << "|" << accountNumber << std::endl;
		return os;
	}

	std::ostream & operator<<(std::ostream & os, const Transaction & toPrint)
	{
		return toPrint.print(os);
	}
}