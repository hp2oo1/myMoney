// myMoney.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "minicsv.h"
#include <iostream>

struct Transaction
{
	std::string date;
	std::string type;
	std::string description;
	double value;
	double balance;
	std::string accountName;
	std::string accountNumber;
};

int _tmain(int argc, _TCHAR * argv[])
{

    printf( "argc = %d\n", argc );
    for( int i = 0; i < argc; ++i ) {
        printf( "argv[ %d ] = %s\n", i, argv[ i ] );
    }

	mini::csv::ifstream is(argv[1]);
    is.set_delimiter(',', "$$");
	is.enable_trim_quote_on_str(true, '\"');
	if(is.is_open())
    {
        Transaction temp;
		is.read_line(); // skip head line
        while(is.read_line())
        {
			is >> temp.date
			   >> temp.type
			   >> temp.description
			   >> temp.value
			   >> temp.balance
			   >> temp.accountName
			   >> temp.accountNumber;
            // display the read items
			std::cout << temp.date << "|"
				      << temp.type << "|"
					  << temp.description << "|"
					  << temp.value << "|"
					  << temp.balance << "|"
					  << temp.accountName << "|"
					  << temp.accountNumber << std::endl;
        }
    }
    return 0;
}

