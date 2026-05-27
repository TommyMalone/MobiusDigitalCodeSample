/// <summary>
/// The cpp file containing the definition of nontemplated Datum functions declared in Datum.h.
/// </summary>

#include "pch.h"
#include "Datum.h"
#include "RTTI.h"
#pragma warning(push)
#pragma warning(disable: 4201)
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#pragma warning(pop)
#include <algorithm>

using namespace glm;
using namespace std;

namespace FIEAGameEngine
{
	#pragma region TypeSizesScope
	const size_t Datum::mTypeSizes[] =
	{
		0, // Unknown
		sizeof(int),
		sizeof(float),
		sizeof(vec4),
		sizeof(mat4),
		sizeof(Scope*),
		sizeof(string),
		sizeof(RTTI*)
	};
	#pragma endregion



	#pragma region Constructor
	Datum::Datum(Datum const& other)
	{
		*this = other;
	}

	Datum::Datum(int const& other)
	{
		PushBack(other);
	}

	Datum::Datum(float const& other)
	{
		PushBack(other);
	}

	Datum::Datum(vec4 const& other)
	{
		PushBack(other);
	}

	Datum::Datum(mat4x4 const& other)
	{
		PushBack(other);
	}

	FIEAGameEngine::Datum::Datum(Scope const& other)
	{
		PushBack(other);
	}

	Datum::Datum(string const& other)
	{
		PushBack(other);
	}

	Datum::Datum(RTTI * const& other)
	{
		PushBack(other);
	}
	#pragma endregion

	#pragma region operator=
	Datum & Datum::operator=(Datum const& other)
	{
		if (this != &other)
		{
			if (mIsInternal)
			{
				Clear();
				ShrinkToFit();
			}

			mType = other.mType;

			if (other.mIsInternal)
			{
				mIsInternal = true;
				if (mType != DatumType::Unknown)
				{
					Reserve(other.mSize);
				}
				if(mType != DatumType::String)
				{
					memcpy(mArray.tVoid, other.mArray.tVoid, other.mSize * mTypeSizes[static_cast<size_t>(mType)]);
					mSize = other.mSize;
				}
				else
				{
					for (size_t i = 0; i < other.mSize; i++)
					{
						PushBack(other.mArray.tString[i]);
					}
				}
			}
			else
			{
				mIsInternal = false;
				mCapacity = other.mCapacity;
				mSize = other.mSize;
				mArray = other.mArray;
			}
		}
		return *this;
	}

	Datum & Datum::StoragePreservedAssignment(Datum const& other)
	{
		if (this != &other)
		{
			if (mIsInternal)
			{
				Clear();
				Reserve(other.mSize);
				mType = other.mType;
				if (mType != DatumType::String)
				{
					memcpy(mArray.tVoid, other.mArray.tVoid, other.mSize * mTypeSizes[static_cast<size_t>(mType)]);
					mSize = other.mSize;
				}
				else
				{
					for (size_t i = 0; i < other.mSize; i++)
					{
						PushBack(other.mArray.tString[i]);
					}
				}
			}
			else
			{
				if (mType != other.mType)
				{
					throw exception(storagePreservedAssignmentTypeMismatchExceptionText.c_str());
				}
				size_t minSize = std::min(mSize, other.mSize);
				if (mType != DatumType::String)
				{
					memcpy(mArray.tVoid, other.mArray.tVoid, minSize * mTypeSizes[static_cast<size_t>(mType)]);
					mSize = other.mSize;
				}
				else
				{
					for (size_t i = 0; i < minSize; i++)
					{
						Set(other.mArray.tString[i], i);
					}
				}
				
			}
		}
		return *this;
	}

	Datum & Datum::operator=(int const& other)
	{

		if (mType == DatumType::Unknown || mType == DatumType::Integer)
		{
			mType = DatumType::Integer;
		}
		else
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		if (mIsInternal)
		{
			Resize(1);
			ShrinkToFit();
			Set(other);
		}
		else
		{
			mSize = 1;
			mCapacity = 1;
			mArray.tInt[0] = (const_cast<int&>(other));
		}

		return *this;
	}

	Datum & Datum::operator=(float const& other)
	{
		if (mType == DatumType::Unknown || mType == DatumType::Float)
		{
			mType = DatumType::Float;
		}
		else
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		if (mIsInternal)
		{
			Resize(1);
			ShrinkToFit();
			Set(other);
		}
		else
		{
			mSize = 1;
			mCapacity = 1;
			mArray.tFloat[0] = (const_cast<float&>(other));
		}
		return *this;
	}

	Datum & Datum::operator=(vec4 const& other)
	{
		if (mType == DatumType::Unknown || mType == DatumType::Vector4)
		{
			mType = DatumType::Vector4;
		}
		else
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		if (mIsInternal)
		{
			Resize(1);
			ShrinkToFit();
			Set(other);
		}
		else
		{
			mSize = 1;
			mCapacity = 1;
			mArray.tVec4[0] = (const_cast<vec4&>(other));
		}
		return *this;
	}

	Datum & Datum::operator=(mat4x4 const& other)
	{
		if (mType == DatumType::Unknown || mType == DatumType::Matrix4x4)
		{
			mType = DatumType::Matrix4x4;
		}
		else
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		if (mIsInternal)
		{
			Resize(1);
			ShrinkToFit();
			Set(other);
		}
		else
		{
			mSize = 1;
			mCapacity = 1;
			mArray.tMat4x4[0] = (const_cast<mat4x4&>(other));
		}
		return *this;
	}

