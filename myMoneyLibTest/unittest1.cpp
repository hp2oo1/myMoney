#include "CppUnitTest.h"

#include "../myMoneyLib/TransactionViewModel.h"

#include <iostream>
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

myMoneyLib::TransactionViewModel tvm;

namespace myMoneyLibTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(PrepareSampletData)
		{
			// create sample
			std::ofstream fout("sample.csv");
			fout << ""																							<< std::endl
				 << "Date, Type, Description, Value, Balance, Account Name, Account Number"						<< std::endl
				 << ""																							<< std::endl
				 << "22/09/2015,D/D,\"'TV LICENCE MBP\",-12.18,945.68,\"'XXXX Y 2006\",\"'123456-12345678\","	<< std::endl
				 << "22/09/2015,D/D,\"'tv licence mbp\",-12.18,945.68,\"'XXXX Y 2006\",\"'123456-12345678\","	<< std::endl
				 << ""																							<< std::endl
				 << ""																							<< std::endl;
			fout.close();

			// load sample
			tvm.SetInputFile("sample.csv");
			tvm.Load();
			int total = tvm.GetTransactionsTotal();

			Assert::AreEqual(2, total);
		}

		TEST_METHOD(Search_test)
		{
			// load sample
			tvm.SetInputFile("sample.csv");
			tvm.Load();

			// search
			tvm.SetSearchTerm("TV");
			tvm.SetCaseInsensitive(false);
			tvm.Search();
			int count = tvm.GetSearchResults().size();
			Assert::AreEqual(1, count);

			// search - case insensitive
			tvm.SetSearchTerm("tv");
			tvm.SetCaseInsensitive(true);
			tvm.Search();
			count = tvm.GetSearchResults().size();
			Assert::AreEqual(2, count);

			// search - not found
			tvm.SetSearchTerm("tvv");
			tvm.SetCaseInsensitive(true);
			tvm.Search();
			count = tvm.GetSearchResults().size();
			Assert::AreEqual(0, count);
		}

	};
}