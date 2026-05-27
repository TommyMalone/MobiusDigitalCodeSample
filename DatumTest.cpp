#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "Datum.h"
#include "Vector.h"
#include <random>
#include <time.h>
#include <limits>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FIEAGameEngine;
using namespace std;
using namespace UnitTests;
using namespace glm;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(DatumTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				_CrtDumpMemoryLeaks();
				Assert::Fail(L"Memory leak detected!\nIf a static object is dynamically allocating memory this may be a false positive.");
			}
#endif
		}

		float randFloat()
		{
			numeric_limits<float> floatLimits;
			return floatLimits.min() + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (floatLimits.max() - floatLimits.min())));
		}

		std::string random_string(size_t length)
		{
			auto randchar = []() -> char
			{
				const char charset[] =
					"0123456789"
					"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
					"abcdefghijklmnopqrstuvwxyz";
				const size_t max_index = (sizeof(charset) - 1);
				return charset[rand() % max_index];
			};
			std::string str(length, 0);
			std::generate_n(str.begin(), length, randchar);
			return str;
		}

		TEST_METHOD(ConstructorsAndDestructorTest)
		{
			srand(static_cast<unsigned int>(time(NULL)));

			int testInt = rand();
			float testFloat = randFloat();
			vec4 testVec4(randFloat(), randFloat(), randFloat(), randFloat());
			mat4x4 testMat4x4(randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat());
			string testString = random_string(32);
			Foo testFoo(rand());
			RTTI * testRTTI = &testFoo;

			Datum intDatum(testInt);
			Assert::IsTrue(intDatum.Front<int>() == testInt);
			Assert::IsTrue(intDatum.Back<int>() == testInt);
			Datum floatDatum(testFloat);
			Assert::IsTrue(floatDatum.Front<float>() == testFloat);
			Assert::IsTrue(floatDatum.Back<float>() == testFloat);
			Datum vec4Datum(testVec4);
			Assert::IsTrue(vec4Datum.Front<vec4>() == testVec4);
			Assert::IsTrue(vec4Datum.Back<vec4>() == testVec4);
			Datum mat4x4Datum(testMat4x4);
			Assert::IsTrue(mat4x4Datum.Front<mat4x4>() == testMat4x4);
			Assert::IsTrue(mat4x4Datum.Back<mat4x4>() == testMat4x4);
			Datum stringDatum(testString);
			Assert::IsTrue(stringDatum.Front<string>() == testString);
			Assert::IsTrue(stringDatum.Back<string>() == testString);
			Datum RTTIDatum(testRTTI);
			Assert::IsTrue(RTTIDatum.Front<RTTI*>() == testRTTI);
			Assert::IsTrue(RTTIDatum.Back<RTTI*>() == testRTTI);
			Datum datumDatum(stringDatum);
			Assert::IsTrue(datumDatum.Front<string>() == testString);
			Assert::IsTrue(datumDatum.Back<string>() == testString);
			Datum unknownDatum;
		}

		TEST_METHOD(InitializerListConstructorTest)
		{
			size_t testSize = 16;

			Vector<int> testInts;
			Vector<float> testFloats;
			Vector<vec4> testVec4s;
			Vector<mat4x4> testMat4x4s;
			Vector<string> testStrings;
			Vector<Foo> testFoos;
			Vector<RTTI*> testRTTIs;
			for (size_t i = 0; i < testSize; i++)
			{
				testInts.PushBack(rand());
				testFloats.PushBack(randFloat());
				testVec4s.PushBack(vec4(randFloat(), randFloat(), randFloat(), randFloat()));
				testMat4x4s.PushBack(mat4x4((randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat())));
				testStrings.PushBack(random_string(testSize));
				testFoos.PushBack(Foo(rand()));
			}
			//Must be seperate from other for loop because Foo vector will resize and change the address od it's stored Foos.
			for (size_t i = 0; i < testSize; i++)
			{
				testRTTIs.PushBack(&testFoos[i]);
			}

			Datum intDatum = { testInts[0], testInts[1], testInts[2], testInts[3],testInts[4], testInts[5], testInts[6], testInts[7],testInts[8], testInts[9], testInts[10], testInts[11],testInts[12], testInts[13], testInts[14], testInts[15] };
			Assert::IsTrue(intDatum.Front<int>() == testInts[0]);
			Assert::IsTrue(intDatum.Back<int>() == testInts[testSize-1]);
			
			Datum floatDatum = { testFloats[0], testFloats[1], testFloats[2], testFloats[3],testFloats[4], testFloats[5], testFloats[6], testFloats[7],testFloats[8], testFloats[9], testFloats[10], testFloats[11],testFloats[12], testFloats[13], testFloats[14], testFloats[15] }; 
			Assert::IsTrue(floatDatum.Front<float>() == testFloats[0]);
			Assert::IsTrue(floatDatum.Back<float>() == testFloats[testSize-1]);

			Datum vec4Datum = { testVec4s[0], testVec4s[1], testVec4s[2], testVec4s[3],testVec4s[4], testVec4s[5], testVec4s[6], testVec4s[7],testVec4s[8], testVec4s[9], testVec4s[10], testVec4s[11],testVec4s[12], testVec4s[13], testVec4s[14], testVec4s[15] };
			Assert::IsTrue(vec4Datum.Front<vec4>() == testVec4s[0]);
			Assert::IsTrue(vec4Datum.Back<vec4>() == testVec4s[testSize-1]);

			Datum mat4x4Datum = { testMat4x4s[0], testMat4x4s[1], testMat4x4s[2], testMat4x4s[3],testMat4x4s[4], testMat4x4s[5], testMat4x4s[6], testMat4x4s[7],testMat4x4s[8], testMat4x4s[9], testMat4x4s[10], testMat4x4s[11],testMat4x4s[12], testMat4x4s[13], testMat4x4s[14], testMat4x4s[15] };
			Assert::IsTrue(mat4x4Datum.Front<mat4x4>() == testMat4x4s[0]);
			Assert::IsTrue(mat4x4Datum.Back<mat4x4>() == testMat4x4s[testSize-1]);
			
			Datum stringDatum = { testStrings[0], testStrings[1], testStrings[2], testStrings[3],testStrings[4], testStrings[5], testStrings[6], testStrings[7],testStrings[8], testStrings[9], testStrings[10], testStrings[11],testStrings[12], testStrings[13], testStrings[14], testStrings[15] };
			Assert::IsTrue(stringDatum.Front<string>() == testStrings[0]);
			Assert::IsTrue(stringDatum.Back<string>() == testStrings[testSize-1]);

			Datum RTTIDatum = { testRTTIs[0], testRTTIs[1], testRTTIs[2], testRTTIs[3],testRTTIs[4], testRTTIs[5], testRTTIs[6], testRTTIs[7],testRTTIs[8], testRTTIs[9], testRTTIs[10], testRTTIs[11],testRTTIs[12], testRTTIs[13], testRTTIs[14], testRTTIs[15] };
			Assert::IsTrue(RTTIDatum.Front<RTTI*>() == testRTTIs[0]);
			Assert::IsTrue(RTTIDatum.Back<RTTI*>() == testRTTIs[testSize-1]);

			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue(intDatum.Get<int>(i) == testInts[i]);
				Assert::IsTrue(floatDatum.Get<float>(i) == testFloats[i]);
				Assert::IsTrue(vec4Datum.Get<vec4>(i) == testVec4s[i]);
				Assert::IsTrue(mat4x4Datum.Get<mat4x4>(i) == testMat4x4s[i]);
				Assert::IsTrue(stringDatum.Get<string>(i) == testStrings[i]);
				Assert::IsTrue(RTTIDatum.Get<RTTI*>(i) == testRTTIs[i]);
			}

		}

		TEST_METHOD(ClearTest)
		{
			size_t testSize = 16;

			Vector<int> testInts;
			Vector<float> testFloats;
			Vector<vec4> testVec4s;
			Vector<mat4x4> testMat4x4s;
			Vector<string> testStrings;
			Vector<Foo> testFoos;
			Vector<RTTI*> testRTTIs;
			for (size_t i = 0; i < testSize; i++)
			{
				testInts.PushBack(rand());
				testFloats.PushBack(randFloat());
				testVec4s.PushBack(vec4(randFloat(), randFloat(), randFloat(), randFloat()));
				testMat4x4s.PushBack(mat4x4((randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat())));
				testStrings.PushBack(random_string(testSize));
				testFoos.PushBack(Foo(rand()));
			}
			//Must be seperate from other for loop because Foo vector will resize and change the address od it's stored Foos.
			for (size_t i = 0; i < testSize; i++)
			{
				testRTTIs.PushBack(&testFoos[i]);
			}

			Datum intDatum = { testInts[0], testInts[1], testInts[2], testInts[3],testInts[4], testInts[5], testInts[6], testInts[7],testInts[8], testInts[9], testInts[10], testInts[11],testInts[12], testInts[13], testInts[14], testInts[15] };
			Datum floatDatum = { testFloats[0], testFloats[1], testFloats[2], testFloats[3],testFloats[4], testFloats[5], testFloats[6], testFloats[7],testFloats[8], testFloats[9], testFloats[10], testFloats[11],testFloats[12], testFloats[13], testFloats[14], testFloats[15] };
			Datum vec4Datum = { testVec4s[0], testVec4s[1], testVec4s[2], testVec4s[3],testVec4s[4], testVec4s[5], testVec4s[6], testVec4s[7],testVec4s[8], testVec4s[9], testVec4s[10], testVec4s[11],testVec4s[12], testVec4s[13], testVec4s[14], testVec4s[15] };
			Datum mat4x4Datum = { testMat4x4s[0], testMat4x4s[1], testMat4x4s[2], testMat4x4s[3],testMat4x4s[4], testMat4x4s[5], testMat4x4s[6], testMat4x4s[7],testMat4x4s[8], testMat4x4s[9], testMat4x4s[10], testMat4x4s[11],testMat4x4s[12], testMat4x4s[13], testMat4x4s[14], testMat4x4s[15] };
			Datum stringDatum = { testStrings[0], testStrings[1], testStrings[2], testStrings[3],testStrings[4], testStrings[5], testStrings[6], testStrings[7],testStrings[8], testStrings[9], testStrings[10], testStrings[11],testStrings[12], testStrings[13], testStrings[14], testStrings[15] };
			Datum RTTIDatum = { testRTTIs[0], testRTTIs[1], testRTTIs[2], testRTTIs[3],testRTTIs[4], testRTTIs[5], testRTTIs[6], testRTTIs[7],testRTTIs[8], testRTTIs[9], testRTTIs[10], testRTTIs[11],testRTTIs[12], testRTTIs[13], testRTTIs[14], testRTTIs[15] };
			

			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue(intDatum.Get<int>(i) == testInts[i]);
				Assert::IsTrue(floatDatum.Get<float>(i) == testFloats[i]);
				Assert::IsTrue(vec4Datum.Get<vec4>(i) == testVec4s[i]);
				Assert::IsTrue(mat4x4Datum.Get<mat4x4>(i) == testMat4x4s[i]);
				Assert::IsTrue(stringDatum.Get<string>(i) == testStrings[i]);
				Assert::IsTrue(RTTIDatum.Get<RTTI*>(i) == testRTTIs[i]);
			}

			intDatum.Clear();
			floatDatum.Clear();
			vec4Datum.Clear();
			mat4x4Datum.Clear();
			stringDatum.Clear();
			RTTIDatum.Clear();

			Assert::IsTrue(intDatum.Size() == 0);
			Assert::IsTrue(floatDatum.Size() == 0);
			Assert::IsTrue(vec4Datum.Size() == 0);
			Assert::IsTrue(mat4x4Datum.Size() == 0);
			Assert::IsTrue(stringDatum.Size() == 0);
			Assert::IsTrue(RTTIDatum.Size() == 0);

			int* testIntArr = new int[testSize];
			float* testFloatArr = new float[testSize];
			vec4* testVec4Arr = new vec4[testSize];
			mat4x4* testMat4x4Arr = new mat4x4[testSize];
			string* testStringArr = new string[testSize];
			Foo* testFooArr = new Foo[testSize];
			RTTI** testRTTIArr = new RTTI*[testSize];

			for (size_t i = 0; i < testSize; i++)
			{
				testIntArr[i] = rand();
				testFloatArr[i] = randFloat();
				testVec4Arr[i] = vec4(randFloat(), randFloat(), randFloat(), randFloat());
				testMat4x4Arr[i] = mat4x4(randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat());
				testStringArr[i] = random_string(32);
				testFooArr[i] = Foo(rand());
				testRTTIArr[i] = &testFooArr[i];
			}

			
			intDatum.SetStorage(testIntArr, testSize);
			auto intExternalClearException = [&intDatum] {intDatum.Clear(); };
			Assert::ExpectException<std::exception>(intExternalClearException);

			
			floatDatum.SetStorage(testFloatArr, testSize);
			auto floatExternalClearException = [&floatDatum] {floatDatum.Clear(); };
			Assert::ExpectException<std::exception>(floatExternalClearException);

			
			vec4Datum.SetStorage(testVec4Arr, testSize);
			auto vec4ExternalClearException = [&vec4Datum] {vec4Datum.Clear(); };
			Assert::ExpectException<std::exception>(vec4ExternalClearException);

		
			mat4x4Datum.SetStorage(testMat4x4Arr, testSize);
			auto mat4x4ExternalClearException = [&mat4x4Datum] {mat4x4Datum.Clear(); };
			Assert::ExpectException<std::exception>(mat4x4ExternalClearException);

			
			stringDatum.SetStorage(testStringArr, testSize);
			auto stringExternalClearException = [&stringDatum] {stringDatum.Clear(); };
			Assert::ExpectException<std::exception>(stringExternalClearException);

			
			RTTIDatum.SetStorage(testRTTIArr, testSize);
			auto RTTIExternalClearException = [&RTTIDatum] {RTTIDatum.Clear(); };
			Assert::ExpectException<std::exception>(RTTIExternalClearException);

			delete[] testIntArr;
			delete[] testFloatArr;
			delete[] testVec4Arr;
			delete[] testMat4x4Arr;
			delete[] testStringArr;
			delete[] testFooArr;
			delete[] testRTTIArr;
		}

		TEST_METHOD(SetStorageTest)
		{
			srand(static_cast<unsigned int>(time(NULL)));

			Datum intDatum;
			
			Datum floatDatum;
			
			Datum vec4Datum;
			
			Datum mat4x4Datum;
			
			Datum stringDatum;
			
			Datum RTTIDatum;
			
			Datum datumDatum;
			
			Datum unknownDatum;

			size_t testSize = 16;
			int* testIntArr = new int[testSize];
			float* testFloatArr = new float[testSize];
			vec4* testVec4Arr = new vec4[testSize];
			mat4x4* testMat4x4Arr = new mat4x4[testSize];
			string* testStringArr = new string[testSize];
			Foo* testFooArr = new Foo[testSize];
			RTTI** testRTTIArr = new RTTI*[testSize];

			for (size_t i = 0; i < testSize; i++)
			{
				testIntArr[i] = rand();
				testFloatArr[i] = randFloat();
				testVec4Arr[i] = vec4(randFloat(), randFloat(), randFloat(), randFloat());
				testMat4x4Arr[i] = mat4x4(randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat());
				testStringArr[i] = random_string(32);
				testFooArr[i] = Foo(rand());
				testRTTIArr[i] = &testFooArr[i];
			}

			Assert::IsTrue(intDatum.IsInternal());
			Assert::IsTrue(intDatum.Size() == 0);
			Assert::IsTrue(intDatum.Capacity() == 0);
			intDatum.SetStorage(testIntArr, testSize);
			Assert::IsTrue(intDatum.Size() == testSize);
			Assert::IsTrue(intDatum.Capacity() == testSize);
			Assert::IsFalse(intDatum.IsInternal());

			Assert::IsTrue(floatDatum.IsInternal());
			Assert::IsTrue(floatDatum.Size() == 0);
			Assert::IsTrue(floatDatum.Capacity() == 0);
			floatDatum.SetStorage(testFloatArr, testSize);
			Assert::IsTrue(floatDatum.Size() == testSize);
			Assert::IsTrue(floatDatum.Capacity() == testSize);
			Assert::IsFalse(floatDatum.IsInternal());

			Assert::IsTrue(vec4Datum.IsInternal());
			Assert::IsTrue(vec4Datum.Size() == 0);
			Assert::IsTrue(vec4Datum.Capacity() == 0);
			vec4Datum.SetStorage(testVec4Arr, testSize);
			Assert::IsTrue(vec4Datum.Size() == testSize);
			Assert::IsTrue(vec4Datum.Capacity() == testSize);
			Assert::IsFalse(vec4Datum.IsInternal());

			Assert::IsTrue(mat4x4Datum.IsInternal());
			Assert::IsTrue(mat4x4Datum.Size() == 0);
			Assert::IsTrue(mat4x4Datum.Capacity() == 0);
			mat4x4Datum.SetStorage(testMat4x4Arr, testSize);
			Assert::IsTrue(mat4x4Datum.Size() == testSize);
			Assert::IsTrue(mat4x4Datum.Capacity() == testSize);
			Assert::IsFalse(mat4x4Datum.IsInternal());

			Assert::IsTrue(stringDatum.IsInternal());
			Assert::IsTrue(stringDatum.Size() == 0);
			Assert::IsTrue(stringDatum.Capacity() == 0);
			stringDatum.SetStorage(testStringArr, testSize);
			Assert::IsTrue(stringDatum.Size() == testSize);
			Assert::IsTrue(stringDatum.Capacity() == testSize);
			Assert::IsFalse(stringDatum.IsInternal());

			Assert::IsTrue(RTTIDatum.IsInternal());
			Assert::IsTrue(RTTIDatum.Size() == 0);
			Assert::IsTrue(RTTIDatum.Capacity() == 0);
			RTTIDatum.SetStorage(testRTTIArr, testSize);
			Assert::IsTrue(RTTIDatum.Size() == testSize);
			Assert::IsTrue(RTTIDatum.Capacity() == testSize);
			Assert::IsFalse(RTTIDatum.IsInternal());

			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue(intDatum.Get<int>(i) == testIntArr[i]);
				Assert::IsTrue(floatDatum.Get<float>(i) == testFloatArr[i]);
				Assert::IsTrue(vec4Datum.Get<vec4>(i) == testVec4Arr[i]);
				Assert::IsTrue(mat4x4Datum.Get<mat4x4>(i) == testMat4x4Arr[i]);
				Assert::IsTrue(stringDatum.Get<string>(i) == testStringArr[i]);
				Assert::IsTrue(RTTIDatum.Get<RTTI*>(i) == testRTTIArr[i]);
			}

			auto intTypeMismatchException = [&intDatum, &testFloatArr, &testSize] {intDatum.SetStorage(testFloatArr, testSize); };
			Assert::ExpectException<std::exception>(intTypeMismatchException);

			auto floatTypeMismatchException = [&floatDatum, &testIntArr, &testSize] {floatDatum.SetStorage(testIntArr, testSize); };
			Assert::ExpectException<std::exception>(floatTypeMismatchException);

			auto vec4TypeMismatchException = [&vec4Datum, &testMat4x4Arr, &testSize] {vec4Datum.SetStorage(testMat4x4Arr, testSize); };
			Assert::ExpectException<std::exception>(vec4TypeMismatchException);

			auto mat4x4TypeMismatchException = [&mat4x4Datum, &testStringArr, &testSize] {mat4x4Datum.SetStorage(testStringArr, testSize); };
			Assert::ExpectException<std::exception>(mat4x4TypeMismatchException);

			auto stringTypeMismatchException = [&stringDatum, &testRTTIArr, &testSize] {stringDatum.SetStorage(testRTTIArr, testSize); };
			Assert::ExpectException<std::exception>(stringTypeMismatchException);

			auto RTTITypeMismatchException = [&RTTIDatum, &testVec4Arr, &testSize] {RTTIDatum.SetStorage(testVec4Arr, testSize); };
			Assert::ExpectException<std::exception>(RTTITypeMismatchException);

			auto intNullptrException = [&intDatum, &testSize] {intDatum.SetStorage(static_cast<int*>(nullptr), testSize); };
			Assert::ExpectException<std::exception>(intNullptrException);

			auto floatNullptrException = [&floatDatum, &testSize] {floatDatum.SetStorage(static_cast<float*>(nullptr), testSize); };
			Assert::ExpectException<std::exception>(floatNullptrException);

			auto vec4NullptrException = [&vec4Datum, &testSize] {vec4Datum.SetStorage(static_cast<vec4*>(nullptr), testSize); };
			Assert::ExpectException<std::exception>(vec4NullptrException);

			auto mat4x4NullptrException = [&mat4x4Datum, &testSize] {mat4x4Datum.SetStorage(static_cast<mat4x4*>(nullptr), testSize); };
			Assert::ExpectException<std::exception>(mat4x4NullptrException);

			auto stringNullptrException = [&stringDatum, &testRTTIArr, &testSize] {stringDatum.SetStorage(static_cast<string*>(nullptr), testSize); };
			Assert::ExpectException<std::exception>(stringNullptrException);

			auto RTTINullptrException = [&RTTIDatum, &testSize] {RTTIDatum.SetStorage(static_cast<RTTI**>(nullptr), testSize); };
			Assert::ExpectException<std::exception>(RTTINullptrException);

			delete[] testIntArr;
			delete[] testFloatArr;
			delete[] testVec4Arr;
			delete[] testMat4x4Arr;
			delete[] testStringArr;
			delete[] testFooArr;
			delete[] testRTTIArr;
		}

		TEST_METHOD(AssignmentOperatorTest)
		{
			srand(static_cast<unsigned int>(time(NULL)));

			size_t testSize = 16;
			int testInt = rand();
			int testInt2 = rand();
			float testFloat = randFloat();
			float testFloat2 = randFloat();
			vec4 testVec4(randFloat(), randFloat(), randFloat(), randFloat());
			vec4 testVec42(randFloat(), randFloat(), randFloat(), randFloat());
			mat4x4 testMat4x4(randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat());
			mat4x4 testMat4x42(randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat());
			string testString = random_string(32);
			string testString2 = random_string(32);
			Foo testFoo(rand());
			Foo testFoo2(rand());
			RTTI * testRTTI = &testFoo;
			RTTI * testRTTI2 = &testFoo2;

			Datum intDatum(testInt);
			Datum floatDatum(testFloat);
			Datum vec4Datum(testVec4);
			Datum mat4x4Datum(testMat4x4);
			Datum stringDatum(testString);
			Datum RTTIDatum(testRTTI);
			Datum datumDatum(stringDatum);
			Datum unknownDatum;

			intDatum = testInt2;
			Assert::IsTrue(intDatum.Size() == 1);
			Assert::IsTrue(intDatum.Capacity() == 1);
			Assert::IsTrue(intDatum.Front<int>() == testInt2);
			Assert::IsTrue(intDatum.Back<int>() == testInt2);

			floatDatum = testFloat2;
			Assert::IsTrue(floatDatum.Size() == 1);
			Assert::IsTrue(floatDatum.Front<float>() == testFloat2);
			Assert::IsTrue(floatDatum.Back<float>() == testFloat2);

			vec4Datum = testVec42;
			Assert::IsTrue(vec4Datum.Size() == 1);
			Assert::IsTrue(vec4Datum.Capacity() == 1);
			Assert::IsTrue(vec4Datum.Front<vec4>() == testVec42);
			Assert::IsTrue(vec4Datum.Back<vec4>() == testVec42);

			mat4x4Datum = testMat4x42;
			Assert::IsTrue(mat4x4Datum.Size() == 1);
			Assert::IsTrue(mat4x4Datum.Front<mat4x4>() == testMat4x42);
			Assert::IsTrue(mat4x4Datum.Back<mat4x4>() == testMat4x42);

			stringDatum = testString2;
			Assert::IsTrue(stringDatum.Size() == 1);
			Assert::IsTrue(stringDatum.Front<string>() == testString2);
			Assert::IsTrue(stringDatum.Back<string>() == testString2);

			RTTIDatum = testRTTI2;
			Assert::IsTrue(RTTIDatum.Size() == 1);
			Assert::IsTrue(RTTIDatum.Front<RTTI*>() == testRTTI2);
			Assert::IsTrue(RTTIDatum.Back<RTTI*>() == testRTTI2);

			auto intTypeMismatchException = [&intDatum, &testRTTI] { intDatum = testRTTI; };
			Assert::ExpectException<std::exception>(intTypeMismatchException);

			auto floatTypeMismatchException = [&floatDatum, &testInt] { floatDatum = testInt; };
			Assert::ExpectException<std::exception>(floatTypeMismatchException);

			auto vec4TypeMismatchException = [&vec4Datum, &testFloat] { vec4Datum = testFloat; };
			Assert::ExpectException<std::exception>(vec4TypeMismatchException);

			auto mat4x4TypeMismatchException = [&mat4x4Datum, &testVec4] { mat4x4Datum = testVec4; };
			Assert::ExpectException<std::exception>(mat4x4TypeMismatchException);

			auto stringTypeMismatchException = [&stringDatum, &testMat4x4] { stringDatum = testMat4x4; };
			Assert::ExpectException<std::exception>(stringTypeMismatchException);

			auto RTTITypeMismatchException = [&RTTIDatum, &testString] { RTTIDatum = testString; };
			Assert::ExpectException<std::exception>(RTTITypeMismatchException);

			intDatum = vec4Datum;
			Assert::IsTrue(intDatum == vec4Datum);

			intDatum = floatDatum;
			Assert::IsTrue(intDatum == floatDatum);

			intDatum = mat4x4Datum;
			Assert::IsTrue(intDatum == mat4x4Datum);

			intDatum = stringDatum;
			Assert::IsTrue(intDatum == stringDatum);

			intDatum = RTTIDatum;
			Assert::IsTrue(intDatum == RTTIDatum);

			intDatum = datumDatum;
			Assert::IsTrue(intDatum == datumDatum);

			intDatum = unknownDatum;
			Assert::IsTrue(intDatum == unknownDatum);


			int* testIntArr = new int[testSize];
			float* testFloatArr = new float[testSize];
			vec4* testVec4Arr = new vec4[testSize];
			mat4x4* testMat4x4Arr = new mat4x4[testSize];
			string* testStringArr = new string[testSize];
			Foo* testFooArr = new Foo[testSize];
			RTTI** testRTTIArr = new RTTI*[testSize];

			for (size_t i = 0; i < testSize; i++)
			{
				testIntArr[i] = rand();
				testFloatArr[i] = randFloat();
				testVec4Arr[i] = vec4(randFloat(), randFloat(), randFloat(), randFloat());
				testMat4x4Arr[i] = mat4x4(randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat());
				testStringArr[i] = random_string(32);
				testFooArr[i] = Foo(rand());
				testRTTIArr[i] = &testFooArr[i];
			}

			Datum intDatumExternal;

			Datum floatDatumExternal;

			Datum vec4DatumExternal;

			Datum mat4x4DatumExternal;

			Datum stringDatumExternal;

			Datum RTTIDatumExternal;

			Datum datumDatumExternal;

			Datum unknownDatumExternal;

			intDatumExternal.SetStorage(testIntArr, testSize);

			floatDatumExternal.SetStorage(testFloatArr, testSize);

			vec4DatumExternal.SetStorage(testVec4Arr, testSize);

			mat4x4DatumExternal.SetStorage(testMat4x4Arr, testSize);

			stringDatumExternal.SetStorage(testStringArr, testSize);

			RTTIDatumExternal.SetStorage(testRTTIArr, testSize);

			intDatum = intDatumExternal;
			Assert::IsFalse(intDatum.IsInternal());
			Assert::IsTrue(intDatum.Size() == intDatumExternal.Size());
			Assert::IsTrue(intDatum.Capacity() == intDatumExternal.Capacity());

			intDatum = floatDatumExternal;
			Assert::IsFalse(intDatum.IsInternal());
			Assert::IsTrue(intDatum.Size() == floatDatumExternal.Size());
			Assert::IsTrue(intDatum.Capacity() == floatDatumExternal.Capacity());

			intDatum = vec4DatumExternal;
			Assert::IsFalse(intDatum.IsInternal());
			Assert::IsTrue(intDatum.Size() == vec4DatumExternal.Size());
			Assert::IsTrue(intDatum.Capacity() == vec4DatumExternal.Capacity());

			intDatum = mat4x4DatumExternal;
			Assert::IsFalse(intDatum.IsInternal());
			Assert::IsTrue(intDatum.Size() == mat4x4DatumExternal.Size());
			Assert::IsTrue(intDatum.Capacity() == mat4x4DatumExternal.Capacity());

			intDatum = stringDatumExternal;
			Assert::IsFalse(intDatum.IsInternal());
			Assert::IsTrue(intDatum.Size() == stringDatumExternal.Size());
			Assert::IsTrue(intDatum.Capacity() == stringDatumExternal.Capacity());

			intDatum = RTTIDatumExternal;
			Assert::IsFalse(intDatum.IsInternal());
			Assert::IsTrue(intDatum.Size() == RTTIDatumExternal.Size());
			Assert::IsTrue(intDatum.Capacity() == RTTIDatumExternal.Capacity());

			

			
			intDatumExternal = testInt;
			Assert::IsFalse(intDatum.IsInternal());
			Assert::IsTrue(intDatumExternal.Size() == 1);
			Assert::IsTrue(intDatumExternal.Capacity() == 1);

			floatDatumExternal = testFloat;
			Assert::IsFalse(intDatum.IsInternal());
			Assert::IsTrue(floatDatumExternal.Size() == 1);
			Assert::IsTrue(floatDatumExternal.Capacity() == 1);

			vec4DatumExternal = testVec4;
			Assert::IsFalse(intDatum.IsInternal());
			Assert::IsTrue(vec4DatumExternal.Size() == 1);
			Assert::IsTrue(vec4DatumExternal.Capacity() == 1);

			mat4x4DatumExternal = testMat4x4;
			Assert::IsFalse(intDatum.IsInternal());
			Assert::IsTrue(mat4x4DatumExternal.Size() == 1);
			Assert::IsTrue(mat4x4DatumExternal.Capacity() == 1);

			stringDatumExternal = testString;
			Assert::IsFalse(intDatum.IsInternal());
			Assert::IsTrue(stringDatumExternal.Size() == 1);
			Assert::IsTrue(stringDatumExternal.Capacity() == 1);

			RTTIDatumExternal = testRTTI;
			Assert::IsFalse(intDatum.IsInternal());
			Assert::IsTrue(RTTIDatumExternal.Size() == 1);
			Assert::IsTrue(RTTIDatumExternal.Capacity() == 1);

			delete[] testIntArr;
			delete[] testFloatArr;
			delete[] testVec4Arr;
			delete[] testMat4x4Arr;
			delete[] testStringArr;
			delete[] testFooArr;
			delete[] testRTTIArr;
		}

		TEST_METHOD(TypeTest)
		{

			int testInt = rand();
			float testFloat = randFloat();
			vec4 testVec4(randFloat(), randFloat(), randFloat(), randFloat());
			mat4x4 testMat4x4(randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat());
			string testString = random_string(32);
			Foo testFoo(rand());
			RTTI * testRTTI = &testFoo;

			Datum intDatum(testInt);
			Assert::IsTrue(Datum::DatumType::Integer == intDatum.Type());
			Datum floatDatum(testFloat);
			Assert::IsTrue(Datum::DatumType::Float == floatDatum.Type());
			Datum vec4Datum(testVec4);
			Assert::IsTrue(Datum::DatumType::Vector4 == vec4Datum.Type());
			Datum mat4x4Datum(testMat4x4);
			Assert::IsTrue(Datum::DatumType::Matrix4x4 == mat4x4Datum.Type());
			Datum stringDatum(testString);
			Assert::IsTrue(Datum::DatumType::String == stringDatum.Type());
			Datum RTTIDatum(testRTTI);
			Assert::IsTrue(Datum::DatumType::RTTIPtr == RTTIDatum.Type());
			Datum datumDatum(stringDatum);
			Assert::IsTrue(Datum::DatumType::String == datumDatum.Type());
			Datum unknownDatum;
			Assert::IsTrue(Datum::DatumType::Unknown == unknownDatum.Type());
		}

		TEST_METHOD(SetTypeTest)
		{
			Datum intDatum;
			Assert::IsTrue(Datum::DatumType::Unknown == intDatum.Type());
			intDatum.SetType(Datum::DatumType::Integer);
			Assert::IsTrue(Datum::DatumType::Integer == intDatum.Type());

			Datum floatDatum;
			Assert::IsTrue(Datum::DatumType::Unknown == floatDatum.Type());
			floatDatum.SetType(Datum::DatumType::Float);
			Assert::IsTrue(Datum::DatumType::Float == floatDatum.Type());

			Datum vec4Datum;
			Assert::IsTrue(Datum::DatumType::Unknown == vec4Datum.Type());
			vec4Datum.SetType(Datum::DatumType::Vector4);
			Assert::IsTrue(Datum::DatumType::Vector4 == vec4Datum.Type());

			Datum mat4x4Datum;
			Assert::IsTrue(Datum::DatumType::Unknown == mat4x4Datum.Type());
			mat4x4Datum.SetType(Datum::DatumType::Matrix4x4);
			Assert::IsTrue(Datum::DatumType::Matrix4x4 == mat4x4Datum.Type());

			Datum stringDatum;
			Assert::IsTrue(Datum::DatumType::Unknown == stringDatum.Type());
			stringDatum.SetType(Datum::DatumType::String);
			Assert::IsTrue(Datum::DatumType::String == stringDatum.Type());

			Datum RTTIDatum;
			Assert::IsTrue(Datum::DatumType::Unknown == RTTIDatum.Type());
			RTTIDatum.SetType(Datum::DatumType::RTTIPtr);

			auto typeAlreadySetException = [&stringDatum] {stringDatum.SetType(Datum::DatumType::Unknown); };
			Assert::ExpectException<std::exception>(typeAlreadySetException);
		}

		TEST_METHOD(EquivalenceOperatorsTest)
		{
			size_t testSize = 16;
			Vector<int> testInts;
			Vector<float> testFloats;
			Vector<vec4> testVec4s;
			Vector<mat4x4> testMat4x4s;
			Vector<string> testStrings;
			Vector<Foo> testFoos;
			Vector<RTTI*> testRTTIs;
			for (size_t i = 0; i < testSize; i++)
			{
				testInts.PushBack(rand());
				testFloats.PushBack(randFloat());
				testVec4s.PushBack(vec4(randFloat(), randFloat(), randFloat(), randFloat()));
				testMat4x4s.PushBack(mat4x4((randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat())));
				testStrings.PushBack(random_string(testSize));
				testFoos.PushBack(Foo(rand()));
			}
			//Must be seperate from other for loop because Foo vector will resize and change the address od it's stored Foos.
			for (size_t i = 0; i < testSize; i++)
			{
				testRTTIs.PushBack(&testFoos[i]);
			}

			Datum unknownDatum;
			Datum unknownDatum2;
			
			Assert::IsTrue(unknownDatum == unknownDatum2);

			Datum intDatum = { testInts[0], testInts[1], testInts[2], testInts[3],testInts[4], testInts[5], testInts[6], testInts[7],testInts[8], testInts[9], testInts[10], testInts[11],testInts[12], testInts[13], testInts[14], testInts[15] };
			Datum intDatum2 = { testInts[0], testInts[1], testInts[2], testInts[3],testInts[4], testInts[5], testInts[6], testInts[7],testInts[8], testInts[9], testInts[10], testInts[11],testInts[12], testInts[13], testInts[14], testInts[15] };
			Datum intDatum3 = { testInts[15], testInts[14], testInts[13], testInts[12],testInts[11], testInts[10], testInts[9], testInts[8],testInts[7], testInts[6], testInts[5], testInts[4],testInts[3], testInts[2], testInts[1], testInts[0] };

			Assert::IsTrue(intDatum == intDatum2);
			Assert::IsTrue(intDatum != intDatum3);

			intDatum = 1;
			Assert::IsTrue(intDatum == 1);
			Assert::IsFalse(intDatum != 1);

			Datum floatDatum = { testFloats[0], testFloats[1], testFloats[2], testFloats[3],testFloats[4], testFloats[5], testFloats[6], testFloats[7],testFloats[8], testFloats[9], testFloats[10], testFloats[11],testFloats[12], testFloats[13], testFloats[14], testFloats[15] };
			Datum floatDatum2 = { testFloats[0], testFloats[1], testFloats[2], testFloats[3],testFloats[4], testFloats[5], testFloats[6], testFloats[7],testFloats[8], testFloats[9], testFloats[10], testFloats[11],testFloats[12], testFloats[13], testFloats[14], testFloats[15] };
			Datum floatDatum3 = { testFloats[15], testFloats[14], testFloats[13], testFloats[12],testFloats[11], testFloats[10], testFloats[9], testFloats[8],testFloats[7], testFloats[6], testFloats[5], testFloats[4],testFloats[3], testFloats[2], testFloats[1], testFloats[0] };

			Assert::IsTrue(floatDatum == floatDatum2);
			Assert::IsTrue(floatDatum != floatDatum3);

			floatDatum = 1.0f;
			Assert::IsTrue(floatDatum == 1.0f);
			Assert::IsFalse(floatDatum != 1.0f);

			Datum vec4Datum = { testVec4s[0], testVec4s[1], testVec4s[2], testVec4s[3],testVec4s[4], testVec4s[5], testVec4s[6], testVec4s[7],testVec4s[8], testVec4s[9], testVec4s[10], testVec4s[11],testVec4s[12], testVec4s[13], testVec4s[14], testVec4s[15] };
			Datum vec4Datum2 = { testVec4s[0], testVec4s[1], testVec4s[2], testVec4s[3],testVec4s[4], testVec4s[5], testVec4s[6], testVec4s[7],testVec4s[8], testVec4s[9], testVec4s[10], testVec4s[11],testVec4s[12], testVec4s[13], testVec4s[14], testVec4s[15] };
			Datum vec4Datum3 = { testVec4s[15], testVec4s[14], testVec4s[13], testVec4s[12],testVec4s[11], testVec4s[10], testVec4s[9], testVec4s[8],testVec4s[7], testVec4s[6], testVec4s[5], testVec4s[4],testVec4s[3], testVec4s[2], testVec4s[1], testVec4s[0] };

			Assert::IsTrue(vec4Datum == vec4Datum2);
			Assert::IsTrue(vec4Datum != vec4Datum3);

			vec4Datum = vec4(1.0f, 2.0f, 3.0f, 4.0f);
			Assert::IsTrue(vec4Datum == vec4(1.0f, 2.0f, 3.0f, 4.0f));
			Assert::IsFalse(vec4Datum != vec4(1.0f, 2.0f, 3.0f, 4.0f));


			Datum mat4x4Datum = { testMat4x4s[0], testMat4x4s[1], testMat4x4s[2], testMat4x4s[3],testMat4x4s[4], testMat4x4s[5], testMat4x4s[6], testMat4x4s[7],testMat4x4s[8], testMat4x4s[9], testMat4x4s[10], testMat4x4s[11],testMat4x4s[12], testMat4x4s[13], testMat4x4s[14], testMat4x4s[15] };
			Datum mat4x4Datum2 = { testMat4x4s[0], testMat4x4s[1], testMat4x4s[2], testMat4x4s[3],testMat4x4s[4], testMat4x4s[5], testMat4x4s[6], testMat4x4s[7],testMat4x4s[8], testMat4x4s[9], testMat4x4s[10], testMat4x4s[11],testMat4x4s[12], testMat4x4s[13], testMat4x4s[14], testMat4x4s[15] };
			Datum mat4x4Datum3 = { testMat4x4s[15], testMat4x4s[14], testMat4x4s[13], testMat4x4s[12],testMat4x4s[11], testMat4x4s[10], testMat4x4s[9], testMat4x4s[8],testMat4x4s[7], testMat4x4s[6], testMat4x4s[5], testMat4x4s[4],testMat4x4s[3], testMat4x4s[2], testMat4x4s[1], testMat4x4s[0] };

			Assert::IsTrue(mat4x4Datum == mat4x4Datum2);
			Assert::IsTrue(mat4x4Datum != mat4x4Datum3);

			mat4x4Datum = mat4x4(1.0f, 2.0f, 3.0f, 4.0f, 5.0f , 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f);
			Assert::IsTrue(mat4x4Datum == mat4x4(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f));
			Assert::IsFalse(mat4x4Datum != mat4x4(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f));

			Datum stringDatum = { testStrings[0], testStrings[1], testStrings[2], testStrings[3],testStrings[4], testStrings[5], testStrings[6], testStrings[7],testStrings[8], testStrings[9], testStrings[10], testStrings[11],testStrings[12], testStrings[13], testStrings[14], testStrings[15] };
			Datum stringDatum2 = { testStrings[0], testStrings[1], testStrings[2], testStrings[3],testStrings[4], testStrings[5], testStrings[6], testStrings[7],testStrings[8], testStrings[9], testStrings[10], testStrings[11],testStrings[12], testStrings[13], testStrings[14], testStrings[15] };
			Datum stringDatum3 = { testStrings[15], testStrings[14], testStrings[13], testStrings[12],testStrings[11], testStrings[10], testStrings[9], testStrings[8],testStrings[7], testStrings[6], testStrings[5], testStrings[4],testStrings[3], testStrings[2], testStrings[1], testStrings[0] };

			Assert::IsTrue(stringDatum == stringDatum2);
			Assert::IsTrue(stringDatum != stringDatum3);

			stringDatum = "Hello";
			Assert::IsTrue(stringDatum == "Hello");
			Assert::IsFalse(stringDatum != "Hello");

			Datum RTTIDatum = { testRTTIs[0], testRTTIs[1], testRTTIs[2], testRTTIs[3],testRTTIs[4], testRTTIs[5], testRTTIs[6], testRTTIs[7],testRTTIs[8], testRTTIs[9], testRTTIs[10], testRTTIs[11],testRTTIs[12], testRTTIs[13], testRTTIs[14], testRTTIs[15] };
			Datum RTTIDatum2 = { testRTTIs[0], testRTTIs[1], testRTTIs[2], testRTTIs[3],testRTTIs[4], testRTTIs[5], testRTTIs[6], testRTTIs[7],testRTTIs[8], testRTTIs[9], testRTTIs[10], testRTTIs[11],testRTTIs[12], testRTTIs[13], testRTTIs[14], testRTTIs[15] };
			Datum RTTIDatum3 = { testRTTIs[15], testRTTIs[14], testRTTIs[13], testRTTIs[12],testRTTIs[11], testRTTIs[10], testRTTIs[9], testRTTIs[8],testRTTIs[7], testRTTIs[6], testRTTIs[5], testRTTIs[4],testRTTIs[3], testRTTIs[2], testRTTIs[1], testRTTIs[0] };
			
			Assert::IsTrue(RTTIDatum == RTTIDatum2);
			Assert::IsTrue(RTTIDatum != RTTIDatum3);

			RTTIDatum = static_cast<RTTI*>(nullptr);
			Assert::IsTrue(RTTIDatum == nullptr);
			Assert::IsFalse(RTTIDatum != nullptr);

			RTTIDatum = testRTTIs[0];
			Assert::IsTrue(RTTIDatum == testRTTIs[0]);
			Assert::IsFalse(RTTIDatum != testRTTIs[0]);

			Assert::IsTrue(intDatum != "Hello");
			Assert::IsTrue(floatDatum != 1);
			Assert::IsTrue(vec4Datum != testRTTIs[0]);
			Assert::IsTrue(mat4x4Datum != vec4(1.0f, 2.0f, 3.0f, 4.0f));
			Assert::IsTrue(stringDatum != 1.0f);
			Assert::IsTrue(RTTIDatum != mat4x4(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f));
		}

		TEST_METHOD(GetTest)
		{
			size_t testSize = 16;
			Vector<int> testInts;
			Vector<float> testFloats;
			Vector<vec4> testVec4s;
			Vector<mat4x4> testMat4x4s;
			Vector<string> testStrings;
			Vector<Foo> testFoos;
			Vector<RTTI*> testRTTIs;
			for (size_t i = 0; i < testSize; i++)
			{
				testInts.PushBack(rand());
				testFloats.PushBack(randFloat());
				testVec4s.PushBack(vec4(randFloat(), randFloat(), randFloat(), randFloat()));
				testMat4x4s.PushBack(mat4x4((randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat())));
				testStrings.PushBack(random_string(testSize));
				testFoos.PushBack(Foo(rand()));
			}
			//Must be seperate from other for loop because Foo vector will resize and change the address od it's stored Foos.
			for (size_t i = 0; i < testSize; i++)
			{
				testRTTIs.PushBack(&testFoos[i]);
			}

			Datum intDatum = { testInts[0], testInts[1], testInts[2], testInts[3],testInts[4], testInts[5], testInts[6], testInts[7],testInts[8], testInts[9], testInts[10], testInts[11],testInts[12], testInts[13], testInts[14], testInts[15] };
			Datum floatDatum = { testFloats[0], testFloats[1], testFloats[2], testFloats[3],testFloats[4], testFloats[5], testFloats[6], testFloats[7],testFloats[8], testFloats[9], testFloats[10], testFloats[11],testFloats[12], testFloats[13], testFloats[14], testFloats[15] };
			Datum vec4Datum = { testVec4s[0], testVec4s[1], testVec4s[2], testVec4s[3],testVec4s[4], testVec4s[5], testVec4s[6], testVec4s[7],testVec4s[8], testVec4s[9], testVec4s[10], testVec4s[11],testVec4s[12], testVec4s[13], testVec4s[14], testVec4s[15] };
			Datum mat4x4Datum = { testMat4x4s[0], testMat4x4s[1], testMat4x4s[2], testMat4x4s[3],testMat4x4s[4], testMat4x4s[5], testMat4x4s[6], testMat4x4s[7],testMat4x4s[8], testMat4x4s[9], testMat4x4s[10], testMat4x4s[11],testMat4x4s[12], testMat4x4s[13], testMat4x4s[14], testMat4x4s[15] };
			Datum stringDatum = { testStrings[0], testStrings[1], testStrings[2], testStrings[3],testStrings[4], testStrings[5], testStrings[6], testStrings[7],testStrings[8], testStrings[9], testStrings[10], testStrings[11],testStrings[12], testStrings[13], testStrings[14], testStrings[15] };
			Datum RTTIDatum = { testRTTIs[0], testRTTIs[1], testRTTIs[2], testRTTIs[3],testRTTIs[4], testRTTIs[5], testRTTIs[6], testRTTIs[7],testRTTIs[8], testRTTIs[9], testRTTIs[10], testRTTIs[11],testRTTIs[12], testRTTIs[13], testRTTIs[14], testRTTIs[15] };
			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue(intDatum.Get<int>(i) == testInts[i]);
				Assert::IsTrue(floatDatum.Get<float>(i) == testFloats[i]);
				Assert::IsTrue(vec4Datum.Get<vec4>(i) == testVec4s[i]);
				Assert::IsTrue(mat4x4Datum.Get<mat4x4>(i) == testMat4x4s[i]);
				Assert::IsTrue(stringDatum.Get<string>(i) == testStrings[i]);
				Assert::IsTrue(RTTIDatum.Get<RTTI*>(i) == testRTTIs[i]);

				auto intTypeMismatchException = [&intDatum, &i] {intDatum.Get<float>(i); };
				Assert::ExpectException<std::exception>(intTypeMismatchException);
				auto floatTypeMismatchException = [&floatDatum, &i] {floatDatum.Get<vec4>(i); };
				Assert::ExpectException<std::exception>(floatTypeMismatchException);
				auto vec4TypeMismatchException = [&vec4Datum, &i] {vec4Datum.Get<mat4x4>(i); };
				Assert::ExpectException<std::exception>(vec4TypeMismatchException);
				auto mat4x4TypeMismatchException = [&mat4x4Datum, &i] {mat4x4Datum.Get<string>(i); };
				Assert::ExpectException<std::exception>(mat4x4TypeMismatchException);
				auto stringTypeMismatchException = [&stringDatum, &i] {stringDatum.Get<RTTI*>(i); };
				Assert::ExpectException<std::exception>(stringTypeMismatchException);
				auto RTTITypeMismatchException = [&RTTIDatum, &i] {RTTIDatum.Get<int>(i); };
				Assert::ExpectException<std::exception>(RTTITypeMismatchException);

				auto intOutOfBoundsMismatchException = [&intDatum, &testSize] {intDatum.Get<int>(testSize); };
				Assert::ExpectException<std::exception>(intOutOfBoundsMismatchException);
				auto floatOutOfBoundsMismatchException = [&floatDatum, testSize] {floatDatum.Get<float>(testSize); };
				Assert::ExpectException<std::exception>(floatOutOfBoundsMismatchException);
				auto vec4OutOfBoundsMismatchException = [&vec4Datum, testSize] {vec4Datum.Get<vec4>(testSize); };
				Assert::ExpectException<std::exception>(vec4OutOfBoundsMismatchException);
				auto mat4x4OutOfBoundsMismatchException = [&mat4x4Datum, testSize] {mat4x4Datum.Get<mat4x4>(testSize); };
				Assert::ExpectException<std::exception>(mat4x4OutOfBoundsMismatchException);
				auto stringOutOfBoundsMismatchException = [&stringDatum, testSize] {stringDatum.Get<string>(testSize); };
				Assert::ExpectException<std::exception>(stringOutOfBoundsMismatchException);
				auto RTTIOutOfBoundsMismatchException = [&RTTIDatum, testSize] {RTTIDatum.Get<RTTI*>(testSize); };
				Assert::ExpectException<std::exception>(RTTIOutOfBoundsMismatchException);

				auto getUnsupportedTypeException = [&intDatum, &testSize] {intDatum.Get<char>(testSize); };
				Assert::ExpectException<std::exception>(getUnsupportedTypeException);

				auto frontUnsupportedTypeException = [&intDatum, &testSize] {intDatum.Front<char>(); };
				Assert::ExpectException<std::exception>(frontUnsupportedTypeException);

				auto backUnsupportedTypeException = [&intDatum, &testSize] {intDatum.Back<char>(); };
				Assert::ExpectException<std::exception>(backUnsupportedTypeException);
				
			}
		}

		TEST_METHOD(SetTest)
		{
			size_t testSize = 16;
			Vector<int> testInts;
			Vector<float> testFloats;
			Vector<vec4> testVec4s;
			Vector<mat4x4> testMat4x4s;
			Vector<string> testStrings;
			Vector<Foo> testFoos;
			Vector<RTTI*> testRTTIs;
			for (size_t i = 0; i < testSize; i++)
			{
				testInts.PushBack(rand());
				testFloats.PushBack(randFloat());
				testVec4s.PushBack(vec4(randFloat(), randFloat(), randFloat(), randFloat()));
				testMat4x4s.PushBack(mat4x4((randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat())));
				testStrings.PushBack(random_string(testSize));
				testFoos.PushBack(Foo(rand()));
			}
			//Must be seperate from other for loop because Foo vector will resize and change the address od it's stored Foos.
			for (size_t i = 0; i < testSize; i++)
			{
				testRTTIs.PushBack(&testFoos[i]);
			}

			Datum intDatum = { testInts[0], testInts[1], testInts[2], testInts[3],testInts[4], testInts[5], testInts[6], testInts[7],testInts[8], testInts[9], testInts[10], testInts[11],testInts[12], testInts[13], testInts[14], testInts[15] };
			Datum floatDatum = { testFloats[0], testFloats[1], testFloats[2], testFloats[3],testFloats[4], testFloats[5], testFloats[6], testFloats[7],testFloats[8], testFloats[9], testFloats[10], testFloats[11],testFloats[12], testFloats[13], testFloats[14], testFloats[15] };
			Datum vec4Datum = { testVec4s[0], testVec4s[1], testVec4s[2], testVec4s[3],testVec4s[4], testVec4s[5], testVec4s[6], testVec4s[7],testVec4s[8], testVec4s[9], testVec4s[10], testVec4s[11],testVec4s[12], testVec4s[13], testVec4s[14], testVec4s[15] };
			Datum mat4x4Datum = { testMat4x4s[0], testMat4x4s[1], testMat4x4s[2], testMat4x4s[3],testMat4x4s[4], testMat4x4s[5], testMat4x4s[6], testMat4x4s[7],testMat4x4s[8], testMat4x4s[9], testMat4x4s[10], testMat4x4s[11],testMat4x4s[12], testMat4x4s[13], testMat4x4s[14], testMat4x4s[15] };
			Datum stringDatum = { testStrings[0], testStrings[1], testStrings[2], testStrings[3],testStrings[4], testStrings[5], testStrings[6], testStrings[7],testStrings[8], testStrings[9], testStrings[10], testStrings[11],testStrings[12], testStrings[13], testStrings[14], testStrings[15] };
			Datum RTTIDatum = { testRTTIs[0], testRTTIs[1], testRTTIs[2], testRTTIs[3],testRTTIs[4], testRTTIs[5], testRTTIs[6], testRTTIs[7],testRTTIs[8], testRTTIs[9], testRTTIs[10], testRTTIs[11],testRTTIs[12], testRTTIs[13], testRTTIs[14], testRTTIs[15] };
			for (size_t i = 0; i < testSize; i++)
			{
				intDatum.Set(static_cast<int>(i), i);
				floatDatum.Set(static_cast<float>(i), i);
				vec4Datum.Set(vec4(i, i, i, i), i);
				mat4x4Datum.Set(mat4x4(i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i), i);
				stringDatum.Set(glm::to_string(mat4x4(i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i)), i);
				RTTIDatum.Set(reinterpret_cast<RTTI*>(i), i);

				Assert::IsTrue(intDatum.Get<int>(i) == static_cast<int>(i));
				Assert::IsTrue(floatDatum.Get<float>(i) == static_cast<float>(i));
				Assert::IsTrue(vec4Datum.Get<vec4>(i) == vec4(i, i, i, i));
				Assert::IsTrue(mat4x4Datum.Get<mat4x4>(i) == mat4x4(i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i));
				Assert::IsTrue(stringDatum.Get<string>(i) == glm::to_string(mat4x4(i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i)));
				Assert::IsTrue(RTTIDatum.Get<RTTI*>(i) == reinterpret_cast<RTTI*>(i));

				auto intTypeMismatchException = [&intDatum, &i] {intDatum.Set(1.0f, i); };
				Assert::ExpectException<std::exception>(intTypeMismatchException);

				auto floatTypeMismatchException = [&floatDatum, &i] {floatDatum.Set(vec4(0, 1, 2, 3), i); };
				Assert::ExpectException<std::exception>(floatTypeMismatchException);

				auto vec4TypeMismatchException = [&vec4Datum, &i] {vec4Datum.Set(mat4x4(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15), i); };
				Assert::ExpectException<std::exception>(vec4TypeMismatchException);

				auto mat4x4TypeMismatchException = [&mat4x4Datum, &i] {mat4x4Datum.Set("Hello", i); };
				Assert::ExpectException<std::exception>(mat4x4TypeMismatchException);

				auto stringTypeMismatchException = [&stringDatum, &i] {stringDatum.Set(static_cast<RTTI*>(nullptr), i); };
				Assert::ExpectException<std::exception>(stringTypeMismatchException);

				auto RTTITypeMismatchException = [&RTTIDatum, &i] {RTTIDatum.Set(1, i); };
				Assert::ExpectException<std::exception>(RTTITypeMismatchException);
			}
		}

		TEST_METHOD(ReserveTest)
		{
			size_t testSize = 16;
			size_t resizeSize = testSize * 10;
			Vector<int> testInts;
			Vector<float> testFloats;
			Vector<vec4> testVec4s;
			Vector<mat4x4> testMat4x4s;
			Vector<string> testStrings;
			Vector<Foo> testFoos;
			Vector<RTTI*> testRTTIs;
			for (size_t i = 0; i < testSize; i++)
			{
				testInts.PushBack(rand());
				testFloats.PushBack(randFloat());
				testVec4s.PushBack(vec4(randFloat(), randFloat(), randFloat(), randFloat()));
				testMat4x4s.PushBack(mat4x4((randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat())));
				testStrings.PushBack(random_string(testSize));
				testFoos.PushBack(Foo(rand()));
			}
			//Must be seperate from other for loop because Foo vector will resize and change the address od it's stored Foos.
			for (size_t i = 0; i < testSize; i++)
			{
				testRTTIs.PushBack(&testFoos[i]);
			}

			Datum intDatum = { testInts[0], testInts[1], testInts[2], testInts[3],testInts[4], testInts[5], testInts[6], testInts[7],testInts[8], testInts[9], testInts[10], testInts[11],testInts[12], testInts[13], testInts[14], testInts[15] };
			Assert::IsTrue(intDatum.Size() == testSize);
			size_t intCapacityBefore = intDatum.Capacity();
			intDatum.Reserve(testSize - 1);
			Assert::IsTrue(intDatum.Size() == testSize);
			Assert::IsTrue(intDatum.Capacity() == intCapacityBefore);
			intDatum.Reserve(resizeSize);
			Assert::IsTrue(intDatum.Size() == testSize);
			Assert::IsTrue(intDatum.Capacity() == resizeSize);
			

			Datum floatDatum = { testFloats[0], testFloats[1], testFloats[2], testFloats[3],testFloats[4], testFloats[5], testFloats[6], testFloats[7],testFloats[8], testFloats[9], testFloats[10], testFloats[11],testFloats[12], testFloats[13], testFloats[14], testFloats[15] };
			Assert::IsTrue(floatDatum.Size() == testSize);
			size_t floatCapacityBefore = floatDatum.Capacity();
			floatDatum.Reserve(testSize - 1);
			Assert::IsTrue(floatDatum.Size() == testSize);
			Assert::IsTrue(floatDatum.Capacity() == floatCapacityBefore);
			floatDatum.Reserve(resizeSize);
			Assert::IsTrue(floatDatum.Size() == testSize);
			Assert::IsTrue(floatDatum.Capacity() == resizeSize);

			Datum vec4Datum = { testVec4s[0], testVec4s[1], testVec4s[2], testVec4s[3],testVec4s[4], testVec4s[5], testVec4s[6], testVec4s[7],testVec4s[8], testVec4s[9], testVec4s[10], testVec4s[11],testVec4s[12], testVec4s[13], testVec4s[14], testVec4s[15] };
			Assert::IsTrue(vec4Datum.Size() == testSize);
			size_t vec4CapacityBefore = vec4Datum.Capacity();
			vec4Datum.Reserve(testSize - 1);
			Assert::IsTrue(vec4Datum.Size() == testSize);
			Assert::IsTrue(vec4Datum.Capacity() == vec4CapacityBefore);
			vec4Datum.Reserve(resizeSize);
			Assert::IsTrue(vec4Datum.Size() == testSize);
			Assert::IsTrue(vec4Datum.Capacity() == resizeSize);
			

			Datum mat4x4Datum = { testMat4x4s[0], testMat4x4s[1], testMat4x4s[2], testMat4x4s[3],testMat4x4s[4], testMat4x4s[5], testMat4x4s[6], testMat4x4s[7],testMat4x4s[8], testMat4x4s[9], testMat4x4s[10], testMat4x4s[11],testMat4x4s[12], testMat4x4s[13], testMat4x4s[14], testMat4x4s[15] };
			Assert::IsTrue(mat4x4Datum.Size() == testSize);
			size_t mat4x4CapacityBefore = mat4x4Datum.Capacity();
			mat4x4Datum.Reserve(testSize - 1);
			Assert::IsTrue(mat4x4Datum.Size() == testSize);
			Assert::IsTrue(mat4x4Datum.Capacity() == mat4x4CapacityBefore);
			mat4x4Datum.Reserve(resizeSize);
			Assert::IsTrue(mat4x4Datum.Size() == testSize);
			Assert::IsTrue(mat4x4Datum.Capacity() == resizeSize);

			Datum stringDatum = { testStrings[0], testStrings[1], testStrings[2], testStrings[3],testStrings[4], testStrings[5], testStrings[6], testStrings[7],testStrings[8], testStrings[9], testStrings[10], testStrings[11],testStrings[12], testStrings[13], testStrings[14], testStrings[15] };
			Assert::IsTrue(stringDatum.Size() == testSize);
			size_t stringCapacityBefore = stringDatum.Capacity();
			stringDatum.Reserve(testSize - 1);
			Assert::IsTrue(stringDatum.Size() == testSize);
			Assert::IsTrue(stringDatum.Capacity() == stringCapacityBefore);
			stringDatum.Reserve(resizeSize);
			Assert::IsTrue(stringDatum.Size() == testSize);
			Assert::IsTrue(stringDatum.Capacity() == resizeSize);

			Datum RTTIDatum = { testRTTIs[0], testRTTIs[1], testRTTIs[2], testRTTIs[3],testRTTIs[4], testRTTIs[5], testRTTIs[6], testRTTIs[7],testRTTIs[8], testRTTIs[9], testRTTIs[10], testRTTIs[11],testRTTIs[12], testRTTIs[13], testRTTIs[14], testRTTIs[15] };
			Assert::IsTrue(RTTIDatum.Size() == testSize);
			size_t RTTICapacityBefore = RTTIDatum.Capacity();
			RTTIDatum.Reserve(testSize - 1);
			Assert::IsTrue(RTTIDatum.Size() == testSize);
			Assert::IsTrue(RTTIDatum.Capacity() == RTTICapacityBefore);
			RTTIDatum.Reserve(resizeSize);
			Assert::IsTrue(RTTIDatum.Size() == testSize);
			Assert::IsTrue(RTTIDatum.Capacity() == resizeSize);
			

			string* testStringArr = new string[testSize];
		

			for (size_t i = 0; i < testSize; i++)
			{
				testStringArr[i] = random_string(32);
			}

			Datum stringDatumExternal;

			Datum unknownDatum;

			stringDatumExternal.SetStorage(testStringArr, testSize);

			auto cannotReserveWhenExternalException = [&stringDatumExternal] {stringDatumExternal.Reserve(1); };
			Assert::ExpectException<std::exception>(cannotReserveWhenExternalException);

			auto cannotReserveUnknownTypeException = [&unknownDatum] {unknownDatum.Reserve(1); };
			Assert::ExpectException<std::exception>(cannotReserveUnknownTypeException);

			delete[] testStringArr;
		}

		TEST_METHOD(ShrinkToFitTest)
		{
			size_t testSize = 16;
			size_t resizeSize = testSize * 10;
			Vector<int> testInts;
			Vector<float> testFloats;
			Vector<vec4> testVec4s;
			Vector<mat4x4> testMat4x4s;
			Vector<string> testStrings;
			Vector<Foo> testFoos;
			Vector<RTTI*> testRTTIs;
			for (size_t i = 0; i < testSize; i++)
			{
				testInts.PushBack(rand());
				testFloats.PushBack(randFloat());
				testVec4s.PushBack(vec4(randFloat(), randFloat(), randFloat(), randFloat()));
				testMat4x4s.PushBack(mat4x4((randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat())));
				testStrings.PushBack(random_string(testSize));
				testFoos.PushBack(Foo(rand()));
			}
			//Must be seperate from other for loop because Foo vector will resize and change the address od it's stored Foos.
			for (size_t i = 0; i < testSize; i++)
			{
				testRTTIs.PushBack(&testFoos[i]);
			}

			Datum intDatum = { testInts[0], testInts[1], testInts[2], testInts[3],testInts[4], testInts[5], testInts[6], testInts[7],testInts[8], testInts[9], testInts[10], testInts[11],testInts[12], testInts[13], testInts[14], testInts[15] };
			Assert::IsTrue(intDatum.Size() == testSize);
			intDatum.Reserve(resizeSize);
			Assert::IsTrue(intDatum.Capacity() > intDatum.Size());
			intDatum.ShrinkToFit();
			Assert::IsTrue(intDatum.Size() == intDatum.Capacity());


			Datum floatDatum = { testFloats[0], testFloats[1], testFloats[2], testFloats[3],testFloats[4], testFloats[5], testFloats[6], testFloats[7],testFloats[8], testFloats[9], testFloats[10], testFloats[11],testFloats[12], testFloats[13], testFloats[14], testFloats[15] };
			Assert::IsTrue(floatDatum.Size() == testSize);
			floatDatum.Reserve(resizeSize);
			Assert::IsTrue(floatDatum.Capacity() > floatDatum.Size());
			floatDatum.ShrinkToFit();
			Assert::IsTrue(floatDatum.Size() == floatDatum.Capacity());

			Datum vec4Datum = { testVec4s[0], testVec4s[1], testVec4s[2], testVec4s[3],testVec4s[4], testVec4s[5], testVec4s[6], testVec4s[7],testVec4s[8], testVec4s[9], testVec4s[10], testVec4s[11],testVec4s[12], testVec4s[13], testVec4s[14], testVec4s[15] };
			Assert::IsTrue(vec4Datum.Size() == testSize);
			vec4Datum.Reserve(resizeSize);
			Assert::IsTrue(vec4Datum.Capacity() > vec4Datum.Size());
			vec4Datum.ShrinkToFit();
			Assert::IsTrue(vec4Datum.Size() == vec4Datum.Capacity());


			Datum mat4x4Datum = { testMat4x4s[0], testMat4x4s[1], testMat4x4s[2], testMat4x4s[3],testMat4x4s[4], testMat4x4s[5], testMat4x4s[6], testMat4x4s[7],testMat4x4s[8], testMat4x4s[9], testMat4x4s[10], testMat4x4s[11],testMat4x4s[12], testMat4x4s[13], testMat4x4s[14], testMat4x4s[15] };
			Assert::IsTrue(mat4x4Datum.Size() == testSize);
			mat4x4Datum.Reserve(resizeSize);
			Assert::IsTrue(mat4x4Datum.Capacity() > mat4x4Datum.Size());
			mat4x4Datum.ShrinkToFit();
			Assert::IsTrue(mat4x4Datum.Size() == mat4x4Datum.Capacity());

			Datum stringDatum = { testStrings[0], testStrings[1], testStrings[2], testStrings[3],testStrings[4], testStrings[5], testStrings[6], testStrings[7],testStrings[8], testStrings[9], testStrings[10], testStrings[11],testStrings[12], testStrings[13], testStrings[14], testStrings[15] };
			Assert::IsTrue(stringDatum.Size() == testSize);
			stringDatum.Reserve(resizeSize);
			Assert::IsTrue(stringDatum.Capacity() > stringDatum.Size());
			stringDatum.ShrinkToFit();
			Assert::IsTrue(stringDatum.Size() == stringDatum.Capacity());

			Datum RTTIDatum = { testRTTIs[0], testRTTIs[1], testRTTIs[2], testRTTIs[3],testRTTIs[4], testRTTIs[5], testRTTIs[6], testRTTIs[7],testRTTIs[8], testRTTIs[9], testRTTIs[10], testRTTIs[11],testRTTIs[12], testRTTIs[13], testRTTIs[14], testRTTIs[15] };
			Assert::IsTrue(RTTIDatum.Size() == testSize);
			RTTIDatum.Reserve(resizeSize);
			Assert::IsTrue(RTTIDatum.Capacity() > RTTIDatum.Size());
			RTTIDatum.ShrinkToFit();
			Assert::IsTrue(RTTIDatum.Size() == RTTIDatum.Capacity());

			Datum unknownDatum;
			unknownDatum.ShrinkToFit();

		}
		
		TEST_METHOD(ResizeTest)
		{
			size_t testSize = 16;
			size_t resizeSize = testSize * 10;
			Vector<int> testInts;
			Vector<float> testFloats;
			Vector<vec4> testVec4s;
			Vector<mat4x4> testMat4x4s;
			Vector<string> testStrings;
			Vector<Foo> testFoos;
			Vector<RTTI*> testRTTIs;
			for (size_t i = 0; i < testSize; i++)
			{
				testInts.PushBack(rand());
				testFloats.PushBack(randFloat());
				testVec4s.PushBack(vec4(randFloat(), randFloat(), randFloat(), randFloat()));
				testMat4x4s.PushBack(mat4x4((randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat())));
				testStrings.PushBack(random_string(testSize));
				testFoos.PushBack(Foo(rand()));
			}
			//Must be seperate from other for loop because Foo vector will resize and change the address od it's stored Foos.
			for (size_t i = 0; i < testSize; i++)
			{
				testRTTIs.PushBack(&testFoos[i]);
			}

			Datum intDatum = { testInts[0], testInts[1], testInts[2], testInts[3],testInts[4], testInts[5], testInts[6], testInts[7],testInts[8], testInts[9], testInts[10], testInts[11],testInts[12], testInts[13], testInts[14], testInts[15] };
			intDatum.ShrinkToFit();
			Assert::IsTrue(intDatum.Size() == testSize);
			Assert::IsTrue(intDatum.Capacity() == testSize);
			intDatum.Resize(resizeSize);
			Assert::IsTrue(intDatum.Size() == resizeSize);
			Assert::IsTrue(intDatum.Capacity() == resizeSize);
			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue(intDatum.Get<int>(i) == testInts[i]);
			}
			for (size_t i = testSize; i < resizeSize; i++)
			{
				Assert::IsTrue(intDatum.Get<int>(i) == int());
			}
			intDatum.Resize(testSize);
			Assert::IsTrue(intDatum.Size() == testSize);
			Assert::IsTrue(intDatum.Capacity() == resizeSize);
			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue(intDatum.Get<int>(i) == testInts[i]);
			}
			intDatum.Resize(testSize + 1);
			Assert::IsTrue(intDatum.Size() == testSize + 1);
			Assert::IsTrue(intDatum.Capacity() == resizeSize);
			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue(intDatum.Get<int>(i) == testInts[i]);
			}
			Assert::IsTrue(intDatum.Get<int>(testSize) == int());
			

			Datum floatDatum = { testFloats[0], testFloats[1], testFloats[2], testFloats[3],testFloats[4], testFloats[5], testFloats[6], testFloats[7],testFloats[8], testFloats[9], testFloats[10], testFloats[11],testFloats[12], testFloats[13], testFloats[14], testFloats[15] };
			floatDatum.ShrinkToFit();
			Assert::IsTrue(floatDatum.Size() == testSize);
			Assert::IsTrue(floatDatum.Capacity() == testSize);
			floatDatum.Resize(resizeSize);
			Assert::IsTrue(floatDatum.Size() == resizeSize);
			Assert::IsTrue(floatDatum.Capacity() == resizeSize);
			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue(floatDatum.Get<float>(i) == testFloats[i]);
			}
			for (size_t i = testSize; i < resizeSize; i++)
			{
				Assert::IsTrue(floatDatum.Get<float>(i) == float());
			}
			floatDatum.Resize(testSize);
			Assert::IsTrue(floatDatum.Size() == testSize);
			Assert::IsTrue(floatDatum.Capacity() == resizeSize);
			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue(floatDatum.Get<float>(i) == testFloats[i]);
			}
			floatDatum.Resize(testSize + 1);
			Assert::IsTrue(floatDatum.Size() == testSize + 1);
			Assert::IsTrue(floatDatum.Capacity() == resizeSize);
			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue(floatDatum.Get<float>(i) == testFloats[i]);
			}
			Assert::IsTrue(floatDatum.Get<float>(testSize) == float());

			Datum vec4Datum = { testVec4s[0], testVec4s[1], testVec4s[2], testVec4s[3],testVec4s[4], testVec4s[5], testVec4s[6], testVec4s[7],testVec4s[8], testVec4s[9], testVec4s[10], testVec4s[11],testVec4s[12], testVec4s[13], testVec4s[14], testVec4s[15] };
			vec4Datum.ShrinkToFit();
			Assert::IsTrue(vec4Datum.Size() == testSize);
			Assert::IsTrue(vec4Datum.Capacity() == testSize);
			vec4Datum.Resize(resizeSize);
			Assert::IsTrue(vec4Datum.Size() == resizeSize);
			Assert::IsTrue(vec4Datum.Capacity() == resizeSize);
			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue(vec4Datum.Get<vec4>(i) == testVec4s[i]);
			}
			for (size_t i = testSize; i < resizeSize; i++)
			{
				Assert::IsTrue(vec4Datum.Get<vec4>(i) == vec4());
			}
			vec4Datum.Resize(testSize);
			Assert::IsTrue(vec4Datum.Size() == testSize);
			Assert::IsTrue(vec4Datum.Capacity() == resizeSize);
			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue(vec4Datum.Get<vec4>(i) == testVec4s[i]);
			}
			vec4Datum.Resize(testSize + 1);
			Assert::IsTrue(vec4Datum.Size() == testSize + 1);
			Assert::IsTrue(vec4Datum.Capacity() == resizeSize);
			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue(vec4Datum.Get<vec4>(i) == testVec4s[i]);
			}
			Assert::IsTrue(vec4Datum.Get<vec4>(testSize) == vec4());

			Datum mat4x4Datum = { testMat4x4s[0], testMat4x4s[1], testMat4x4s[2], testMat4x4s[3],testMat4x4s[4], testMat4x4s[5], testMat4x4s[6], testMat4x4s[7],testMat4x4s[8], testMat4x4s[9], testMat4x4s[10], testMat4x4s[11],testMat4x4s[12], testMat4x4s[13], testMat4x4s[14], testMat4x4s[15] };
			mat4x4Datum.ShrinkToFit();
			Assert::IsTrue(mat4x4Datum.Size() == testSize);
			Assert::IsTrue(mat4x4Datum.Capacity() == testSize);
			mat4x4Datum.Resize(resizeSize);
			Assert::IsTrue(mat4x4Datum.Size() == resizeSize);
			Assert::IsTrue(mat4x4Datum.Capacity() == resizeSize);
			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue(mat4x4Datum.Get<mat4x4>(i) == testMat4x4s[i]);
			}
			for (size_t i = testSize; i < resizeSize; i++)
			{
				Assert::IsTrue(mat4x4Datum.Get<mat4x4>(i) == mat4x4());
			}
			mat4x4Datum.Resize(testSize);
			Assert::IsTrue(mat4x4Datum.Size() == testSize);
			Assert::IsTrue(mat4x4Datum.Capacity() == resizeSize);
			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue(mat4x4Datum.Get<mat4x4>(i) == testMat4x4s[i]);
			}
			mat4x4Datum.Resize(testSize + 1);
			Assert::IsTrue(mat4x4Datum.Size() == testSize + 1);
			Assert::IsTrue(mat4x4Datum.Capacity() == resizeSize);
			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue(mat4x4Datum.Get<mat4x4>(i) == testMat4x4s[i]);
			}
			Assert::IsTrue(mat4x4Datum.Get<mat4x4>(testSize) == mat4x4());

			Datum stringDatum = { testStrings[0], testStrings[1], testStrings[2], testStrings[3],testStrings[4], testStrings[5], testStrings[6], testStrings[7],testStrings[8], testStrings[9], testStrings[10], testStrings[11],testStrings[12], testStrings[13], testStrings[14], testStrings[15] };
			stringDatum.ShrinkToFit();
			Assert::IsTrue(stringDatum.Size() == testSize);
			Assert::IsTrue(stringDatum.Capacity() == testSize);
			stringDatum.Resize(resizeSize);
			Assert::IsTrue(stringDatum.Size() == resizeSize);
			Assert::IsTrue(stringDatum.Capacity() == resizeSize);
			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue(stringDatum.Get<string>(i) == testStrings[i]);
			}
			for (size_t i = testSize; i < resizeSize; i++)
			{
				Assert::IsTrue(stringDatum.Get<string>(i) == string());
			}
			stringDatum.Resize(testSize);
			Assert::IsTrue(stringDatum.Size() == testSize);
			Assert::IsTrue(stringDatum.Capacity() == resizeSize);
			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue(stringDatum.Get<string>(i) == testStrings[i]);
			}
			stringDatum.Resize(testSize + 1);
			Assert::IsTrue(stringDatum.Size() == testSize + 1);
			Assert::IsTrue(stringDatum.Capacity() == resizeSize);
			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue(stringDatum.Get<string>(i) == testStrings[i]);
			}
			Assert::IsTrue(stringDatum.Get<string>(testSize) == string());

			Datum RTTIDatum = { testRTTIs[0], testRTTIs[1], testRTTIs[2], testRTTIs[3],testRTTIs[4], testRTTIs[5], testRTTIs[6], testRTTIs[7],testRTTIs[8], testRTTIs[9], testRTTIs[10], testRTTIs[11],testRTTIs[12], testRTTIs[13], testRTTIs[14], testRTTIs[15] };
			RTTIDatum.ShrinkToFit();
			Assert::IsTrue(RTTIDatum.Size() == testSize);
			Assert::IsTrue(RTTIDatum.Capacity() == testSize);
			RTTIDatum.Resize(resizeSize);
			Assert::IsTrue(RTTIDatum.Size() == resizeSize);
			Assert::IsTrue(RTTIDatum.Capacity() == resizeSize);
			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue(RTTIDatum.Get<RTTI*>(i) == testRTTIs[i]);
			}
			for (size_t i = testSize; i < resizeSize; i++)
			{
				Assert::IsTrue(RTTIDatum.Get<RTTI*>(i) == nullptr);
			}
			RTTIDatum.Resize(testSize);
			Assert::IsTrue(RTTIDatum.Size() == testSize);
			Assert::IsTrue(RTTIDatum.Capacity() == resizeSize);
			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue(RTTIDatum.Get<RTTI*>(i) == testRTTIs[i]);
			}
			RTTIDatum.Resize(testSize + 1);
			Assert::IsTrue(RTTIDatum.Size() == testSize + 1);
			Assert::IsTrue(RTTIDatum.Capacity() == resizeSize);
			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue(RTTIDatum.Get<RTTI*>(i) == testRTTIs[i]);
			}
			Assert::IsTrue(RTTIDatum.Get<RTTI*>(testSize) == nullptr);



			string * testStringArr = new string[testSize];
			for (size_t i = 0; i < testSize; i++)
			{
				testStringArr[i] = random_string(32);
			}


			Datum stringDatumExternal;

			Datum unknownDatum;

			stringDatumExternal.SetStorage(testStringArr, testSize);

			auto cannotResizeWhenExternalException = [&stringDatumExternal] {stringDatumExternal.Resize(1); };
			Assert::ExpectException<std::exception>(cannotResizeWhenExternalException);

			auto cannotResizeUnknownTypeException = [&unknownDatum] {unknownDatum.Resize(1); };
			Assert::ExpectException<std::exception>(cannotResizeUnknownTypeException);

			delete[] testStringArr;
		}

		TEST_METHOD(SetFromStringTest)
		{
			size_t testSize = 16;
			Vector<int> testInts;
			Vector<float> testFloats;
			Vector<vec4> testVec4s;
			Vector<mat4x4> testMat4x4s;
			Vector<string> testStrings;
			Vector<Foo> testFoos;
			Vector<RTTI*> testRTTIs;
			for (size_t i = 0; i < testSize; i++)
			{
				testInts.PushBack(rand());
				testFloats.PushBack(randFloat());
				testVec4s.PushBack(vec4(randFloat(), randFloat(), randFloat(), randFloat()));
				testMat4x4s.PushBack(mat4x4((randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat())));
				testStrings.PushBack(random_string(testSize));
				testFoos.PushBack(Foo(rand()));
			}
			//Must be seperate from other for loop because Foo vector will resize and change the address od it's stored Foos.
			for (size_t i = 0; i < testSize; i++)
			{
				testRTTIs.PushBack(&testFoos[i]);
			}

			Datum intDatum = { testInts[0], testInts[1], testInts[2], testInts[3],testInts[4], testInts[5], testInts[6], testInts[7],testInts[8], testInts[9], testInts[10], testInts[11],testInts[12], testInts[13], testInts[14], testInts[15] };
			Datum floatDatum = { testFloats[0], testFloats[1], testFloats[2], testFloats[3],testFloats[4], testFloats[5], testFloats[6], testFloats[7],testFloats[8], testFloats[9], testFloats[10], testFloats[11],testFloats[12], testFloats[13], testFloats[14], testFloats[15] };
			Datum vec4Datum = { testVec4s[0], testVec4s[1], testVec4s[2], testVec4s[3],testVec4s[4], testVec4s[5], testVec4s[6], testVec4s[7],testVec4s[8], testVec4s[9], testVec4s[10], testVec4s[11],testVec4s[12], testVec4s[13], testVec4s[14], testVec4s[15] };
			Datum mat4x4Datum = { testMat4x4s[0], testMat4x4s[1], testMat4x4s[2], testMat4x4s[3],testMat4x4s[4], testMat4x4s[5], testMat4x4s[6], testMat4x4s[7],testMat4x4s[8], testMat4x4s[9], testMat4x4s[10], testMat4x4s[11],testMat4x4s[12], testMat4x4s[13], testMat4x4s[14], testMat4x4s[15] };
			Datum stringDatum = { testStrings[0], testStrings[1], testStrings[2], testStrings[3],testStrings[4], testStrings[5], testStrings[6], testStrings[7],testStrings[8], testStrings[9], testStrings[10], testStrings[11],testStrings[12], testStrings[13], testStrings[14], testStrings[15] };
			Datum RTTIDatum = { testRTTIs[0], testRTTIs[1], testRTTIs[2], testRTTIs[3],testRTTIs[4], testRTTIs[5], testRTTIs[6], testRTTIs[7],testRTTIs[8], testRTTIs[9], testRTTIs[10], testRTTIs[11],testRTTIs[12], testRTTIs[13], testRTTIs[14], testRTTIs[15] };
			for (size_t i = 0; i < testSize; i++)
			{
				intDatum.SetFromString(to_string(static_cast<int>(i)), i);
				floatDatum.SetFromString(to_string((static_cast<float>(i))), i);
				vec4Datum.SetFromString(glm::to_string(vec4(i, i, i, i)), i);
				mat4x4Datum.SetFromString(glm::to_string(mat4x4(i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i)), i);
				stringDatum.SetFromString(glm::to_string(mat4x4(i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i)), i);
				auto unsupportedDataTypeException = [&RTTIDatum, &i] {RTTIDatum.SetFromString(to_string(i), i); };
				Assert::ExpectException<std::exception>(unsupportedDataTypeException);

				Assert::IsTrue(intDatum.Get<int>(i) == static_cast<int>(i));
				Assert::IsTrue(floatDatum.Get<float>(i) == static_cast<float>(i));
				Assert::IsTrue(vec4Datum.Get<vec4>(i) == vec4(i, i, i, i));
				Assert::IsTrue(mat4x4Datum.Get<mat4x4>(i) == mat4x4(i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i));
				Assert::IsTrue(stringDatum.Get<string>(i) == glm::to_string(mat4x4(i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i)));
			}

			Datum unknownDatum;

			auto uninitializedTypeException = [&unknownDatum] {unknownDatum.SetFromString("Hello"); };
			Assert::ExpectException<std::exception>(uninitializedTypeException);
		}

		TEST_METHOD(ToStringTest)
		{
			size_t testSize = 16;
			Vector<int> testInts;
			Vector<float> testFloats;
			Vector<vec4> testVec4s;
			Vector<mat4x4> testMat4x4s;
			Vector<string> testStrings;
			Vector<Foo> testFoos;
			Vector<RTTI*> testRTTIs;
			for (size_t i = 0; i < testSize; i++)
			{
				testInts.PushBack(rand());
				testFloats.PushBack(randFloat());
				testVec4s.PushBack(vec4(randFloat(), randFloat(), randFloat(), randFloat()));
				testMat4x4s.PushBack(mat4x4((randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat())));
				testStrings.PushBack(random_string(testSize));
				testFoos.PushBack(Foo(rand()));
			}
			//Must be seperate from other for loop because Foo vector will resize and change the address od it's stored Foos.
			for (size_t i = 0; i < testSize; i++)
			{
				testRTTIs.PushBack(&testFoos[i]);
			}


			Datum intDatum = { testInts[0], testInts[1], testInts[2], testInts[3],testInts[4], testInts[5], testInts[6], testInts[7],testInts[8], testInts[9], testInts[10], testInts[11],testInts[12], testInts[13], testInts[14], testInts[15] };
			Datum floatDatum = { testFloats[0], testFloats[1], testFloats[2], testFloats[3],testFloats[4], testFloats[5], testFloats[6], testFloats[7],testFloats[8], testFloats[9], testFloats[10], testFloats[11],testFloats[12], testFloats[13], testFloats[14], testFloats[15] };
			Datum vec4Datum = { testVec4s[0], testVec4s[1], testVec4s[2], testVec4s[3],testVec4s[4], testVec4s[5], testVec4s[6], testVec4s[7],testVec4s[8], testVec4s[9], testVec4s[10], testVec4s[11],testVec4s[12], testVec4s[13], testVec4s[14], testVec4s[15] };
			Datum mat4x4Datum = { testMat4x4s[0], testMat4x4s[1], testMat4x4s[2], testMat4x4s[3],testMat4x4s[4], testMat4x4s[5], testMat4x4s[6], testMat4x4s[7],testMat4x4s[8], testMat4x4s[9], testMat4x4s[10], testMat4x4s[11],testMat4x4s[12], testMat4x4s[13], testMat4x4s[14], testMat4x4s[15] };
			Datum stringDatum = { testStrings[0], testStrings[1], testStrings[2], testStrings[3],testStrings[4], testStrings[5], testStrings[6], testStrings[7],testStrings[8], testStrings[9], testStrings[10], testStrings[11],testStrings[12], testStrings[13], testStrings[14], testStrings[15] };
			Datum RTTIDatum = { testRTTIs[0], testRTTIs[1], testRTTIs[2], testRTTIs[3],testRTTIs[4], testRTTIs[5], testRTTIs[6], testRTTIs[7],testRTTIs[8], testRTTIs[9], testRTTIs[10], testRTTIs[11],testRTTIs[12], testRTTIs[13], testRTTIs[14], testRTTIs[15] };
			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue(intDatum.ToString(i) == std::to_string(static_cast<int>(intDatum.Get<int>(i))));
				Assert::IsTrue(floatDatum.ToString(i) == std::to_string((static_cast<float>(floatDatum.Get<float>(i)))));
				Assert::IsTrue(vec4Datum.ToString(i) == glm::to_string(vec4Datum.Get<vec4>(i)));
				Assert::IsTrue(mat4x4Datum.ToString(i) == glm::to_string(mat4x4Datum.Get<mat4x4>(i)));
				Assert::IsTrue(stringDatum.ToString(i) == stringDatum.Get<string>(i));
				Assert::IsTrue(RTTIDatum.ToString(i) == std::to_string(testFoos[i].Data()));
			}

			Datum unknownDatum;

			auto uninitializedTypeException = [&unknownDatum] {unknownDatum.ToString(); };
			Assert::ExpectException<std::exception>(uninitializedTypeException);
		}

		TEST_METHOD(PushBackTest)
		{
			size_t testSize = 16;

			int testInt = rand();
			float testFloat = randFloat();
			vec4 testVec4(randFloat(), randFloat(), randFloat(), randFloat());
			mat4x4 testMat4x4(randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat());
			string testString = random_string(32);
			Foo testFoo(rand());
			RTTI * testRTTI = &testFoo;

			Vector<int> testInts;
			Vector<float> testFloats;
			Vector<vec4> testVec4s;
			Vector<mat4x4> testMat4x4s;
			Vector<string> testStrings;
			Vector<Foo> testFoos;
			Vector<RTTI*> testRTTIs;
			for (size_t i = 0; i < testSize; i++)
			{
				testInts.PushBack(rand());
				testFloats.PushBack(randFloat());
				testVec4s.PushBack(vec4(randFloat(), randFloat(), randFloat(), randFloat()));
				testMat4x4s.PushBack(mat4x4((randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat())));
				testStrings.PushBack(random_string(testSize));
				testFoos.PushBack(Foo(rand()));
			}
			//Must be seperate from other for loop because Foo vector will resize and change the address od it's stored Foos.
			for (size_t i = 0; i < testSize; i++)
			{
				testRTTIs.PushBack(&testFoos[i]);
			}

			Datum intDatum;
			Datum floatDatum;
			Datum vec4Datum;
			Datum mat4x4Datum;
			Datum stringDatum;
			Datum RTTIDatum;

			for (size_t i = 0; i < testSize; i++)
			{
				intDatum.PushBack(testInts[i]);
				Assert::IsTrue(intDatum.Size() == i+1);
				Assert::IsTrue(intDatum.Front<int>() == testInts[0]);
				Assert::IsTrue(intDatum.Back<int>() == testInts[i]);
				floatDatum.PushBack(testFloats[i]);
				Assert::IsTrue(floatDatum.Size() == i+1);
				Assert::IsTrue(floatDatum.Front<float>() == testFloats[0]);
				Assert::IsTrue(floatDatum.Back<float>() == testFloats[i]);
				vec4Datum.PushBack(testVec4s[i]);
				Assert::IsTrue(vec4Datum.Size() == i+1);
				Assert::IsTrue(vec4Datum.Front<vec4>() == testVec4s[0]);
				Assert::IsTrue(vec4Datum.Back<vec4>() == testVec4s[i]);
				mat4x4Datum.PushBack(testMat4x4s[i]);
				Assert::IsTrue(mat4x4Datum.Size() == i+1);
				Assert::IsTrue(mat4x4Datum.Front<mat4x4>() == testMat4x4s[0]);
				Assert::IsTrue(mat4x4Datum.Back<mat4x4>() == testMat4x4s[i]);
				stringDatum.PushBack(testStrings[i]);
				Assert::IsTrue(stringDatum.Size() == i+1);
				Assert::IsTrue(stringDatum.Front<string>() == testStrings[0]);
				Assert::IsTrue(stringDatum.Back<string>() == testStrings[i]);
				RTTIDatum.PushBack(testRTTIs[i]);
				Assert::IsTrue(RTTIDatum.Size() == i+1);
				Assert::IsTrue(RTTIDatum.Front<RTTI*>() == testRTTIs[0]);
				Assert::IsTrue(RTTIDatum.Back<RTTI*>() == testRTTIs[i]);
			}

			auto intIncorrectTypeException = [&RTTIDatum, &testInt] {RTTIDatum.PushBack(testInt); };
			Assert::ExpectException<std::exception>(intIncorrectTypeException);

			auto floatIncorrectTypeException = [&intDatum, &testFloat] {intDatum.PushBack(testFloat); };
			Assert::ExpectException<std::exception>(floatIncorrectTypeException);

			auto vec4IncorrectTypeException = [&floatDatum, &testVec4] {floatDatum.PushBack(testVec4); };
			Assert::ExpectException<std::exception>(vec4IncorrectTypeException);

			auto mat4x4IncorrectTypeException = [&vec4Datum, &testMat4x4] {vec4Datum.PushBack(testMat4x4); };
			Assert::ExpectException<std::exception>(mat4x4IncorrectTypeException);

			auto stringIncorrectTypeException = [&mat4x4Datum, &testString] {mat4x4Datum.PushBack(testString); };
			Assert::ExpectException<std::exception>(stringIncorrectTypeException);

			auto RTTIIncorrectTypeException = [&stringDatum, &testRTTI] {stringDatum.PushBack(testRTTI); };
			Assert::ExpectException<std::exception>(mat4x4IncorrectTypeException);

			int* testIntArr = new int[testSize];

			Datum externalDatum;

			externalDatum.SetStorage(testIntArr, testSize);

			auto externalDatumException = [&externalDatum] {externalDatum.PushBack(1); };
			Assert::ExpectException<std::exception>(externalDatumException);

			delete[] testIntArr;

		}

		TEST_METHOD(PopBackTest)
		{
			size_t testSize = 16;

			Vector<int> testInts;
			Vector<float> testFloats;
			Vector<vec4> testVec4s;
			Vector<mat4x4> testMat4x4s;
			Vector<string> testStrings;
			Vector<Foo> testFoos;
			Vector<RTTI*> testRTTIs;
			for (size_t i = 0; i < testSize; i++)
			{
				testInts.PushBack(rand());
				testFloats.PushBack(randFloat());
				testVec4s.PushBack(vec4(randFloat(), randFloat(), randFloat(), randFloat()));
				testMat4x4s.PushBack(mat4x4((randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat())));
				testStrings.PushBack(random_string(testSize));
				testFoos.PushBack(Foo(rand()));
			}
			//Must be seperate from other for loop because Foo vector will resize and change the address od it's stored Foos.
			for (size_t i = 0; i < testSize; i++)
			{
				testRTTIs.PushBack(&testFoos[i]);
			}

			Datum intDatum = { testInts[0], testInts[1], testInts[2], testInts[3],testInts[4], testInts[5], testInts[6], testInts[7],testInts[8], testInts[9], testInts[10], testInts[11],testInts[12], testInts[13], testInts[14], testInts[15] };
			
			Datum floatDatum = { testFloats[0], testFloats[1], testFloats[2], testFloats[3],testFloats[4], testFloats[5], testFloats[6], testFloats[7],testFloats[8], testFloats[9], testFloats[10], testFloats[11],testFloats[12], testFloats[13], testFloats[14], testFloats[15] };

			Datum vec4Datum = { testVec4s[0], testVec4s[1], testVec4s[2], testVec4s[3],testVec4s[4], testVec4s[5], testVec4s[6], testVec4s[7],testVec4s[8], testVec4s[9], testVec4s[10], testVec4s[11],testVec4s[12], testVec4s[13], testVec4s[14], testVec4s[15] };

			Datum mat4x4Datum = { testMat4x4s[0], testMat4x4s[1], testMat4x4s[2], testMat4x4s[3],testMat4x4s[4], testMat4x4s[5], testMat4x4s[6], testMat4x4s[7],testMat4x4s[8], testMat4x4s[9], testMat4x4s[10], testMat4x4s[11],testMat4x4s[12], testMat4x4s[13], testMat4x4s[14], testMat4x4s[15] };

			Datum stringDatum = { testStrings[0], testStrings[1], testStrings[2], testStrings[3],testStrings[4], testStrings[5], testStrings[6], testStrings[7],testStrings[8], testStrings[9], testStrings[10], testStrings[11],testStrings[12], testStrings[13], testStrings[14], testStrings[15] };

			Datum RTTIDatum = { testRTTIs[0], testRTTIs[1], testRTTIs[2], testRTTIs[3],testRTTIs[4], testRTTIs[5], testRTTIs[6], testRTTIs[7],testRTTIs[8], testRTTIs[9], testRTTIs[10], testRTTIs[11],testRTTIs[12], testRTTIs[13], testRTTIs[14], testRTTIs[15] };

			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue(intDatum.Size() == testSize - i);
				Assert::IsTrue(intDatum.Front<int>() == testInts[0]);
				Assert::IsTrue(intDatum.Back<int>() == testInts[testSize - i - 1]);
				intDatum.PopBack();
				Assert::IsTrue(floatDatum.Size() == testSize - i);
				Assert::IsTrue(floatDatum.Front<float>() == testFloats[0]);
				Assert::IsTrue(floatDatum.Back<float>() == testFloats[testSize - i - 1]);
				floatDatum.PopBack();
				Assert::IsTrue(vec4Datum.Size() == testSize - i);
				Assert::IsTrue(vec4Datum.Front<vec4>() == testVec4s[0]);
				Assert::IsTrue(vec4Datum.Back<vec4>() == testVec4s[testSize - i - 1]);
				vec4Datum.PopBack();
				Assert::IsTrue(mat4x4Datum.Size() == testSize - i);
				Assert::IsTrue(mat4x4Datum.Front<mat4x4>() == testMat4x4s[0]);
				Assert::IsTrue(mat4x4Datum.Back<mat4x4>() == testMat4x4s[testSize - i - 1]);
				mat4x4Datum.PopBack();
				Assert::IsTrue(stringDatum.Size() == testSize - i);
				Assert::IsTrue(stringDatum.Front<string>() == testStrings[0]);
				Assert::IsTrue(stringDatum.Back<string>() == testStrings[testSize - i - 1]);
				stringDatum.PopBack();
				Assert::IsTrue(RTTIDatum.Size() == testSize - i);
				Assert::IsTrue(RTTIDatum.Front<RTTI*>() == testRTTIs[0]);
				Assert::IsTrue(RTTIDatum.Back<RTTI*>() == testRTTIs[testSize - i - 1]);
				RTTIDatum.PopBack();
			}

			Datum unknownDatum;

			auto datumUninitializedException = [&unknownDatum] {unknownDatum.PopBack(); };
			Assert::ExpectException<std::exception>(datumUninitializedException);

			int* testIntArr = new int[testSize];

			Datum externalDatum;

			externalDatum.SetStorage(testIntArr, testSize);

			auto externalDatumException = [&externalDatum] {externalDatum.PopBack(); };
			Assert::ExpectException<std::exception>(externalDatumException);

			delete[] testIntArr;


		}

		TEST_METHOD(RemoveTest)
		{
			size_t testSize = 16;

			Vector<int> testInts;
			Vector<float> testFloats;
			Vector<vec4> testVec4s;
			Vector<mat4x4> testMat4x4s;
			Vector<string> testStrings;
			Vector<Foo> testFoos;
			Vector<RTTI*> testRTTIs;
			for (size_t i = 0; i < testSize; i++)
			{
				testInts.PushBack(rand());
				testFloats.PushBack(randFloat());
				testVec4s.PushBack(vec4(randFloat(), randFloat(), randFloat(), randFloat()));
				testMat4x4s.PushBack(mat4x4((randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat())));
				testStrings.PushBack(random_string(testSize));
				testFoos.PushBack(Foo(rand()));
			}
			//Must be seperate from other for loop because Foo vector will resize and change the address od it's stored Foos.
			for (size_t i = 0; i < testSize; i++)
			{
				testRTTIs.PushBack(&testFoos[i]);
			}

			Datum intDatum = { testInts[0], testInts[1], testInts[2], testInts[3],testInts[4], testInts[5], testInts[6], testInts[7],testInts[8], testInts[9], testInts[10], testInts[11],testInts[12], testInts[13], testInts[14], testInts[15] };

			Datum floatDatum = { testFloats[0], testFloats[1], testFloats[2], testFloats[3],testFloats[4], testFloats[5], testFloats[6], testFloats[7],testFloats[8], testFloats[9], testFloats[10], testFloats[11],testFloats[12], testFloats[13], testFloats[14], testFloats[15] };

			Datum vec4Datum = { testVec4s[0], testVec4s[1], testVec4s[2], testVec4s[3],testVec4s[4], testVec4s[5], testVec4s[6], testVec4s[7],testVec4s[8], testVec4s[9], testVec4s[10], testVec4s[11],testVec4s[12], testVec4s[13], testVec4s[14], testVec4s[15] };

			Datum mat4x4Datum = { testMat4x4s[0], testMat4x4s[1], testMat4x4s[2], testMat4x4s[3],testMat4x4s[4], testMat4x4s[5], testMat4x4s[6], testMat4x4s[7],testMat4x4s[8], testMat4x4s[9], testMat4x4s[10], testMat4x4s[11],testMat4x4s[12], testMat4x4s[13], testMat4x4s[14], testMat4x4s[15] };

			Datum stringDatum = { testStrings[0], testStrings[1], testStrings[2], testStrings[3],testStrings[4], testStrings[5], testStrings[6], testStrings[7],testStrings[8], testStrings[9], testStrings[10], testStrings[11],testStrings[12], testStrings[13], testStrings[14], testStrings[15] };

			Datum RTTIDatum = { testRTTIs[0], testRTTIs[1], testRTTIs[2], testRTTIs[3],testRTTIs[4], testRTTIs[5], testRTTIs[6], testRTTIs[7],testRTTIs[8], testRTTIs[9], testRTTIs[10], testRTTIs[11],testRTTIs[12], testRTTIs[13], testRTTIs[14], testRTTIs[15] };

			Assert::IsTrue(intDatum.Size() == testSize);
			intDatum.Remove(-1);
			Assert::IsTrue(intDatum.Size() == testSize);

			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue(intDatum.Size() == testSize - i);
				Assert::IsTrue(intDatum.Front<int>() == testInts[i]);
				Assert::IsTrue(intDatum.Back<int>() == testInts[testSize-1]);
				intDatum.Remove(testInts[i]);
				Assert::IsTrue(floatDatum.Size() == testSize - i);
				Assert::IsTrue(floatDatum.Front<float>() == testFloats[i]);
				Assert::IsTrue(floatDatum.Back<float>() == testFloats[testSize-1]);
				floatDatum.Remove(testFloats[i]);
				Assert::IsTrue(vec4Datum.Size() == testSize - i);
				Assert::IsTrue(vec4Datum.Front<vec4>() == testVec4s[i]);
				Assert::IsTrue(vec4Datum.Back<vec4>() == testVec4s[testSize - 1]);
				vec4Datum.Remove(testVec4s[i]);
				Assert::IsTrue(mat4x4Datum.Size() == testSize - i);
				Assert::IsTrue(mat4x4Datum.Front<mat4x4>() == testMat4x4s[i]);
				Assert::IsTrue(mat4x4Datum.Back<mat4x4>() == testMat4x4s[testSize - 1]);
				mat4x4Datum.Remove(testMat4x4s[i]);
				Assert::IsTrue(stringDatum.Size() == testSize - i);
				Assert::IsTrue(stringDatum.Front<string>() == testStrings[i]);
				Assert::IsTrue(stringDatum.Back<string>() == testStrings[testSize - 1]);
				stringDatum.Remove(testStrings[i]);
				Assert::IsTrue(RTTIDatum.Size() == testSize - i);
				Assert::IsTrue(RTTIDatum.Front<RTTI*>() == testRTTIs[i]);
				Assert::IsTrue(RTTIDatum.Back<RTTI*>() == testRTTIs[testSize - 1]);
				RTTIDatum.Remove(testRTTIs[i]);
			}

			Datum unknownDatum;

			auto datumUninitializedException = [&unknownDatum] {unknownDatum.Remove(0); };
			Assert::ExpectException<std::exception>(datumUninitializedException);

			int* testIntArr = new int[testSize];

			Datum externalDatum;

			externalDatum.SetStorage(testIntArr, testSize);

			auto externalDatumException = [&externalDatum, &testIntArr] {externalDatum.Remove(testIntArr[0]); };
			Assert::ExpectException<std::exception>(externalDatumException);

			delete[] testIntArr;
		}

		TEST_METHOD(RemoveAtTest)
		{
			size_t testSize = 16;

			Vector<int> testInts;
			Vector<float> testFloats;
			Vector<vec4> testVec4s;
			Vector<mat4x4> testMat4x4s;
			Vector<string> testStrings;
			Vector<Foo> testFoos;
			Vector<RTTI*> testRTTIs;
			for (size_t i = 0; i < testSize; i++)
			{
				testInts.PushBack(rand());
				testFloats.PushBack(randFloat());
				testVec4s.PushBack(vec4(randFloat(), randFloat(), randFloat(), randFloat()));
				testMat4x4s.PushBack(mat4x4((randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat())));
				testStrings.PushBack(random_string(testSize));
				testFoos.PushBack(Foo(rand()));
			}
			//Must be seperate from other for loop because Foo vector will resize and change the address od it's stored Foos.
			for (size_t i = 0; i < testSize; i++)
			{
				testRTTIs.PushBack(&testFoos[i]);
			}

			Datum intDatum = { testInts[0], testInts[1], testInts[2], testInts[3],testInts[4], testInts[5], testInts[6], testInts[7],testInts[8], testInts[9], testInts[10], testInts[11],testInts[12], testInts[13], testInts[14], testInts[15] };

			Datum floatDatum = { testFloats[0], testFloats[1], testFloats[2], testFloats[3],testFloats[4], testFloats[5], testFloats[6], testFloats[7],testFloats[8], testFloats[9], testFloats[10], testFloats[11],testFloats[12], testFloats[13], testFloats[14], testFloats[15] };

			Datum vec4Datum = { testVec4s[0], testVec4s[1], testVec4s[2], testVec4s[3],testVec4s[4], testVec4s[5], testVec4s[6], testVec4s[7],testVec4s[8], testVec4s[9], testVec4s[10], testVec4s[11],testVec4s[12], testVec4s[13], testVec4s[14], testVec4s[15] };

			Datum mat4x4Datum = { testMat4x4s[0], testMat4x4s[1], testMat4x4s[2], testMat4x4s[3],testMat4x4s[4], testMat4x4s[5], testMat4x4s[6], testMat4x4s[7],testMat4x4s[8], testMat4x4s[9], testMat4x4s[10], testMat4x4s[11],testMat4x4s[12], testMat4x4s[13], testMat4x4s[14], testMat4x4s[15] };

			Datum stringDatum = { testStrings[0], testStrings[1], testStrings[2], testStrings[3],testStrings[4], testStrings[5], testStrings[6], testStrings[7],testStrings[8], testStrings[9], testStrings[10], testStrings[11],testStrings[12], testStrings[13], testStrings[14], testStrings[15] };

			Datum RTTIDatum = { testRTTIs[0], testRTTIs[1], testRTTIs[2], testRTTIs[3],testRTTIs[4], testRTTIs[5], testRTTIs[6], testRTTIs[7],testRTTIs[8], testRTTIs[9], testRTTIs[10], testRTTIs[11],testRTTIs[12], testRTTIs[13], testRTTIs[14], testRTTIs[15] };

			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue(intDatum.Size() == testSize - i);
				Assert::IsTrue(intDatum.Front<int>() == testInts[i]);
				Assert::IsTrue(intDatum.Back<int>() == testInts[testSize - 1]);
				intDatum.RemoveAt(0);
				Assert::IsTrue(floatDatum.Size() == testSize - i);
				Assert::IsTrue(floatDatum.Front<float>() == testFloats[i]);
				Assert::IsTrue(floatDatum.Back<float>() == testFloats[testSize - 1]);
				floatDatum.RemoveAt(0);
				Assert::IsTrue(vec4Datum.Size() == testSize - i);
				Assert::IsTrue(vec4Datum.Front<vec4>() == testVec4s[i]);
				Assert::IsTrue(vec4Datum.Back<vec4>() == testVec4s[testSize - 1]);
				vec4Datum.RemoveAt(0);
				Assert::IsTrue(mat4x4Datum.Size() == testSize - i);
				Assert::IsTrue(mat4x4Datum.Front<mat4x4>() == testMat4x4s[i]);
				Assert::IsTrue(mat4x4Datum.Back<mat4x4>() == testMat4x4s[testSize - 1]);
				mat4x4Datum.RemoveAt(0);
				Assert::IsTrue(stringDatum.Size() == testSize - i);
				Assert::IsTrue(stringDatum.Front<string>() == testStrings[i]);
				Assert::IsTrue(stringDatum.Back<string>() == testStrings[testSize - 1]);
				stringDatum.RemoveAt(0);
				Assert::IsTrue(RTTIDatum.Size() == testSize - i);
				Assert::IsTrue(RTTIDatum.Front<RTTI*>() == testRTTIs[i]);
				Assert::IsTrue(RTTIDatum.Back<RTTI*>() == testRTTIs[testSize - 1]);
				RTTIDatum.RemoveAt(0);
			}

			auto outOfBoundsException = [&intDatum] {intDatum.RemoveAt(0); };
			Assert::ExpectException<std::exception>(outOfBoundsException);

			Datum unknownDatum;

			auto datumUninitializedException = [&unknownDatum] {unknownDatum.RemoveAt(0); };
			Assert::ExpectException<std::exception>(datumUninitializedException);

			int* testIntArr = new int[testSize];

			Datum externalDatum;

			externalDatum.SetStorage(testIntArr, testSize);

			auto externalDatumException = [&externalDatum, &testIntArr] {externalDatum.RemoveAt(0); };
			Assert::ExpectException<std::exception>(externalDatumException);

			delete[] testIntArr;
		}

		/*TEST_METHOD(RemoveRangeTest)
		{
			size_t testSize = 16;

			Vector<int> testInts;
			Vector<float> testFloats;
			Vector<vec4> testVec4s;
			Vector<mat4x4> testMat4x4s;
			Vector<string> testStrings;
			Vector<Foo> testFoos;
			Vector<RTTI*> testRTTIs;
			for (size_t i = 0; i < testSize; i++)
			{
				testInts.PushBack(rand());
				testFloats.PushBack(randFloat());
				testVec4s.PushBack(vec4(randFloat(), randFloat(), randFloat(), randFloat()));
				testMat4x4s.PushBack(mat4x4((randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat())));
				testStrings.PushBack(random_string(testSize));
				testFoos.PushBack(Foo(rand()));
			}
			//Must be seperate from other for loop because Foo vector will resize and change the address od it's stored Foos.
			for (size_t i = 0; i < testSize; i++)
			{
				testRTTIs.PushBack(&testFoos[i]);
			}

			Datum intDatum = { testInts[0], testInts[1], testInts[2], testInts[3],testInts[4], testInts[5], testInts[6], testInts[7],testInts[8], testInts[9], testInts[10], testInts[11],testInts[12], testInts[13], testInts[14], testInts[15] };

			Datum floatDatum = { testFloats[0], testFloats[1], testFloats[2], testFloats[3],testFloats[4], testFloats[5], testFloats[6], testFloats[7],testFloats[8], testFloats[9], testFloats[10], testFloats[11],testFloats[12], testFloats[13], testFloats[14], testFloats[15] };

			Datum vec4Datum = { testVec4s[0], testVec4s[1], testVec4s[2], testVec4s[3],testVec4s[4], testVec4s[5], testVec4s[6], testVec4s[7],testVec4s[8], testVec4s[9], testVec4s[10], testVec4s[11],testVec4s[12], testVec4s[13], testVec4s[14], testVec4s[15] };

			Datum mat4x4Datum = { testMat4x4s[0], testMat4x4s[1], testMat4x4s[2], testMat4x4s[3],testMat4x4s[4], testMat4x4s[5], testMat4x4s[6], testMat4x4s[7],testMat4x4s[8], testMat4x4s[9], testMat4x4s[10], testMat4x4s[11],testMat4x4s[12], testMat4x4s[13], testMat4x4s[14], testMat4x4s[15] };

			Datum stringDatum = { testStrings[0], testStrings[1], testStrings[2], testStrings[3],testStrings[4], testStrings[5], testStrings[6], testStrings[7],testStrings[8], testStrings[9], testStrings[10], testStrings[11],testStrings[12], testStrings[13], testStrings[14], testStrings[15] };

			Datum RTTIDatum = { testRTTIs[0], testRTTIs[1], testRTTIs[2], testRTTIs[3],testRTTIs[4], testRTTIs[5], testRTTIs[6], testRTTIs[7],testRTTIs[8], testRTTIs[9], testRTTIs[10], testRTTIs[11],testRTTIs[12], testRTTIs[13], testRTTIs[14], testRTTIs[15] };

			intDatum.RemoveRange(0, testSize/2);
			Assert::IsTrue(intDatum.Size() == testSize/2);
			floatDatum.RemoveRange(0, testSize / 2);
			Assert::IsTrue(floatDatum.Size() == testSize/2);
			vec4Datum.RemoveRange(0, testSize / 2);
			Assert::IsTrue(vec4Datum.Size() == testSize/2);
			mat4x4Datum.RemoveRange(0, testSize / 2);
			Assert::IsTrue(mat4x4Datum.Size() == testSize/2);
			stringDatum.RemoveRange(0, testSize / 2);
			Assert::IsTrue(stringDatum.Size() == testSize/2);
			RTTIDatum.RemoveRange(0, testSize / 2);
			Assert::IsTrue(RTTIDatum.Size() == testSize / 2);

			for (size_t i = testSize/2; i < testSize; i++)
			{
				Assert::IsTrue(intDatum.Get<int>(i - testSize/2) == testInts[i]);
				Assert::IsTrue(floatDatum.Get<float>(i - testSize / 2) == testFloats[i]);
				Assert::IsTrue(vec4Datum.Get<vec4>(i - testSize / 2) == testVec4s[i]);
				Assert::IsTrue(mat4x4Datum.Get<mat4x4>(i - testSize / 2) == testMat4x4s[i]);
				Assert::IsTrue(stringDatum.Get<string>(i - testSize / 2) == testStrings[i]);
				Assert::IsTrue(RTTIDatum.Get<RTTI*>(i - testSize / 2) == testRTTIs[i]);
			}

			auto outOfBoundsException = [&intDatum, &testSize] {intDatum.RemoveRange(0, testSize); };
			Assert::ExpectException<std::exception>(outOfBoundsException);

			Datum unknownDatum;

			auto datumUninitializedException = [&unknownDatum] {unknownDatum.RemoveRange(0, 0); };
			Assert::ExpectException<std::exception>(datumUninitializedException);

			int* testIntArr = new int[testSize];

			Datum externalDatum;

			externalDatum.SetStorage(testIntArr, testSize);

			auto externalDatumException = [&externalDatum, &testIntArr, &testSize] {externalDatum.RemoveRange(0, testSize); };
			Assert::ExpectException<std::exception>(externalDatumException);

			delete[] testIntArr;
		}*/

		TEST_METHOD(FindTest)
		{
			size_t testSize = 16;

			Vector<int> testInts;
			Vector<float> testFloats;
			Vector<vec4> testVec4s;
			Vector<mat4x4> testMat4x4s;
			Vector<string> testStrings;
			Vector<Foo> testFoos;
			Vector<RTTI*> testRTTIs;
			for (size_t i = 0; i < testSize; i++)
			{
				testInts.PushBack(rand());
				testFloats.PushBack(randFloat());
				testVec4s.PushBack(vec4(randFloat(), randFloat(), randFloat(), randFloat()));
				testMat4x4s.PushBack(mat4x4((randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat(), randFloat())));
				testStrings.PushBack(random_string(testSize));
				testFoos.PushBack(Foo(rand()));
			}
			//Must be seperate from other for loop because Foo vector will resize and change the address od it's stored Foos.
			for (size_t i = 0; i < testSize; i++)
			{
				testRTTIs.PushBack(&testFoos[i]);
			}

			Datum intDatum = { testInts[0], testInts[1], testInts[2], testInts[3],testInts[4], testInts[5], testInts[6], testInts[7],testInts[8], testInts[9], testInts[10], testInts[11],testInts[12], testInts[13], testInts[14], testInts[15] };

			Datum floatDatum = { testFloats[0], testFloats[1], testFloats[2], testFloats[3],testFloats[4], testFloats[5], testFloats[6], testFloats[7],testFloats[8], testFloats[9], testFloats[10], testFloats[11],testFloats[12], testFloats[13], testFloats[14], testFloats[15] };

			Datum vec4Datum = { testVec4s[0], testVec4s[1], testVec4s[2], testVec4s[3],testVec4s[4], testVec4s[5], testVec4s[6], testVec4s[7],testVec4s[8], testVec4s[9], testVec4s[10], testVec4s[11],testVec4s[12], testVec4s[13], testVec4s[14], testVec4s[15] };

			Datum mat4x4Datum = { testMat4x4s[0], testMat4x4s[1], testMat4x4s[2], testMat4x4s[3],testMat4x4s[4], testMat4x4s[5], testMat4x4s[6], testMat4x4s[7],testMat4x4s[8], testMat4x4s[9], testMat4x4s[10], testMat4x4s[11],testMat4x4s[12], testMat4x4s[13], testMat4x4s[14], testMat4x4s[15] };

			Datum stringDatum = { testStrings[0], testStrings[1], testStrings[2], testStrings[3],testStrings[4], testStrings[5], testStrings[6], testStrings[7],testStrings[8], testStrings[9], testStrings[10], testStrings[11],testStrings[12], testStrings[13], testStrings[14], testStrings[15] };

			Datum RTTIDatum = { testRTTIs[0], testRTTIs[1], testRTTIs[2], testRTTIs[3],testRTTIs[4], testRTTIs[5], testRTTIs[6], testRTTIs[7],testRTTIs[8], testRTTIs[9], testRTTIs[10], testRTTIs[11],testRTTIs[12], testRTTIs[13], testRTTIs[14], testRTTIs[15] };

			Assert::IsFalse((intDatum.Find(-1).first));
			Assert::IsFalse((floatDatum.Find(-1.0f).first));
			Assert::IsFalse((vec4Datum.Find(vec4(-1.0f, -1.0f, -1.0f, -1.0f)).first));
			Assert::IsFalse((mat4x4Datum.Find(mat4x4(-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f)).first));
			Assert::IsFalse((stringDatum.Find("Hello").first));
			Assert::IsFalse((RTTIDatum.Find(static_cast<RTTI*>(nullptr)).first));

			for (size_t i = 0; i < testSize; i++)
			{
				Assert::IsTrue((intDatum.Find(testInts[i]).first && *(intDatum.Find(testInts[i]).second) == testInts[i]));
				Assert::IsTrue((floatDatum.Find(testFloats[i]).first && *(floatDatum.Find(testFloats[i]).second) == testFloats[i]));
				Assert::IsTrue((vec4Datum.Find(testVec4s[i]).first && *(vec4Datum.Find(testVec4s[i]).second) == testVec4s[i]));
				Assert::IsTrue((mat4x4Datum.Find(testMat4x4s[i]).first && *(mat4x4Datum.Find(testMat4x4s[i]).second) == testMat4x4s[i]));
				Assert::IsTrue((stringDatum.Find(testStrings[i]).first && *(stringDatum.Find(testStrings[i]).second) == testStrings[i]));
				Assert::IsTrue((RTTIDatum.Find(testRTTIs[i]).first && *(RTTIDatum.Find(testRTTIs[i]).second) == testRTTIs[i]));
			}

			auto intTypeMismatchException = [&intDatum] {intDatum.Find(static_cast<RTTI*>(nullptr)); };
			Assert::ExpectException<std::exception>(intTypeMismatchException);

			auto floatTypeMismatchException = [&floatDatum] {floatDatum.Find(-1); };
			Assert::ExpectException<std::exception>(floatTypeMismatchException);

			auto vec4TypeMismatchException = [&vec4Datum] {vec4Datum.Find(-1.0f); };
			Assert::ExpectException<std::exception>(vec4TypeMismatchException);

			auto mat4x4TypeMismatchException = [&mat4x4Datum] {mat4x4Datum.Find(vec4(-1.0f, -1.0f, -1.0f, -1.0f)); };
			Assert::ExpectException<std::exception>(mat4x4TypeMismatchException);

			auto stringTypeMismatchException = [&stringDatum] {stringDatum.Find(mat4x4(-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f)); };
			Assert::ExpectException<std::exception>(stringTypeMismatchException);

			auto RTTITypeMismatchException = [&RTTIDatum] {RTTIDatum.Find("Hello"); };
			Assert::ExpectException<std::exception>(RTTITypeMismatchException);

		}
	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState DatumTest::sStartMemState;
}