	Datum & FIEAGameEngine::Datum::operator=(Scope const& other)
	{
		if (mType == DatumType::Unknown || mType == DatumType::Scope)
		{
			mType = DatumType::Scope;
		}
		else
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		if (mIsInternal)
		{
			Resize(1);
			ShrinkToFit();
			Set(other);
		}
		else
		{
			throw exception(noExternalScopesExceptionText.c_str());
		}
		return *this;
	}

	Datum & Datum::operator=(string const& other)
	{
		if (mType == DatumType::Unknown || mType == DatumType::String)
		{
			mType = DatumType::String;
		}
		else
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		if (mIsInternal)
		{
			Resize(1);
			ShrinkToFit();
			Set(other);
		}
		else
		{
			mSize = 1;
			mCapacity = 1;
			mArray.tString[0] = (const_cast<string&>(other));
		}
		return *this;
	}

	Datum & Datum::operator=(RTTI * const& other)
	{
		if (mType == DatumType::Unknown || mType == DatumType::RTTIPtr)
		{
			mType = DatumType::RTTIPtr;
		}
		else
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		if (mIsInternal)
		{
			Resize(1);
			ShrinkToFit();
			Set(other);
		}
		else
		{
			mSize = 1;
			mCapacity = 1;
			mArray.tRTTIPtr[0] = (const_cast<RTTI*&>(other));
		}
		return *this;
	}
	#pragma endregion

	#pragma region InitializerList
	Datum::Datum(initializer_list<int> const& iList)
	{
		SetType(DatumType::Integer);
		for (auto const& i : iList)
		{
			PushBack(i);
		}
	}

	Datum::Datum(initializer_list<float> const& iList)
	{
		SetType(DatumType::Float);
		for (auto const& i : iList)
		{
			PushBack(i);
		}
	}

	Datum::Datum(initializer_list<vec4> const& iList)
	{
		SetType(DatumType::Vector4);
		for (auto const& i : iList)
		{
			PushBack(i);
		}
	}

	Datum::Datum(initializer_list<mat4x4> const& iList)
	{
		SetType(DatumType::Matrix4x4);
		for (auto const& i : iList)
		{
			PushBack(i);
		}
	}

	Datum::Datum(initializer_list<string> const& iList)
	{
		SetType(DatumType::String);
		for (auto const& i : iList)
		{
			PushBack(i);
		}
	}

	Datum::Datum(initializer_list<RTTI*> const& iList)
	{
		SetType(DatumType::RTTIPtr);
		for (auto const& i : iList)
		{
			PushBack(i);
		}
	}
	#pragma endregion

	#pragma region Destructor
	Datum::~Datum()
	{
		if (mIsInternal && mSize > 0)
		{
			Clear();
		}
		ShrinkToFit();
	}
	#pragma endregion

	#pragma region Reserve
	void Datum::Reserve(size_t newCapacity)
	{
		if (!mIsInternal)
		{
			throw exception(cannotMutateExternalStorageExceptionText.c_str());
		}
		if (mType == DatumType::Unknown)
		{
			throw exception(datumTypeNotInitializedExceptionText.c_str());
		}
		if (newCapacity > mCapacity)
		{
			mCapacity = newCapacity;

			DatumPointer newMemoryAdrress;
			newMemoryAdrress.tVoid = realloc(mArray.tVoid, mCapacity * mTypeSizes[static_cast<size_t>(mType)]);
			if (newMemoryAdrress.tVoid)
			{
				mArray.tVoid = newMemoryAdrress.tVoid;
			}
		}
	}
	#pragma endregion

	#pragma region operator==
	bool Datum::operator==(Datum const& other) const
	{
		bool result = false;
		if (mSize == other.mSize && mIsInternal == other.mIsInternal && mType == other.mType)
		{
			if (mType == DatumType::Scope || mType == DatumType::RTTIPtr)
			{
				result = true;
				for (size_t i = 0; i < mSize; i++)
				{
					if (!(mArray.tRTTIPtr[i]->Equals(other.mArray.tRTTIPtr[i])))
					{
						result = false;
						break;
					}
				}
			}
			else if (mType == DatumType::String)
			{
				result = true;
				for (size_t i = 0; i < mSize; i++)
				{
					if (mArray.tString[i] != other.mArray.tString[i])
					{
						result = false;
						break;
					}
				}
			}
			else
			{
				result = memcmp(mArray.tVoid, other.mArray.tVoid, mSize * mTypeSizes[static_cast<size_t>(mType)]) == 0;
			}
		
		
		}
		return result;
	}

	bool Datum::operator==(int const& other) const
	{
		if (DatumType::Integer != mType)
		{
			return false;
		}
		return mSize == 1 && mArray.tInt[0] == other;
	}

	bool Datum::operator==(float const& other) const
	{
		if (DatumType::Float != mType)
		{
			return false;
		}
		return mSize == 1 && mArray.tFloat[0] == other;
	}

	bool Datum::operator==(vec4 const& other) const
	{
		if (DatumType::Vector4 != mType)
		{
			return false;
		}
		return mSize == 1 && mArray.tVec4[0] == other;
	}

	bool Datum::operator==(mat4x4 const& other) const
	{
		if (DatumType::Matrix4x4 != mType)
		{
			return false;
		}
		return mSize == 1 && mArray.tMat4x4[0] == other;
	}

