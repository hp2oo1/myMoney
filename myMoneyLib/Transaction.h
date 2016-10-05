#pragma once

#include <string>

namespace myMoneyLib
{
	class Transaction
	{
		friend class TransactionRepository;

		std::string		date;
		std::string		type;
		std::string		description;
		double			value;
		double			balance;
		std::string		accountName;
		std::string		accountNumber;
	public:
		Transaction();
		~Transaction();

		std::ostream& print(std::ostream& os) const;
	};
	
	std::ostream& operator<<(std::ostream& os, const Transaction& toPrint);
}