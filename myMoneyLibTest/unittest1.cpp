#include "CppUnitTest.h"

#include "../myMoneyLib/TransactionViewModel.h"

#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

myMoneyLib::TransactionViewModel tvm;

namespace myMoneyLibTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(Load)
		{
			tvm.SetInputFile("sample.csv");
			tvm.Load();

			int total = tvm.GetTransactionsTotal();

			Assert::AreEqual(397, total);
		}

		TEST_METHOD(Search)
		{
			tvm.SetSearchTerm("TV");
			tvm.Search();

			int count = tvm.GetSearchResults().size();

			Assert::AreEqual(12, count);
		}

		TEST_METHOD(Search_CaseInsensitive)
		{
			tvm.SetSearchTerm("tv");
			tvm.Search();

			int count = tvm.GetSearchResults().size();

			Assert::AreEqual(12, count);
		}

		TEST_METHOD(Search_NotFound)
		{
			tvm.SetSearchTerm("tvv");
			tvm.Search();

			int count = tvm.GetSearchResults().size();

			Assert::AreEqual(0, count);
		}

	};
}