	bool Datum::operator==(string const& other) const
	{
		if (DatumType::String != mType)
		{
			return false;
		}
		return mSize == 1 && mArray.tString[0] == other;
	}

	bool Datum::operator==(RTTI * const& other) const
	{
		bool result = false;
		if (DatumType::RTTIPtr != mType && DatumType::Scope != mType)
		{
			return false;
		}
		if (mArray.tRTTIPtr[0] != nullptr)
		{
			result = mSize == 1 && mArray.tRTTIPtr[0]->Equals(const_cast<RTTI*>(other));
		}
		else if (other == nullptr)
		{
			result = true;
		}
		return result;
	}
	#pragma endregion

	#pragma region operator!=
	bool Datum::operator!=(Datum const& other) const
	{
		return !(operator==(other));
	}

	bool Datum::operator!=(int const& other) const
	{
		return !(operator==(other));
	}

	bool Datum::operator!=(float const& other) const
	{
		return !(operator==(other));
	}

	bool Datum::operator!=(vec4 const& other) const
	{
		return !(operator==(other));
	}

	bool Datum::operator!=(mat4x4 const& other) const
	{
		return !(operator==(other));
	}

	bool Datum::operator!=(string const& other) const
	{
		return !(operator==(other));
	}

	bool Datum::operator!=(RTTI * const& other) const
	{
		return !(operator==(other));
	}
	#pragma endregion

	#pragma region operator[]
	Scope & FIEAGameEngine::Datum::operator[](size_t index)
	{
		if (mType != DatumType::Scope)
		{
			throw exception(bracketOperatorOnlyValidForScopesExceptionText.c_str());
		}
		return *Get<Scope*>(index);
	}
	#pragma endregion

	#pragma region Type
	Datum::DatumType Datum::Type() const
	{
		return mType;
	}
	#pragma endregion

	#pragma region SetType
	Datum::DatumType Datum::SetType(DatumType type)
	{
		if (mType == DatumType::Unknown || mType == type)
		{
			mType = type;
		}
		else
		{
			throw exception(datumTypeAlreadyInitializedExceptionText.c_str());
		}
		return mType;
	}
	#pragma endregion

	#pragma region Size
	size_t Datum::Size() const
	{
		return mSize;
	}
	#pragma endregion

	#pragma region Capacity
	size_t FIEAGameEngine::Datum::Capacity() const
	{
		return mCapacity;
	}
	#pragma endregion

	#pragma region Resize
	void Datum::Resize(size_t newSize)
	{
		//External memory check done in templated function.
		switch (mType)
		{
		case DatumType::Integer:
			ResizeTemplated<int>(mArray.tInt, newSize);
			break;
		case DatumType::Float:
			ResizeTemplated<float>(mArray.tFloat, newSize);
			break;
		case DatumType::Vector4:
			ResizeTemplated<vec4>(mArray.tVec4, newSize);
			break;
		case DatumType::Matrix4x4:
			ResizeTemplated<mat4x4>(mArray.tMat4x4, newSize);
			break;
		case DatumType::Scope:
			ResizeTemplated<Scope*>(mArray.tScope, newSize);
			break;
		case DatumType::String:
			ResizeTemplated<string>(mArray.tString, newSize);
			break;
		case DatumType::RTTIPtr:
			ResizeTemplated<RTTI*>(mArray.tRTTIPtr, newSize);
			break;
		default:
			throw exception(datumTypeNotInitializedExceptionText.c_str());
			break;
		}
	}
	#pragma endregion

	#pragma region ShrinkToFit
	void FIEAGameEngine::Datum::ShrinkToFit()
	{
		if (mIsInternal)
		{
			if (mSize != mCapacity)
			{
				if (mSize == 0)
				{
					free(mArray.tVoid);
					mArray.tVoid = nullptr;
					mCapacity = 0;
				}
				else
				{

					mCapacity = mSize;
					DatumPointer newMemoryAdrress;
					newMemoryAdrress.tVoid = realloc(mArray.tVoid, mCapacity * mTypeSizes[static_cast<size_t>(mType)]);
					if (newMemoryAdrress.tVoid)
					{
						mArray.tVoid = newMemoryAdrress.tVoid;
					}
				}
			}
		}
	}
	#pragma endregion

	#pragma region Clear
	void Datum::Clear()
	{
		//External memory check done in templated function.
		switch (mType)
		{
		case DatumType::Integer:
			ClearTemplated<int>(mArray.tInt);
			break;
		case DatumType::Float:
			ClearTemplated<float>(mArray.tFloat);
			break;
		case DatumType::Vector4:
			ClearTemplated<vec4>(mArray.tVec4);
			break;
		case DatumType::Matrix4x4:
			ClearTemplated<mat4x4>(mArray.tMat4x4);
			break;
		case DatumType::Scope:
			ClearTemplated<Scope*>(mArray.tScope);
			break;
		case DatumType::String:
			ClearTemplated<string>(mArray.tString);
			break;
		case DatumType::RTTIPtr:
			ClearTemplated<RTTI*>(mArray.tRTTIPtr);
			break;
		default:
			break;
		}
	}
	#pragma endregion

	#pragma region SetStorage
	void Datum::SetStorage(int * const& data, size_t const& size)
	{
		if (data == nullptr || size <= 0)
		{
			throw exception(cannotSetStorageInvalidParamExceptionText.c_str());
		}
		if (mType == DatumType::Unknown)
		{
			mType = DatumType::Integer;
		}
		if (mType != DatumType::Integer)
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		if (mIsInternal)
		{
			Clear();
			ShrinkToFit();
		}
		mIsInternal = false;
		mArray.tInt = data;
		mSize = mCapacity = size;
	}

