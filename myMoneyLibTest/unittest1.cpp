#include "CppUnitTest.h"

#include "../myMoneyLib/TransactionViewModel.h"

#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace myMoneyLibTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			myMoneyLib::TransactionViewModel tvm;

			tvm.searchTerm = "TV";
			tvm.Search();

			int count = tvm.transactionsFound.size();

			Assert::AreEqual(12, count);
		}

		TEST_METHOD(TestMethod2)
		{
			myMoneyLib::TransactionViewModel tvm;

			tvm.searchTerm = "tv";
			tvm.Search();

			int count = tvm.transactionsFound.size();

			Assert::AreEqual(12, count);
		}

	};
}