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
			// set database
			tvm.SetDBPath("myMoney.db");
			tvm.CreateDB();

			// load sample
			tvm.SetInputFile("../Data/sample.csv");
			tvm.Load();
			int total = tvm.GetTransactionsTotal();

			Assert::AreEqual(393, total);
		}

		TEST_METHOD(Search_test)
		{
			// set database
			tvm.SetDBPath("myMoney.db");

			// search (case insensitive)
			tvm.SetSearchTerm("tv");
			tvm.Search();
			int count = tvm.GetSearchResults().size();
			Assert::AreEqual(12, count);
		}

	};
}