	void Datum::SetStorage(float * const& data, size_t const& size)
	{
		if (data == nullptr || size <= 0)
		{
			throw exception(cannotSetStorageInvalidParamExceptionText.c_str());
		}
		if (mType == DatumType::Unknown)
		{
			mType = DatumType::Float;
		}
		if (mType != DatumType::Float)
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		if (mIsInternal)
		{
			Clear();
			free(mArray.tVoid);
		}
		mIsInternal = false;
		mArray.tFloat = data;
		mSize = mCapacity = size;
	}

	void Datum::SetStorage(vec4 * const& data, size_t const& size)
	{
		if (data == nullptr || size <= 0)
		{
			throw exception(cannotSetStorageInvalidParamExceptionText.c_str());
		}
		if (mType == DatumType::Unknown)
		{
			mType = DatumType::Vector4;
		}
		if (mType != DatumType::Vector4)
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		if (mIsInternal)
		{
			Clear();
			free(mArray.tVoid);
		}
		mIsInternal = false;
		mArray.tVec4 = data;
		mSize = mCapacity = size;
	}

	void Datum::SetStorage(mat4x4 * const& data, size_t const& size)
	{
		if (data == nullptr || size <= 0)
		{
			throw exception(cannotSetStorageInvalidParamExceptionText.c_str());
		}
		if (mType == DatumType::Unknown)
		{
			mType = DatumType::Matrix4x4;
		}
		if (mType != DatumType::Matrix4x4)
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		if (mIsInternal)
		{
			Clear();
			free(mArray.tVoid);
		}
		mIsInternal = false;
		mArray.tMat4x4 = data;
		mSize = mCapacity = size;
	}

	void Datum::SetStorage(string * const& data, size_t const& size)
	{
		if (data == nullptr || size <= 0)
		{
			throw exception(cannotSetStorageInvalidParamExceptionText.c_str());
		}
		if (mType == DatumType::Unknown)
		{
			mType = DatumType::String;
		}
		if (mType != DatumType::String)
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		if (mIsInternal)
		{
			Clear();
			free(mArray.tVoid);
		}
		mIsInternal = false;
		mArray.tString = data;
		mSize = mCapacity = size;
	}

	void Datum::SetStorage(RTTI ** const& data, size_t const& size)
	{
		if (data == nullptr || size <= 0)
		{
			throw exception(cannotSetStorageInvalidParamExceptionText.c_str());
		}
		if (mType == DatumType::Unknown)
		{
			mType = DatumType::RTTIPtr;
		}
		if (mType != DatumType::RTTIPtr)
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		if (mIsInternal)
		{
			Clear();
			free(mArray.tVoid);
		}
		mIsInternal = false;
		mArray.tRTTIPtr = data;
		mSize = mCapacity = size;
	}
	#pragma endregion

	#pragma region IsInternal
	bool FIEAGameEngine::Datum::IsInternal() const
	{
		return mIsInternal;
	}
	#pragma endregion

	#pragma region Set
	void Datum::Set(int const& value, size_t const& index)
	{
		if (DatumType::Integer != mType)
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		mArray.tInt[index] = value;
	}

	void Datum::Set(float const& value, size_t const& index)
	{
		if (DatumType::Float != mType)
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		mArray.tFloat[index] = value;
	}

	void Datum::Set(vec4 const& value, size_t const& index)
	{
		if (DatumType::Vector4 != mType)
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		mArray.tVec4[index] = value;
	}

	void Datum::Set(mat4x4 const& value, size_t const& index)
	{
		if (DatumType::Matrix4x4 != mType)
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		mArray.tMat4x4[index] = value;
	}

	void FIEAGameEngine::Datum::Set(Scope const& value, size_t const& index)
	{
		if (DatumType::Scope != mType)
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		mArray.tScope[index] = const_cast<Scope*>(&value);
	}

	void Datum::Set(string const& value, size_t const& index)
	{
		if (DatumType::String != mType)
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		//mArray.tString[index].~basic_string();
		mArray.tString[index] = value;
	}

	void Datum::Set(RTTI * const& value, size_t const& index)
	{
		if (DatumType::RTTIPtr != mType)
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		mArray.tRTTIPtr[index] = value;
	}
	#pragma endregion

	#pragma region SetFromString
	void Datum::SetFromString(string const& value, size_t const& index)
	{
		switch (mType)
		{
		case DatumType::Integer:
			SetFromStringTemplated<int>(value, index);
			break;
		case DatumType::Float:
			SetFromStringTemplated<float>(value, index);
			break;
		case DatumType::Vector4:
			SetFromStringTemplated<vec4>(value, index);
			break;
		case DatumType::Matrix4x4:
			SetFromStringTemplated<mat4x4>(value, index);
			break;
		case DatumType::Scope:
			//TODO: Future implementation
			break;
		case DatumType::String:
			SetFromStringTemplated<string>(value, index);
			break;
		case DatumType::RTTIPtr:
			SetFromStringTemplated<RTTI*>(value, index);
			break;
		default:
			throw exception(datumTypeNotInitializedExceptionText.c_str());
			break;
		}
	}
	#pragma endregion

#pragma region PushBackFromString
	void Datum::PushBackFromString(string const& value)
	{
		switch (mType)
		{
		case DatumType::Integer:
			PushBackFromStringTemplated<int>(value);
			break;
		case DatumType::Float:
			PushBackFromStringTemplated<float>(value);
			break;
		case DatumType::Vector4:
			PushBackFromStringTemplated<vec4>(value);
			break;
		case DatumType::Matrix4x4:
			PushBackFromStringTemplated<mat4x4>(value);
			break;
		case DatumType::Scope:
			//TODO: Future implementation
			break;
		case DatumType::String:
			PushBackFromStringTemplated<string>(value);
			break;
		case DatumType::RTTIPtr:
			PushBackFromStringTemplated<RTTI*>(value);
			break;
		default:
			throw exception(datumTypeNotInitializedExceptionText.c_str());
			break;
		}
	}
#pragma endregion

	#pragma region ToString
	string Datum::ToString(size_t const& index) const
	{
		string result = "";
		switch (mType)
		{
		case DatumType::Integer:
			result = ToStringTemplated<int>(mArray.tInt, index);
			break;
		case DatumType::Float:
			result = ToStringTemplated<float>(mArray.tFloat, index);
			break;
		case DatumType::Vector4:
			result = ToStringTemplated<vec4>(mArray.tVec4, index);
			break;
		case DatumType::Matrix4x4:
			result = ToStringTemplated<mat4x4>(mArray.tMat4x4, index);
			break;
		case DatumType::Scope:
			result = ToStringTemplated<Scope*>(mArray.tScope, index);
			break;
		case DatumType::String:
			result = ToStringTemplated<string>(mArray.tString, index);
			break;
		case DatumType::RTTIPtr:
			result = ToStringTemplated<RTTI*>(mArray.tRTTIPtr, index);
			break;
		default:
			throw exception(datumTypeNotInitializedExceptionText.c_str());
			break;
		}
		return result;
	}
	#pragma endregion

	#pragma region PushBack
	int & Datum::PushBack(int const& value)
	{
		//External memory check done in templated function.
		if (mType == DatumType::Unknown)
		{
			SetType(DatumType::Integer);
		}
		if (mType == DatumType::Integer)
		{
			return PushBackTemplated<int>(mArray.tInt, value);
		}
		else
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
	}

	float & Datum::PushBack(float const& value)
	{
		//External memory check done in templated function.
		if (mType == DatumType::Unknown)
		{
			SetType(DatumType::Float);
		}
		if (mType == DatumType::Float)
		{
			return PushBackTemplated<float>(mArray.tFloat, value);
		}
		else
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
	}

	vec4 & Datum::PushBack(vec4 const& value)
	{
		//External memory check done in templated function.
		if (mType == DatumType::Unknown)
		{
			SetType(DatumType::Vector4);
		}
		if (mType == DatumType::Vector4)
		{
			return PushBackTemplated<vec4>(mArray.tVec4, value);
		}
		else
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
	}

	mat4x4 & Datum::PushBack(mat4x4 const& value)
	{
		//External memory check done in templated function.
		if (mType == DatumType::Unknown)
		{
			SetType(DatumType::Matrix4x4);
		}
		if (mType == DatumType::Matrix4x4)
		{
			return PushBackTemplated<mat4x4>(mArray.tMat4x4, value);
		}
		else
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
	}

	Scope * & FIEAGameEngine::Datum::PushBack(Scope const& value)
	{
		//External memory check done in templated function.
		if (mType == DatumType::Unknown)
		{
			SetType(DatumType::Scope);
		}
		if (mType == DatumType::Scope)
		{
			return PushBackTemplated<Scope*>(mArray.tScope,const_cast<Scope*>(&value));
		}
		else
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
	}

	string & Datum::PushBack(string const& value)
	{
		//External memory check done in templated function.
		if (mType == DatumType::Unknown)
		{
			SetType(DatumType::String);
		}
		if (mType == DatumType::String)
		{
			return PushBackTemplated<string>(mArray.tString, value);
		}
		else
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
	}


	RTTI * & Datum::PushBack(RTTI * const& value)
	{
		//External memory check done in templated function.
		if (mType == DatumType::Unknown)
		{
			SetType(DatumType::RTTIPtr);
		}
		if (mType == DatumType::RTTIPtr)
		{
			return PushBackTemplated<RTTI*>(mArray.tRTTIPtr, value);
		}
		else
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
	}
	#pragma endregion

	#pragma region PopBack
	void Datum::PopBack()
	{
		//External memory check done in templated function.
		switch (mType)
		{
		case DatumType::Integer:
			PopBackTemplated<int>(mArray.tInt);
			break;
		case DatumType::Float:
			PopBackTemplated<float>(mArray.tFloat);
			break;
		case DatumType::Vector4:
			PopBackTemplated<vec4>(mArray.tVec4);
			break;
		case DatumType::Matrix4x4:
			PopBackTemplated<mat4x4>(mArray.tMat4x4);
			break;
		case DatumType::Scope:
			PopBackTemplated<Scope*>(mArray.tScope);
			break;
		case DatumType::String:
			PopBackTemplated<string>(mArray.tString);
			break;
		case DatumType::RTTIPtr:
			PopBackTemplated<RTTI*>(mArray.tRTTIPtr);
			break;
		default:
			throw exception(datumTypeNotInitializedExceptionText.c_str());
			break;
		}
	}
	#pragma endregion

	#pragma region Remove
	void Datum::Remove(int const& value)
	{
		//External memory check done in templated function.
		RemoveTemplated(mArray.tInt, value);
	}


	void Datum::Remove(float const& value)
	{
		//External memory check done in templated function.
		RemoveTemplated(mArray.tFloat, value);
	}

	void Datum::Remove(vec4 const& value)
	{
		//External memory check done in templated function.
		RemoveTemplated(mArray.tVec4, value);
	}

	void Datum::Remove(mat4x4 const& value)
	{
		//External memory check done in templated function.
		RemoveTemplated(mArray.tMat4x4, value);
	}

	void Datum::Remove(string const& value)
	{
		//External memory check done in templated function.
		RemoveTemplated(mArray.tString, value);
	}

	void Datum::Remove(RTTI* const& value)
	{
		//External memory check done in templated function.
		RemoveTemplated(mArray.tRTTIPtr, value);
	}
	#pragma endregion

	#pragma region RemoveAt
	void Datum::RemoveAt(size_t const& index)
	{
		//External memory check done in templated function.
		switch (mType)
		{
		case DatumType::Integer:
			RemoveAtTemplated<int>(mArray.tInt, index);
			break;
		case DatumType::Float:
			RemoveAtTemplated<float>(mArray.tFloat, index);
			break;
		case DatumType::Vector4:
			RemoveAtTemplated<vec4>(mArray.tVec4, index);
			break;
		case DatumType::Matrix4x4:
			RemoveAtTemplated<mat4x4>(mArray.tMat4x4, index);
			break;
		case DatumType::Scope:
			RemoveAtTemplated<Scope*>(mArray.tScope, index);
			break;
		case DatumType::String:
			RemoveAtTemplated<string>(mArray.tString, index);
			break;
		case DatumType::RTTIPtr:
			RemoveAtTemplated<RTTI*>(mArray.tRTTIPtr, index);
			break;
		default:
			throw exception(datumTypeNotInitializedExceptionText.c_str());
			break;
		}
	}
	#pragma endregion

	#pragma region RemoveRange
	void Datum::RemoveRange(size_t const& start, size_t const& finish)
	{
		//External memory check done in templated function.
		switch (mType)
		{
		case DatumType::Integer:
			RemoveRangeTemplated<int>(mArray.tInt, start, finish);
			break;
		case DatumType::Float:
			RemoveRangeTemplated<float>(mArray.tFloat, start, finish);
			break;
		case DatumType::Vector4:
			RemoveRangeTemplated<vec4>(mArray.tVec4, start, finish);
			break;
		case DatumType::Matrix4x4:
			RemoveRangeTemplated<mat4x4>(mArray.tMat4x4, start, finish);
			break;
		case DatumType::Scope:
			RemoveRangeTemplated<Scope*>(mArray.tScope, start, finish);
			break;
		case DatumType::String:
			RemoveRangeTemplated<string>(mArray.tString, start, finish);
			break;
		case DatumType::RTTIPtr:
			RemoveRangeTemplated<RTTI*>(mArray.tRTTIPtr, start, finish);
			break;
		default:
			throw exception(datumTypeNotInitializedExceptionText.c_str());
			break;
		}
	}
	#pragma endregion

	#pragma region Find
	std::pair<bool, int*>  Datum::Find(int const& searchedValue) const
	{
		if (DatumType::Integer != mType)
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		for (size_t i = 0; i < mSize; i++)
		{
			if (mArray.tInt[i] == searchedValue)
			{
				return std::pair<bool, int*>(true, &(mArray.tInt[i]));
			}
		}
		return std::pair<bool, int*>(false, nullptr);
	}

	std::pair<bool, float*> Datum::Find(float const& searchedValue) const
	{
		if (DatumType::Float != mType)
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		for (size_t i = 0; i < mSize; i++)
		{
			if (mArray.tFloat[i] == searchedValue)
			{
				return std::pair<bool, float*>(true, &(mArray.tFloat[i]));
			}
		}
		return std::pair<bool, float*>(false, nullptr);
	}

	std::pair<bool, vec4*> Datum::Find(vec4 const& searchedValue) const
	{
		if (DatumType::Vector4 != mType)
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		for (size_t i = 0; i < mSize; i++)
		{
			if (mArray.tVec4[i] == searchedValue)
			{
				return std::pair<bool, vec4*>(true, &(mArray.tVec4[i]));
			}
		}
		return std::pair<bool, vec4*>(false, nullptr);
	}

	std::pair<bool, mat4x4*> Datum::Find(mat4x4 const& searchedValue) const
	{
		if (DatumType::Matrix4x4 != mType)
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		for (size_t i = 0; i < mSize; i++)
		{
			if (mArray.tMat4x4[i] == searchedValue)
			{
				return std::pair<bool, mat4x4*>(true, &(mArray.tMat4x4[i]));
			}
		}
		return std::pair<bool, mat4x4*>(false, nullptr);
	}

	std::pair<bool, Scope**> FIEAGameEngine::Datum::Find(Scope const& searchedValue) const
	{
		if (DatumType::Scope != mType)
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		for (size_t i = 0; i < mSize; i++)
		{
			if (mArray.tScope[i] == &searchedValue)
			{
				return std::pair<bool, Scope**>(true, &(mArray.tScope[i]));
			}
		}
		return std::pair<bool, Scope**>(false, nullptr);
	}

	std::pair<bool, string*> Datum::Find(string const& searchedValue) const
	{
		if (DatumType::String != mType)
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		for (size_t i = 0; i < mSize; i++)
		{
			if (mArray.tString[i] == searchedValue)
			{
				return std::pair<bool, string*>(true, &(mArray.tString[i]));
			}
		}
		return std::pair<bool, string*>(false, nullptr);
	}

	std::pair<bool, RTTI**> Datum::Find(RTTI * const& searchedValue) const
	{
		if (DatumType::RTTIPtr != mType)
		{
			throw exception(operationTypeMismatchExceptionText.c_str());
		}
		for (size_t i = 0; i < mSize; i++)
		{
			if (mArray.tRTTIPtr[i] == searchedValue)
			{
				return std::pair<bool, RTTI**>(true, &(mArray.tRTTIPtr[i]));
			}
		}
		return std::pair<bool, RTTI**>(false, nullptr);
	}
	void Datum::SetStorage(void * const& data, size_t const& size)
	{
		if (data == nullptr || size <= 0)
		{
			throw exception(cannotSetStorageInvalidParamExceptionText.c_str());
		}
		if (mType == DatumType::Scope)
		{
			throw exception(noExternalScopesExceptionText.c_str());
		}
		if (mIsInternal)
		{
			Clear();
			ShrinkToFit();
		}
		mIsInternal = false;
		mArray.tVoid = data;
		mSize = mCapacity = size;
	}
	#pragma endregion

	#pragma region ResizeTemplated
	template<typename T>
	inline void Datum::ResizeTemplated(T * const& templatedPtr, size_t const& newSize)
	{
		if (mIsInternal)
		{
			if (newSize > mCapacity)
			{
				Reserve(newSize);
				while (newSize > mSize)
				{
					PushBackTemplated<T>(templatedPtr, T());
				}
			}
			else if (newSize > mSize)
			{
				while (newSize > mSize)
				{
					PushBackTemplated<T>(templatedPtr, T());
					/*
					TODO:
					This is if a function table of default functions is ever implemented.
					auto func = CreateDefaultFunctions[static_cast<int>(mType)];
					assert(func != nullptr);
					(this->*func)(i)
					*/
				}
			}
			else if (newSize < mSize)
			{
				RemoveRange(newSize, mSize);
			}
		}
		else
		{
			throw exception(cannotMutateExternalStorageExceptionText.c_str());
		}
	}
	#pragma endregion

	#pragma region Clear
	template<typename T>
	inline void Datum::ClearTemplated(T * const& templatedPtr)
	{
		if (mIsInternal)
		{
			for (size_t i = 0; i < mSize; i++)
			{
				templatedPtr[i].~T();
			}
			mSize = 0;
		}
		else
		{
			throw exception(cannotMutateExternalStorageExceptionText.c_str());
		}
	}
	#pragma endregion

	#pragma region SetFromStringTemplated
	template<typename T>
	void Datum::SetFromStringTemplated(string const&, size_t const&)
	{
		throw exception(unsupportedDataTypeExceptionText.c_str());
	}

	template<>
	void Datum::SetFromStringTemplated<int>(string const& value, size_t const& index)
	{
		Set(stoi(value), index);
	}

	template<>
	void Datum::SetFromStringTemplated<float>(string const& value, size_t const& index)
	{
		Set(stof(value), index);
	}

	template<>
	void Datum::SetFromStringTemplated<vec4>(string const& value, size_t const& index)
	{
		vec4 readInVec;
		sscanf_s(value.c_str(), "vec4(%f, %f, %f, %f)", &readInVec[0], &readInVec[1], &readInVec[2], &readInVec[3]);
		Set(readInVec, index);
	}

	template<>
	void Datum::SetFromStringTemplated<mat4x4>(string const& value, size_t const& index)
	{
		mat4x4 readInMat;
		sscanf_s(value.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
			&readInMat[0][0], &readInMat[0][1], &readInMat[0][2], &readInMat[0][3],
			&readInMat[1][0], &readInMat[1][1], &readInMat[1][2], &readInMat[1][3],
			&readInMat[2][0], &readInMat[2][1], &readInMat[2][2], &readInMat[2][3],
			&readInMat[3][0], &readInMat[3][1], &readInMat[3][2], &readInMat[3][3]);
		Set(readInMat, index);
	}

	template<>
	void Datum::SetFromStringTemplated<string>(string const& value, size_t const& index)
	{
		Set(value, index);
	}

	template<>
	void Datum::SetFromStringTemplated<RTTI*>(string const&, size_t const&)
	{
		throw exception(unsupportedDataTypeExceptionText.c_str());
	}
	#pragma endregion

#pragma region PushBackFromStringTemplated
	template<typename T>
	void Datum::PushBackFromStringTemplated(string const&)
	{
		throw exception(unsupportedDataTypeExceptionText.c_str());
	}

	template<>
	void Datum::PushBackFromStringTemplated<int>(string const& value)
	{
		PushBack(stoi(value));
	}

	template<>
	void Datum::PushBackFromStringTemplated<float>(string const& value)
	{
		PushBack(stof(value));
	}

	template<>
	void Datum::PushBackFromStringTemplated<vec4>(string const& value)
	{
		vec4 readInVec;
		sscanf_s(value.c_str(), "vec4(%f, %f, %f, %f)", &readInVec[0], &readInVec[1], &readInVec[2], &readInVec[3]);
		PushBack(readInVec);
	}

	template<>
	void Datum::PushBackFromStringTemplated<mat4x4>(string const& value)
	{
		mat4x4 readInMat;
		sscanf_s(value.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
			&readInMat[0][0], &readInMat[0][1], &readInMat[0][2], &readInMat[0][3],
			&readInMat[1][0], &readInMat[1][1], &readInMat[1][2], &readInMat[1][3],
			&readInMat[2][0], &readInMat[2][1], &readInMat[2][2], &readInMat[2][3],
			&readInMat[3][0], &readInMat[3][1], &readInMat[3][2], &readInMat[3][3]);
		PushBack(readInMat);
	}

	template<>
	void Datum::PushBackFromStringTemplated<string>(string const& value)
	{
		PushBack(value);
	}

	template<>
	void Datum::PushBackFromStringTemplated<RTTI*>(string const&)
	{
		throw exception(unsupportedDataTypeExceptionText.c_str());
	}
#pragma endregion

	#pragma region ToStringTemplated
	template<typename T>
	string Datum::ToStringTemplated(T * const&, size_t const&) const
	{
		throw exception(unsupportedDataTypeExceptionText.c_str());
	}

	template<>
	string Datum::ToStringTemplated(int * const& templatedPtr, size_t const& index) const
	{
		return std::to_string(templatedPtr[index]);
	}

	template<>
	string Datum::ToStringTemplated(float * const& templatedPtr, size_t const& index) const
	{
		return std::to_string(templatedPtr[index]);
	}

	template<>
	string Datum::ToStringTemplated(vec4 * const& templatedPtr, size_t const& index) const
	{
		return glm::to_string(templatedPtr[index]);
	}

	template<>
	string Datum::ToStringTemplated(mat4x4 * const& templatedPtr, size_t const& index) const
	{
		return glm::to_string(templatedPtr[index]);
	}

	template<>
	string Datum::ToStringTemplated(string * const& templatedPtr, size_t const& index) const
	{
		return templatedPtr[index];
	}

	template<>
	string Datum::ToStringTemplated(RTTI ** const& templatedPtr, size_t const& index) const
	{
		return (templatedPtr[index] != nullptr ? templatedPtr[index]->ToString() : "nullptr");
	}
	#pragma endregion

	#pragma region PushBackTemplated
	template<typename T>
	inline T & Datum::PushBackTemplated(T * const& templatedPtr, T const& value)
	{
		if (mIsInternal)
		{
			if (mSize == mCapacity)
			{
				if (mCapacity == 0)
				{
					mCapacity = 1;
				}
				Reserve(mCapacity * 2);
			}

			return *(new (templatedPtr + (mSize++)) T(value));
		}
		else
		{
			throw exception(cannotMutateExternalStorageExceptionText.c_str());
		}
	}
	#pragma endregion

	#pragma region PopBack
	template<typename T>
	inline void Datum::PopBackTemplated(T * const& templatedPtr)
	{
		if (mIsInternal)
		{
			if (mSize != 0)
			{
				templatedPtr[--mSize].~T();
			}
		}
		else
		{
			throw exception(cannotMutateExternalStorageExceptionText.c_str());
		}
	}
	#pragma endregion

	#pragma region RemoveTemplated
	template<typename T>
	void Datum::RemoveTemplated(T * const& templatedPtr, T const& value)
	{
		if (mType != DatumType::Unknown)
		{
			if (mIsInternal)
			{
				for (size_t i = 0; i < mSize; i++)
				{
					if (templatedPtr[i] == value)
					{
						templatedPtr[i].~T();
						memmove(&(templatedPtr[i]), &(templatedPtr[i + 1]), (mSize - 1 - i) * sizeof(T));
						mSize--;
						break;
					}
				}
			}
			else
			{
				throw exception(cannotMutateExternalStorageExceptionText.c_str());
			}
		}
		else
		{
			throw exception(datumTypeNotInitializedExceptionText.c_str());
		}
	}
	#pragma endregion

	#pragma region RemoveAtTemplated
	template<typename T>
	void Datum::RemoveAtTemplated(T * const& templatedPtr, size_t const& index)
	{
		if (mIsInternal)
		{
			if (index < mSize)
			{
				templatedPtr[index].~T();
				memmove(&(templatedPtr[index]), &(templatedPtr[index + 1]), (mSize - 1 - index) * sizeof(T));
				mSize--;
			}
			else
			{
				throw exception(indexOutOfBoundsExceptionText.c_str());
			}
		}
		else
		{
			throw exception(cannotMutateExternalStorageExceptionText.c_str());
		}
	}
	#pragma endregion

	#pragma region RemoveRangeTemplated
	template<typename T>
	void Datum::RemoveRangeTemplated(T * const& templatedPtr, size_t const& start, size_t const& finish)
	{
		if (start <= finish)
		{
			if (mIsInternal)
			{
				if (start < mSize && finish <= mSize)
				{
					if (mSize > 0 && start < finish)
					{
						for (size_t i = start; i < finish; i++)
						{
							templatedPtr[i].~T();
						}
						memmove(&(templatedPtr[start]), &(templatedPtr[finish]), (mSize - finish) * sizeof(T));
						mSize -= (finish - start);
					}
				
				}
				else
				{
					throw exception(indexOutOfBoundsExceptionText.c_str());
				}
			}
			else
			{
				throw exception(cannotMutateExternalStorageExceptionText.c_str());
			}
		}
		else
		{
			throw exception(removeRangeInvalidParamExceptionText.c_str());
		}
	}
	#pragma endregion
}