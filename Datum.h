#pragma once

/// <summary>
/// The file containing all of the declarations of the Datum class.
/// </summary>

#define GLM_ENABLE_EXPERIMENTAL

#pragma warning(push)
#pragma warning(disable: 4201)
#include "glm/fwd.hpp"
#pragma warning(pop)

#include <string>
#include <initializer_list>
#include <tuple>

namespace FIEAGameEngine
{
	class Scope;
	class RTTI;

	/// <summary>
	/// Datum is a runtime-typed, dynamically sized array used as the value container
	/// in Scope's hierarchical data model.
	///
	/// A Datum stores zero or more elements of a single active DatumType. In isolation,
	/// it behaves like a homogeneous vector whose element type is chosen at runtime.
	/// Once a Datum has been assigned a type, that type is fixed unless the Datum is
	/// assigned from another Datum.
	///
	/// Datum supports two storage modes:
	///
	/// Internal storage:
	/// - The Datum owns its memory.
	/// - Capacity grows as needed when elements are appended.
	/// - Reallocation may move the stored data to a new memory location.
	///
	/// External storage:
	/// - The Datum overlays memory owned by another object.
	/// - The Datum may read and modify the referenced values through Set methods.
	/// - The Datum may not resize, reserve, append past capacity, or otherwise change
	///   the size of the external storage.
	/// - External storage may only be changed by assigning a new external storage block.
	///
	/// When stored inside a Scope, Datum becomes the value side of a string-keyed table
	/// entry. Because Datum can store Scope pointers, Scope and Datum together form a
	/// recursive hierarchical data structure: Scopes map names to Datum values, and
	/// Datum values can represent either leaf data or child Scopes.
	///
	/// This makes Scope and Datum useful as a data-driven object model for parsed JSON,
	/// scripted objects, entity attributes, and runtime-extensible game objects.
	///
	/// Supported Datum types:
	/// - int
	/// - float
	/// - glm::vec4
	/// - glm::mat4x4
	/// - Scope*
	/// - std::string
	/// - RTTI*
	/// </summary>
	class Datum final
	{
	public:
		friend class Scope;

#pragma region DiscriminatedUnion
		/// <summary>
		/// Enum determining what type the datum is storing.
		/// </summary>
		enum class DatumType
		{
			Unknown,
			Integer,
			Float,
			Vector4,
			Matrix4x4,
			Scope,
			String,
			RTTIPtr,
			END
		};

		/// <summary>
		/// Union of pointers of all types the datum could be pointing at.
		/// </summary>
		union DatumPointer
		{
			int * tInt;
			float * tFloat;
			glm::vec4 *  tVec4;
			glm::mat4x4 * tMat4x4;
			Scope** tScope;
			std::string * tString;
			RTTI ** tRTTIPtr;
			void* tVoid;

			DatumPointer() :
				tVoid(nullptr)
			{
			}
		};
#pragma endregion

#pragma region DefaultConstructor
		/// <summary>
		/// Default constructor creatung an empty datum of an unknonw type.
		/// </summary>
		Datum() = default;
#pragma endregion

#pragma region CopyConstructor
		/// <summary>
		/// Datum to datum copy constructor.
		/// Calls the assignment operator.
		/// Deep copies the other datum if it is external storage, otherwise shallow copies an external datum.
		/// </summary>
		/// <param name="other">Datum we with to copy</param>
		Datum(Datum const& other);

		/// <summary>
		/// Datum to integer copy constructor.
		/// </summary>
		/// <param name="other">Integer we wish our datum to store</param>
		Datum(int const& other);

		/// <summary>
		/// Datum to float copy constructor.
		/// </summary>
		/// <param name="other">Float we wish our datum to store</param>
		Datum(float const& other);

		/// <summary>
		/// Datum to vec4 copy constructor.
		/// </summary>
		/// <param name="other">Vec4 we wish our datum to store</param>
		Datum(glm::vec4 const& other);

		/// <summary>
		/// Datum to mat4x4 copy constructor.
		/// </summary>
		/// <param name="other">Mat4x4 we wish our datum to store</param>
		Datum(glm::mat4x4 const& other);
	private:
		/// <summary>
		/// Datum to Scope copy constructor.
		/// </summary>
		/// <param name="other">Scope we wish our datum to store</param>
		Datum(Scope const& other);
	public:
		/// <summary>
		/// Datum to string copy constructor.
		/// </summary>
		/// <param name="other">String we wish our datum to store</param>
		Datum(std::string const& other);

		/// <summary>
		/// Datum to RTTI* copy constructor.
		/// </summary>
		/// <param name="other">RTTI* we wish our datum to store.</param>
		Datum(RTTI * const& other);
#pragma endregion

#pragma region AssignmentOperators
		/// <summary>
		/// Datum to datum assignment operator.
		/// This operation can change the type of a datum as well as whether it is internal or external
		/// </summary>
		/// <param name="other">Datum to assign left hand side to.</param>
		/// <returns>Datum on the left hand side of the assignment operator.</returns>
		Datum& operator= (Datum const& other);

		/// <summary>
		/// Preserve the storage setting of a datum while shallow copying in the contents of the right hand side. 
		/// </summary>
		/// <param name="other">Datum to assign left hand side to.</param>
		/// <returns>Datum on the left hand side of the assignment operator.</returns>
		Datum& StoragePreservedAssignment(Datum const& other);

		/// <summary>
		/// Datum to integer assignment operator.
		/// Sets datum size to 1 and clears all elements previously stored in datum.
		/// </summary>
		/// <param name="other">Integer we wish to store in datum.</param>
		/// <returns>Datum on the left hand side of the assignment operator.</returns>
		Datum& operator= (int const& other);

		/// <summary>
		/// Datum to float assignment operator.
		/// Sets datum size to 1 and clears all elements previously stored in datum.
		/// </summary>
		/// <param name="other">Float we wish to store in datum.</param>
		/// <returns>Datum on the left hand side of the assignment operator.</returns>
		Datum& operator= (float const& other);

		/// <summary>
		/// Datum to vec4 assignment operator.
		/// Sets datum size to 1 and clears all elements previously stored in datum.
		/// </summary>
		/// <param name="other"></param>
		/// <returns>Datum on the left hand side of the assignment operator.</returns>
		Datum& operator= (glm::vec4 const& other);

		/// <summary>
		/// Datum to mat4x4 assignment operator.
		/// Sets datum size to 1 and clears all elements previously stored in datum.
		/// </summary>
		/// <param name="other"></param>
		/// <returns>Datum on the left hand side of the assignment operator.</returns>
		Datum& operator= (glm::mat4x4 const& other);

	private:
		/// <summary>
		/// Datum to Scope* assignment operator.
		/// Sets datum size to 1 and clears all elements previously stored in datum.
		/// </summary>
		/// <param name="other"></param>
		/// <returns>Datum on the left hand side of the assignment operator.</returns>
		Datum& operator= (Scope const& other);
	public:
		/// <summary>
		/// Datum to string assignment operator.
		/// Sets datum size to 1 and clears all elements previously stored in datum.
		/// </summary>
		/// <param name="other"></param>
		/// <returns>Datum on the left hand side of the assignment operator.</returns>
		Datum& operator= (std::string const& other);

		/// <summary>
		/// Datum to RTTI* assignment operator.
		/// Sets datum size to 1 and clears all elements previously stored in datum.
		/// </summary>
		/// <param name="other"></param>
		/// <returns>Datum on the left hand side of the assignment operator.</returns>
		Datum& operator= (RTTI * const& other);
#pragma endregion

#pragma region IntializerListConstructors
		/// <summary>
		/// Populates datum with all elements in the initializer list.
		/// </summary>
		/// <param name="iList">Integer initializer list</param>
		Datum(std::initializer_list<int> const& iList);

		/// <summary>
		/// Populates datum with all elements in the initializer list.
		/// </summary>
		/// <param name="iList">Float initializer list</param>
		Datum(std::initializer_list<float> const& iList);

		/// <summary>
		/// Populates datum with all elements in the initializer list.
		/// </summary>
		/// <param name="iList">Vec4 initializer list</param>
		Datum(std::initializer_list<glm::vec4> const& iList);

		/// <summary>
		/// Populates datum with all elements in the initializer list.
		/// </summary>
		/// <param name="iList">Mat4x4 initializer list</param>
		Datum(std::initializer_list<glm::mat4x4> const& iList);

		/// <summary>
		/// Populates datum with all elements in the initializer list.
		/// </summary>
		/// <param name="iList">Scope* initializer list</param>
		Datum(std::initializer_list<Scope> const& iList);

		/// <summary>
		/// Populates datum with all elements in the initializer list.
		/// </summary>
		/// <param name="iList">String initializer list</param>
		Datum(std::initializer_list<std::string> const& iList);

		/// <summary>
		/// Populates datum with all elements in the initializer list.
		/// </summary>
		/// <param name="iList">RTTI* initializer list</param>
		Datum(std::initializer_list<RTTI*> const& iList);
#pragma endregion

#pragma region Destructor
		/// <summary>
		/// Explicitly destructs all elements of the datum and then frees its memory.
		/// </summary>
		~Datum();
#pragma endregion

#pragma region Reserve
		/// <summary>
		/// Reserves an amount of memory for the vector equal to the new capacity if the new capacity is larger than the old capacity.
		/// </summary>
		/// <param name="newCapacity"> The new capacity of the vector.</param>
		void Reserve(size_t newCapacity);
#pragma endregion

#pragma region Resize
		/// <summary>
		/// Resizes the datum to the given size.
		/// If new size is larger than old size, the new elements added to the datum will be default constructed.
		/// If new size is smaller than old size, the elements outside the bounds of the new size will be explicitly destructed.
		/// </summary>
		/// <param name="newSize">New number of elements in the datum.</param>
		void Resize(size_t newSize);
#pragma endregion

#pragma region ShrinkToFit
		/// <summary>
		/// Reduces capacity to the size of the datum.
		/// </summary>
		void ShrinkToFit();
#pragma endregion

#pragma region EquivalenceOperator
		/// <summary>
		/// Compares a datum to a datum.
		/// </summary>
		/// <param name="other">Datum to compare to.</param>
		/// <returns>Whether the datum and datum are equal.</returns>
		bool operator== (Datum const& other) const;

		/// <summary>
		/// Compares a datum to an integer.
		/// Always returns false if datum size is not one.
		/// </summary>
		/// <param name="other">Integer to compare to.</param>
		/// <returns>Whether the datum and int are equal.</returns>
		bool operator== (int const& other) const;

		/// <summary>
		/// Compares a datum to a float.
		/// Always returns false if datum size is not one.
		/// </summary>
		/// <param name="other">Float to compare to.</param>
		/// <returns>Whether the datum and float are equal.</returns>
		bool operator== (float const& other) const;

		/// <summary>
		/// Compares a datum to a vec4.
		/// Always returns false if datum size is not one.
		/// </summary>
		/// <param name="other">Vec4 to compare to.</param>
		/// <returns>Whether the datum and vec4 are equal.</returns>
		bool operator== (glm::vec4 const& other) const;

		/// <summary>
		/// Compares a datum to a mat4x4.
		/// Always returns false if datum size is not one.
		/// </summary>
		/// <param name="other">Mat4x4 to compare to.</param>
		/// <returns>Whether the datum and mat4x4 are equal.</returns>
		bool operator== (glm::mat4x4 const& other) const;

		/// <summary>
		/// Compares a datum to a string.
		/// Always returns false if datum size is not one.
		/// </summary>
		/// <param name="other">String to compare to.</param>
		/// <returns>Whether the datum and string are equal.</returns>
		bool operator== (std::string const& other) const;

		/// <summary>
		/// Compares a datum to an RTTI*.
		/// Always returns false if datum size is not one.
		/// </summary>
		/// <param name="other">RTTI* to compare to.</param>
		/// <returns>Whether the datum and RTTI* are equal.</returns>
		bool operator== (RTTI * const& other) const;

		/// <summary>
		/// Compares a datum to a datum.
		/// </summary>
		/// <param name="other">Datum to compare to.</param>
		/// <returns>Whether the datum and datum are not equal.</returns>
		bool operator!= (Datum const& other) const;

		/// <summary>
		/// Compares a datum to an integer.
		/// Always returns true if datum size is not one.
		/// </summary>
		/// <param name="other">Integer to compare to.</param>
		/// <returns>Whether the datum and int are not equal.</returns>
		bool operator!= (int const& other) const;

		/// <summary>
		/// Compares a datum to a float.
		/// Always returns true if datum size is not one.
		/// </summary>
		/// <param name="other">Float to compare to.</param>
		/// <returns>Whether the datum and float are not equal.</returns>
		bool operator!= (float const& other) const;

		/// <summary>
		/// Compares a datum to a vec4.
		/// Always returns true if datum size is not one.
		/// </summary>
		/// <param name="other">Vec4 to compare to.</param>
		/// <returns>Whether the datum and vec4 are not equal.</returns>
		bool operator!= (glm::vec4 const& other) const;

		/// <summary>
		/// Compares a datum to a mat4x4.
		/// Always returns true if datum size is not one.
		/// </summary>
		/// <param name="other">Mat4x4 to compare to.</param>
		/// <returns>Whether the datum and mat4x4 are not equal.</returns>
		bool operator!= (glm::mat4x4 const& other) const;

		/// <summary>
		/// Compares a datum to a string.
		/// Always returns true if datum size is not one.
		/// </summary>
		/// <param name="other">String to compare to.</param>
		/// <returns>Whether the datum and string are not equal.</returns>
		bool operator!= (std::string const& other) const;

		/// <summary>
		/// Compares a datum to an RTTI*.
		/// Always returns true if datum size is not one.
		/// </summary>
		/// <param name="other">RTTI* to compare to.</param>
		/// <returns>Whether the datum and RTTI* are not equal.</returns>
		bool operator!= (RTTI * const& other) const;
#pragma endregion

#pragma region operator[]
		/// <summary>
		/// Syntactic sugar allowing for easy access to nested scopes.
		/// </summary>
		/// <param name="index">Index of the scope we want to access.</param>
		/// <returns>Scope (not Scope*) at the given index.</returns>
		Scope& operator[](size_t index);
#pragma endregion

#pragma region Type
		/// <summary>
		/// Returns the datum's type.
		/// </summary>
		/// <returns>An enum specifying the datum's type.</returns>
		DatumType Type() const;
#pragma endregion

#pragma region SetType
		/// <summary>
		/// Sets the type of the datum. Only works if the datum's tyoe is unknown. 
		/// </summary>
		/// <param name="type">Enum specifying the type of the new datum.</param>
		DatumType SetType(DatumType type);
#pragma endregion

#pragma region Size
		/// <summary>
		/// Returns the size of the datum.
		/// </summary>
		/// <returns>The size of the datum.</returns>
		size_t Size() const;
#pragma endregion

#pragma region Capacity
		/// <summary>
		/// Returns the capacity of datum.
		/// </summary>
		/// <returns>The capacity of datum.</returns>
		size_t Capacity() const;
#pragma endregion

#pragma region Clear
		/// <summary>
		/// Explicitly destructs all of the contents of the datum.
		/// </summary>
		void Clear();
#pragma endregion

#pragma region SetStorage
		/// <summary>
		/// Sets the datum to external storage, overlaying an existing array.
		/// Size, capacity, and type become immutable. (Unless this datum is assigned to another datum. Size, but not type, can also be changed by calling set storage again.)
		/// </summary>
		/// <param name="data">Int pointer to overlay.</param>
		/// <param name="size">Number of ints to overlay.</param>
		void SetStorage(int * const& data, size_t const& size);

		/// <summary>
		/// Sets the datum to external storage, overlaying an existing array.
		/// Size, capacity, and type become immutable. (Unless this datum is assigned to another datum. Size, but not type, can also be changed by calling set storage again.)
		/// </summary>
		/// <param name="data">Float pointer to overlay.</param>
		/// <param name="size">Number of floats to overlay.</param>
		void SetStorage(float * const& data, size_t const& size);

		/// <summary>
		/// Sets the datum to external storage, overlaying an existing array.
		/// Size, capacity, and type become immutable. (Unless this datum is assigned to another datum. Size, but not type, can also be changed by calling set storage again.)
		/// </summary>
		/// <param name="data">Vec4 pointer to overlay.</param>
		/// <param name="size">Number of vec4s to overlay.</param>
		void SetStorage(glm::vec4 * const& data, size_t const& size);

		/// <summary>
		/// Sets the datum to external storage, overlaying an existing array.
		/// Size, capacity, and type become immutable. (Unless this datum is assigned to another datum. Size, but not type, can also be changed by calling set storage again.)
		/// </summary>
		/// <param name="data">Mat4x4 pointer to overlay.</param>
		/// <param name="size">Number of mat4x4s to overlay.</param>
		void SetStorage(glm::mat4x4 * const& data, size_t const& size);

		//Scopes cannot be stored as external storage

		/// <summary>
		/// Sets the datum to external storage, overlaying an existing array.
		/// Size, capacity, and type become immutable. (Unless this datum is assigned to another datum. Size, but not type, can also be changed by calling set storage again.)
		/// </summary>
		/// <param name="data">String pointer to overlay.</param>
		/// <param name="size">Number of strings to overlay.</param>
		void SetStorage(std::string * const& data, size_t const& size);

		/// <summary>
		/// Sets the datum to external storage, overlaying an existing array.
		/// Size, capacity, and type become immutable. (Unless this datum is assigned to another datum. Size, but not type, can also be changed by calling set storage again.)
		/// </summary>
		/// <param name="data">RTTI* pointer to overlay.</param>
		/// <param name="size">Number of RTTI*s to overlay.</param>
		void SetStorage(RTTI ** const& data, size_t const& size);
#pragma endregion

#pragma region IsInternal
		/// <summary>
		/// Returns if the datum is internal storage.
		/// </summary>
		/// <returns>If the datum is internal storage.</returns>
		bool IsInternal() const;
#pragma endregion

#pragma region Set
		/// <summary>
		/// Sets data at the given index to the given value.
		/// </summary>
		/// <param name="value">Value to set data to.</param>
		/// <param name="index">Index to set to value.</param>
		void Set(int const& value, size_t const& index = 0);

		/// <summary>
		/// Sets data at the given index to the given value.
		/// </summary>
		/// <param name="value">Value to set data to.</param>
		/// <param name="index">Index to set to value.</param>
		void Set(float const& value, size_t const& index = 0);

		/// <summary>
		/// Sets data at the given index to the given value.
		/// </summary>
		/// <param name="value">Value to set data to.</param>
		/// <param name="index">Index to set to value.</param>
		void Set(glm::vec4 const& value, size_t const& index = 0);

		/// <summary>
		/// Sets data at the given index to the given value.
		/// </summary>
		/// <param name="value">Value to set data to.</param>
		/// <param name="index">Index to set to value.</param>
		void Set(glm::mat4x4 const& value, size_t const& index = 0);

		/// <summary>
		/// Sets data at the given index to the given value.
		/// </summary>
		/// <param name="value">Value to set data to.</param>
		/// <param name="index">Index to set to value.</param>
		void Set(Scope const& value, size_t const& index = 0);

		/// <summary>
		/// Sets data at the given index to the given value.
		/// </summary>
		/// <param name="value">Value to set data to.</param>
		/// <param name="index">Index to set to value.</param>
		void Set(std::string const& value, size_t const& index = 0);

		/// <summary>
		/// Sets data at the given index to the given value.
		/// </summary>
		/// <param name="value">Value to set data to.</param>
		/// <param name="index">Index to set to value.</param>
		void Set(RTTI * const& value, size_t const& index = 0);
#pragma endregion

#pragma region Get
		/// <summary>
		/// Templated get.
		/// If this function is ever called it means it was called on a type the datum does not support.
		/// Thus it always throws an exception.
		/// </summary>
		/// <param name="index">Index to retrieve data from.</param>
		/// <returns>Data stored at given index.</returns>
		template<typename T> T Get(size_t const& index = 0) const;

		/// <summary>
		/// Int specialization of templated get function.
		/// Returns the data at the specified value.
		/// </summary>
		/// <param name="index">Index to retrieve data from.</param>
		/// <returns>Data stored at given index.</returns>
		template<> int Get<int>(size_t const& index) const;

		/// <summary>
		/// Float specialization of templated get function.
		/// Returns the data at the specified value.
		/// </summary>
		/// <param name="index">Index to retrieve data from.</param>
		/// <returns>Data stored at given index.</returns>
		template<> float Get<float>(size_t const& index) const;

		/// <summary>
		/// Vec4 specialization of templated get function.
		/// Returns the data at the specified value.
		/// </summary>
		/// <param name="index">Index to retrieve data from.</param>
		/// <returns>Data stored at given index.</returns>
		template<> glm::vec4 Get<glm::vec4>(size_t const& index) const;

		/// <summary>
		/// Mat4x4 specialization of templated get function.
		/// Returns the data at the specified value.
		/// </summary>
		/// <param name="index">Index to retrieve data from.</param>
		/// <returns>Data stored at given index.</returns>
		template<> glm::mat4x4 Get<glm::mat4x4>(size_t const& index) const;

		/// <summary>
		/// Mat4x4 specialization of templated get function.
		/// Returns the data at the specified value.
		/// </summary>
		/// <param name="index">Index to retrieve data from.</param>
		/// <returns>Data stored at given index.</returns>
		template<> Scope* Get<Scope*>(size_t const& index) const;

		/// <summary>
		/// String specialization of templated get function.
		/// Returns the data at the specified value.
		/// </summary>
		/// <param name="index">Index to retrieve data from.</param>
		/// <returns>Data stored at given index.</returns>
		template<> std::string Get<std::string>(size_t const& index) const;

		/// <summary>
		/// RTTI specialization of templated get function.
		/// Returns the data at the specified value.
		/// </summary>
		/// <param name="index">Index to retrieve data from.</param>
		/// <returns>Data stored at given index.</returns>
		template<> RTTI* Get<RTTI*>(size_t const& index) const;
#pragma endregion

#pragma region PushBackFromString
		/// <summary>
		/// Pushes back the string value converted to the datum's data type.
		/// </summary>
		/// <param name="value">Value to set data to.</param>
		void PushBackFromString(std::string const& value);
#pragma endregion

#pragma region SetFromString
		/// <summary>
		/// Sets data at the given index to the passed in string value converted to the datum's data type.
		/// </summary>
		/// <param name="value">Value to set data to.</param>
		/// <param name="index">Index to set to value.</param>
		void SetFromString(std::string const& value, size_t const& index = 0);
#pragma endregion



#pragma region ToString
		/// <summary>
		/// Converts the data at a given index to a string value.
		/// </summary>
		/// <param name="index">Index to retrieve data from.</param>
		/// <returns>Data at the given index converted to a string.</returns>
		std::string ToString(size_t const& index = 0) const;
#pragma endregion

#pragma region PushBack
		/// <summary>
		/// Increases the datum's size by one and places the given value at the back of the datum.
		/// </summary>
		/// <param name="value">Value to push back.</param>
		int & PushBack(int const& value);

		/// <summary>
		/// Increases the datum's size by one and places the given value at the back of the datum.
		/// </summary>
		/// <param name="value">Value to push back.</param>
		float & PushBack(float const& value);

		/// <summary>
		/// Increases the datum's size by one and places the given value at the back of the datum.
		/// </summary>
		/// <param name="value">Value to push back.</param>
		glm::vec4 & PushBack(glm::vec4 const& value);

		/// <summary>
		/// Increases the datum's size by one and places the given value at the back of the datum.
		/// </summary>
		/// <param name="value">Value to push back.</param>
		glm::mat4x4 & PushBack(glm::mat4x4 const& value);

		/// <summary>
		/// Increases the datum's size by one and places the given value at the back of the datum.
		/// </summary>
		/// <param name="value">Value to push back.</param>
		Scope* & PushBack(Scope const& value);
	public:
		/// <summary>
		/// Increases the datum's size by one and places the given value at the back of the datum.
		/// </summary>
		/// <param name="value">Value to push back.</param>
		std::string & PushBack(std::string const& value);

		/// <summary>
		/// Increases the datum's size by one and places the given value at the back of the datum.
		/// </summary>
		/// <param name="value">Value to push back.</param>
		RTTI* & PushBack(RTTI * const& value);
#pragma endregion

#pragma region PopBack
		/// <summary>
		/// Reduces the datum's size by 1 and explicitly destructs the last element in the datum.
		/// </summary>
		void PopBack();
#pragma endregion

#pragma region Front
		/// <summary>
		/// Templated front.
		/// If this function is ever called it means it was called on a type the datum does not support.
		/// Thus it always throws an exception.
		/// </summary>
		/// <returns>The data at the first index of the datum.</returns>
		template<typename T> T Front() const;

		/// <summary>
		/// Int specialization of templated front function.
		/// Returns the data at the first index of the datum.
		/// </summary>
		/// <returns>The data at the first index of the datum.</returns>
		template<> int Front<int>() const;

		/// <summary>
		/// Float specialization of templated front function.
		/// Returns the data at the first index of the datum.
		/// </summary>
		/// <returns>The data at the first index of the datum.</returns>
		template<> float Front<float>() const;

		/// <summary>
		/// Vec4 specialization of templated front function.
		/// Returns the data at the first index of the datum.
		/// </summary>
		/// <returns>The data at the first index of the datum.</returns>
		template<> glm::vec4 Front<glm::vec4>() const;

		/// <summary>
		/// Mat4x4 specialization of templated front function.
		/// Returns the data at the first index of the datum.
		/// </summary>
		/// <returns>The data at the first index of the datum.</returns>
		template<> glm::mat4x4 Front<glm::mat4x4>() const;

		/// <summary>
		/// Scope* specialization of templated front function.
		/// Returns the data at the first index of the datum.
		/// </summary>
		/// <returns>The data at the first index of the datum.</returns>
		template<> Scope* Front<Scope*>() const;

		/// <summary>
		/// String specialization of templated front function.
		/// Returns the data at the first index of the datum.
		/// </summary>
		/// <returns>The data at the first index of the datum.</returns>
		template<> std::string Front<std::string>() const;

		/// <summary>
		/// RTTI* specialization of templated front function.
		/// Returns the data at the first index of the datum.
		/// </summary>
		/// <returns>The data at the first index of the datum.</returns>
		template<> RTTI* Front<RTTI*>() const;
#pragma endregion

#pragma region Back
		/// <summary>
		/// Templated back.
		/// If this function is ever called it means it was called on a type the datum does not support.
		/// Thus it always throws an exception.
		/// </summary>
		/// <returns>Data stored at last index of datum.</returns>
		template<typename T> T Back() const;

		/// <summary>
		/// Int specialization of templated back function.
		/// Returns the data at the last index of the datum.
		/// </summary>
		/// <returns>The data at the last index of the datum.</returns>
		template<> int Back<int>() const;

		/// <summary>
		/// Float specialization of templated back function.
		/// Returns the data at the last index of the datum.
		/// </summary>
		/// <returns>The data at the last index of the datum.</returns>
		template<> float Back<float>() const;

		/// <summary>
		/// Vec4 specialization of templated back function.
		/// Returns the data at the last index of the datum.
		/// </summary>
		/// <returns>The data at the last index of the datum.</returns>
		template<> glm::vec4 Back<glm::vec4>() const;

		/// <summary>
		/// Mat4x4 specialization of templated back function.
		/// Returns the data at the last index of the datum.
		/// </summary>
		/// <returns>The data at the last index of the datum.</returns>
		template<> glm::mat4x4 Back<glm::mat4x4>() const;

		/// <summary>
		/// Scope* specialization of templated back function.
		/// Returns the data at the last index of the datum.
		/// </summary>
		/// <returns>The data at the last index of the datum.</returns>
		template<> Scope* Back<Scope*>() const;

		/// <summary>
		/// String specialization of templated back function.
		/// Returns the data at the last index of the datum.
		/// </summary>
		/// <returns>The data at the last index of the datum.</returns>
		template<> std::string Back<std::string>() const;

		/// <summary>
		/// RTTI* specialization of templated back function.
		/// Returns the data at the last index of the datum.
		/// </summary>
		/// <returns>The data at the last index of the datum.</returns>
		template<> RTTI* Back<RTTI*>() const;
#pragma endregion

#pragma region Remove
		/// <summary>
		/// Removes the passed in value from the datum if it exists.
		/// </summary>
		/// <param name="value">Value to remove.</param>
		void Remove(int const& value);

		/// <summary>
		/// Removes the passed in value from the datum if it exists.
		/// </summary>
		/// <param name="value">Value to remove.</param>
		void Remove(float const& value);

		/// <summary>
		/// Removes the passed in value from the datum if it exists.
		/// </summary>
		/// <param name="value">Value to remove.</param>
		void Remove(glm::vec4 const& value);

		/// <summary>
		/// Removes the passed in value from the datum if it exists.
		/// </summary>
		/// <param name="value">Value to remove.</param>
		void Remove(glm::mat4x4 const& value);

		/// <summary>
		/// Removes the passed in value from the datum if it exists.
		/// </summary>
		/// <param name="value">Value to remove.</param>
		void Remove(std::string const& value);

		/// <summary>
		/// Removes the passed in value from the datum if it exists.
		/// </summary>
		/// <param name="value">Value to remove.</param>
		void Remove(RTTI* const& value);
#pragma endregion

#pragma region RemoveAt
		/// <summary>
		/// Removes the value at the passed in index from the datum.
		/// </summary>
		/// <param name="index">Index to remove value at.</param>
		void RemoveAt(size_t const& index);
#pragma endregion

#pragma region RemoveRange
		/// <summary>
		/// Removes all values between starting index and ending index. Inclusive of start exclusive of end.
		/// </summary>
		/// <param name="start">Starting index.</param>
		/// <param name="finish">Ending index.</param>
		void RemoveRange(size_t const& start, size_t const& finish);
#pragma endregion

#pragma region Find
		/// <summary>
		/// Searches the datum for a given value.
		/// </summary>
		/// <param name="searchedValue">Value to search the datum for.</param>
		/// <returns>A pair with the first element relaying whether the value was found and the second element as a pointer to the found value.</returns>
		std::pair<bool, int*> Find(int const& searchedValue) const;

		/// <summary>
		/// Searches the datum for a given value.
		/// </summary>
		/// <param name="searchedValue">Value to search the datum for.</param>
		/// <returns>A pair with the first element relaying whether the value was found and the second element as a pointer to the found value.</returns>
		std::pair<bool, float*> Find(float const& searchedValue) const;

		/// <summary>
		/// Searches the datum for a given value.
		/// </summary>
		/// <param name="searchedValue">Value to search the datum for.</param>
		/// <returns>A pair with the first element relaying whether the value was found and the second element as a pointer to the found value.</returns>
		std::pair<bool, glm::vec4*> Find(glm::vec4 const& searchedValue) const;

		/// <summary>
		/// Searches the datum for a given value.
		/// </summary>
		/// <param name="searchedValue">Value to search the datum for.</param>
		/// <returns>A pair with the first element relaying whether the value was found and the second element as a pointer to the found value.</returns>
		std::pair<bool, glm::mat4x4*> Find(glm::mat4x4 const& searchedValue) const;


		/// <summary>
		/// Searches the datum for a given value.
		/// </summary>
		/// <param name="searchedValue">Value to search the datum for.</param>
		/// <returns>A pair with the first element relaying whether the value was found and the second element as a pointer to the found value.</returns>
		std::pair<bool, Scope**> Find(Scope const& searchedValue) const;

		/// <summary>
		/// Searches the datum for a given value.
		/// </summary>
		/// <param name="searchedValue">Value to search the datum for.</param>
		/// <returns>A pair with the first element relaying whether the value was found and the second element as a pointer to the found value.</returns>
		std::pair<bool, std::string*> Find(std::string const& searchedValue) const;

		/// <summary>
		/// Searches the datum for a given value.
		/// </summary>
		/// <param name="searchedValue">Value to search the datum for.</param>
		/// <returns>A pair with the first element relaying whether the value was found and the second element as a pointer to the found value.</returns>
		std::pair<bool, RTTI **> Find(RTTI * const& searchedValue) const;
#pragma endregion

		/// <summary>
		/// 
		/// </summary>
		/// <param name="data"></param>
		/// <param name="size"></param>
		void SetStorage(void * const& data, size_t const& size);

		/// <summary>
		/// Scope of type sizes. Specified at the top of cpp file.
		/// </summary>
		static size_t const mTypeSizes[static_cast<size_t>(DatumType::END)];

	private:
#pragma region ResizeTemplated
		/// <summary>
		/// Templated resize function that takes in a pointer to the internal array of the appropriate pointer type.
		/// The appropriate pointer type is determined by the enum specifiying the datum's type.
		/// Resizes the datum to the given size.
		/// If new size is larger than old size, the new elements added to the datum will be default constructed.
		/// If new size is smaller than old size, the elements outside the bounds of the new size will be explicitly destructed.
		/// </summary>
		/// <param name="templatedPtr">A pointer of the type the datum is.</param>
		/// <param name="newSize">New size of the datum</param>
		template<typename T> void ResizeTemplated(T * const& templatedPtr, size_t const& newSize);
#pragma endregion

#pragma region ClearTemplated
		/// <summary>
		/// Explicitly calls the destructor on all of our datum elements and set's our size to 0.
		/// The appropriate pointer type is determined by the enum specifiying the datum's type.
		/// </summary>
		/// <param name="templatedPtr">A pointer of the type the datum is.</param>
		template<typename T> void ClearTemplated(T * const& templatedPtr);
#pragma endregion

#pragma region SetFromStringTemplated
		/// <summary>
		/// Templated set from string.
		/// If this function is ever called it means it was called on a type the datum does not support.
		/// Thus it always throws an exception.
		/// </summary>
		/// <param name="value">String representative to be converted to data of approproate type.</param>
		/// <param name="index">Index to set to value.</param>
		template<typename T> void SetFromStringTemplated(std::string const& value, size_t const& index = 0);

		/// <summary>
		/// Sets data at the given index to the given string value converted to the appropriate type.
		/// </summary>
		/// <param name="value">String representative to be converted to data of approproate type.</param>
		/// <param name="index">Index to set to value.</param>
		template<> void SetFromStringTemplated<int>(std::string const& value, size_t const& index);

		/// <summary>
		/// Sets data at the given index to the given string value converted to the appropriate type.
		/// </summary>
		/// <param name="value">String representative to be converted to data of approproate type.</param>
		/// <param name="index">Index to set to value.</param>
		template<> void SetFromStringTemplated<float>(std::string const& value, size_t const& index);

		/// <summary>
		/// Sets data at the given index to the given string value converted to the appropriate type.
		/// </summary>
		/// <param name="value">String representative to be converted to data of approproate type.</param>
		/// <param name="index">Index to set to value.</param>
		template<> void SetFromStringTemplated<glm::vec4>(std::string const& value, size_t const& index);

		/// <summary>
		/// Sets data at the given index to the given string value converted to the appropriate type.
		/// </summary>
		/// <param name="value">String representative to be converted to data of approproate type.</param>
		/// <param name="index">Index to set to value.</param>
		template<> void SetFromStringTemplated<glm::mat4x4>(std::string const& value, size_t const& index);

		/// <summary>
		/// Sets data at the given index to the given string value converted to the appropriate type.
		/// </summary>
		/// <param name="value">String representative to be converted to data of approproate type.</param>
		/// <param name="index">Index to set to value.</param>
		template<> void SetFromStringTemplated<std::string>(std::string const& value, size_t const& index);

		/// <summary>
		/// Sets data at the given index to the given string value converted to the appropriate type.
		/// </summary>
		/// <param name="value">String representative to be converted to data of approproate type.</param>
		/// <param name="index">Index to set to value.</param>
		template<> void SetFromStringTemplated<RTTI*>(std::string const& value, size_t const& index);
#pragma endregion

#pragma region PushBackFromStringTemplated
		/// <summary>
		/// Templated push back from string.
		/// If this function is ever called it means it was called on a type the datum does not support.
		/// Thus it always throws an exception.
		/// </summary>
		/// <param name="value">String representative to be converted to data of approproate type.</param>
		template<typename T> void PushBackFromStringTemplated(std::string const& value);

		/// <summary>
		/// Pushes back the string value converted to the appropriate type.
		/// </summary>
		/// <param name="value">String representative to be converted to data of approproate type.</param>
		template<> void PushBackFromStringTemplated<int>(std::string const& value);

		/// <summary>
		/// Pushes back the string value converted to the appropriate type.
		/// </summary>
		/// <param name="value">String representative to be converted to data of approproate type.</param>
		template<> void PushBackFromStringTemplated<float>(std::string const& value);

		/// <summary>
		/// Pushes back the string value converted to the appropriate type.
		/// </summary>
		/// <param name="value">String representative to be converted to data of approproate type.</param>
		template<> void PushBackFromStringTemplated<glm::vec4>(std::string const& value);

		/// <summary>
		/// Pushes back the string value converted to the appropriate type.
		/// </summary>
		/// <param name="value">String representative to be converted to data of approproate type.</param>
		template<> void PushBackFromStringTemplated<glm::mat4x4>(std::string const& value);

		/// <summary>
		/// Pushes back the string value converted to the appropriate type.
		/// </summary>
		/// <param name="value">String representative to be converted to data of approproate type.</param>
		template<> void PushBackFromStringTemplated<std::string>(std::string const& value);

		/// <summary>
		/// Pushes back the string value converted to the appropriate type.
		/// </summary>
		/// <param name="value">String representative to be converted to data of approproate type.</param>
		template<> void PushBackFromStringTemplated<RTTI*>(std::string const& value);
#pragma endregion

#pragma region ToStringTemplated
		/// <summary>
		/// Templated to string.
		/// If this function is ever called it means it was called on a type the datum does not support.
		/// Thus it always throws an exception.
		/// The appropriate pointer type is determined by the enum specifiying the datum's type.
		/// </summary>
		/// <param name="templatedPtr">A pointer of the type the datum is.</param>
		/// <param name="index">Index to get data from to convert to string.</param>
		/// <returns>Data stored at index as a string.</returns>
		template<typename T> std::string ToStringTemplated(T * const& templatedPtr, size_t const& index = 0) const;

		/// <summary>
		/// Returns data at a given index as a string.
		/// The appropriate pointer type is determined by the enum specifiying the datum's type.
		/// </summary>
		/// <param name="templatedPtr">A pointer of the type the datum is.</param>
		/// <param name="index">Index to get data from to convert to string.</param>
		/// <returns>Data stored at index as a string.</returns>
		template<> std::string ToStringTemplated<int>(int * const& templatedPtr, size_t const& index) const;

		/// <summary>
		/// Returns data at a given index as a string.
		/// The appropriate pointer type is determined by the enum specifiying the datum's type.
		/// </summary>
		/// <param name="templatedPtr">A pointer of the type the datum is.</param>
		/// <param name="index">Index to get data from to convert to string.</param>
		/// <returns>Data stored at index as a string.</returns>
		template<> std::string ToStringTemplated<float>(float * const& templatedPtr, size_t const& index) const;

		/// <summary>
		/// Returns data at a given index as a string.
		/// The appropriate pointer type is determined by the enum specifiying the datum's type.
		/// </summary>
		/// <param name="templatedPtr">A pointer of the type the datum is.</param>
		/// <param name="index">Index to get data from to convert to string.</param>
		/// <returns>Data stored at index as a string.</returns>
		template<> std::string ToStringTemplated<glm::vec4>(glm::vec4 * const& templatedPtr, size_t const& index) const;

		/// <summary>
		/// Returns data at a given index as a string.
		/// The appropriate pointer type is determined by the enum specifiying the datum's type.
		/// </summary>
		/// <param name="templatedPtr">A pointer of the type the datum is.</param>
		/// <param name="index">Index to get data from to convert to string.</param>
		/// <returns>Data stored at index as a string.</returns>
		template<> std::string ToStringTemplated<glm::mat4x4>(glm::mat4x4 * const& templatedPtr, size_t const& index) const;

		/// <summary>
		/// Returns data at a given index as a string.
		/// The appropriate pointer type is determined by the enum specifiying the datum's type.
		/// </summary>
		/// <param name="templatedPtr">A pointer of the type the datum is.</param>
		/// <param name="index">Index to get data from to convert to string.</param>
		/// <returns>Data stored at index as a string.</returns>
		template<> std::string ToStringTemplated<std::string>(std::string * const& templatedPtr, size_t const& index) const;

		/// <summary>
		/// Returns data at a given index as a string.
		/// The appropriate pointer type is determined by the enum specifiying the datum's type.
		/// </summary>
		/// <param name="templatedPtr">A pointer of the type the datum is.</param>
		/// <param name="index">Index to get data from to convert to string.</param>
		/// <returns>Data stored at index as a string.</returns>
		template<> std::string ToStringTemplated<RTTI*>(RTTI ** const& templatedPtr, size_t const& index) const;
#pragma endregion

#pragma region PushBackTemplated
		/// <summary>
		/// Increases the datum's size by one and places the given value at the back of the datum.
		/// Reserves more space for the datum if necessary.
		/// The appropriate pointer type for reserve is determined by the enum specifiying the datum's type.
		/// </summary>
		/// <param name="templatedPtr">A pointer of the type the datum is.</param>
		/// <param name="value">Value to push back.</param>
		template<typename T> T & PushBackTemplated(T * const& templatedPtr, T const& value);
#pragma endregion

#pragma region PopBackTemplated
		/// <summary>
		/// Reduces the datum's size by 1 and explicitly destructs the last element in the datum.
		/// The appropriate pointer type is determined by the enum specifiying the datum's type.
		/// </summary>
		/// <param name="templatedPtr">A pointer of the type the datum is.</param>
		template<typename T> void PopBackTemplated(T * const& templatedPtr);
#pragma endregion

#pragma region RemoveTemplated
		/// <summary>
		/// Removes the passed in value from the datum if it exists.
		/// The appropriate pointer type is determined by the enum specifiying the datum's type.
		/// </summary>
		/// <param name="templatedPtr">A pointer of the type the datum is.</param>
		/// <param name="value">Value to remove.</param>
		template<typename T> void RemoveTemplated(T * const& templatedPtr, T const& value);
#pragma endregion

#pragma region RemoveAtTemplated
		/// <summary>
		///  Removes the value at the passed in index from the datum.
		/// The appropriate pointer type is determined by the enum specifiying the datum's type.
		/// </summary>
		/// <param name="templatedPtr">A pointer of the type the datum is.</param>
		/// <param name="index">Index to remove value at.</param>
		template<typename T> void RemoveAtTemplated(T * const& templatedPtr, size_t const& index = 0);
#pragma endregion

#pragma region RemoveRangeTemplated
		/// <summary>
		/// Removes all values between starting index and ending index. Inclusive of start exclusive of end.
		/// The appropriate pointer type is determined by the enum specifiying the datum's type.
		/// </summary>
		/// <param name="templatedPtr">A pointer of the type the datum is.</param>
		/// <param name="start">Starting index.</param>
		/// <param name="finish">Ending index.</param>
		template<typename T> void RemoveRangeTemplated(T * const& templatedPtr, size_t const& start = 0, size_t const& finish = 0);
#pragma endregion

#pragma region MemberData
		/// <summary>
		/// Pointer to the byte string stored by the datum.
		/// </summary>
		DatumPointer mArray;

		/// <summary>
		/// Number of constructed elements in the datum.
		/// </summary>
		size_t mSize = 0;

		/// <summary>
		/// Amount of elements that could potentially be stored in the datum before needing to reserve more memory.
		/// </summary>
		size_t mCapacity = 0;

		/// <summary>
		/// Type of data the datum is storing.
		/// </summary>
		DatumType mType = DatumType::Unknown;

		/// <summary>
		/// Whether the datum is pointing to internal storage.
		/// </summary>
		bool mIsInternal = true;

#pragma endregion

#pragma region ExceptionStrings
		/// <summary>
		/// Communicates the cause of a data lost on resize exception to the user.
		/// </summary>
		inline static const std::string newCapacityCannotBeSmallerThanCapacityExceptionText = "New capacity cannot be smaller than old vector capacity.\n";

		/// <summary>
		/// Communicates the cause of an index out of bounds exception to the user.
		/// </summary>
		inline static const std::string indexOutOfBoundsExceptionText = "Attempted to access an out of bounds index.\n";

		/// <summary>
		/// Communicates the cause of an operation type mismatch exception to the user.
		/// </summary>
		inline static const std::string operationTypeMismatchExceptionText = "Operation type does not match datum type.\n";

		/// <summary>
		/// Communicates the cause of a datum type not initialized exception to the user.
		/// </summary>
		inline static const std::string datumTypeNotInitializedExceptionText = "Datum type is not initialized.\n";

		/// <summary>
		/// Communicates the cause of a datum type already initialized exception to the user.
		/// </summary>
		inline static const std::string datumTypeAlreadyInitializedExceptionText = "Datum type is already initialized. Once the data type is set it may not be changed.\n";

		/// <summary>
		/// Communicates the cause of an unsupported data type exception to the user.
		/// </summary>
		inline static const std::string unsupportedDataTypeExceptionText = "Data type passed in to templated function is not supported by datum.\n";

		/// <summary>
		/// Communicates the cause of a cannot mutate external storage exception to the user.
		/// </summary>
		inline static const std::string cannotMutateExternalStorageExceptionText = "Cannot mutate external storage.\n";

		/// <summary>
		/// Communicates the cause of a cannot set RTTI* from string exception to the user.
		/// </summary>
		inline static const std::string cannotSetRTTIPtrFromStringExceptionText = "Cannot set RTTI * from string.\n";

		/// <summary>
		/// Communicates the cause of a set stroage invalid parameters exception to the user.
		/// </summary>
		inline static const std::string cannotSetStorageInvalidParamExceptionText = "Cannot set storage to nullptr or size 0.\n";

		/// <summary>
		/// Communicates the cause of a set stroage invalid parameters exception to the user.
		/// </summary>
		inline static const std::string bracketOperatorOnlyValidForScopesExceptionText = "Operator[] is only valid for scopes.\n";

		/// <summary>
		/// Communicates the cause of a set stroage invalid parameters exception to the user.
		/// </summary>
		inline static const std::string noExternalScopesExceptionText = "External datum cannot be assigned to scope.\n";

		/// <summary>
		/// Communicates the cause of a storage preserved assignment type mismatch exception to the user.
		/// </summary>
		inline static const std::string storagePreservedAssignmentTypeMismatchExceptionText = "External datum cannot change it's type to match assigned datum.\n";
		
		/// <summary>
		/// Communicates the cause of a remove range invalid parameter exception to the user.
		/// </summary>
		inline static const std::string removeRangeInvalidParamExceptionText = "Remove range finish index cannot be before start index.\n";
#pragma endregion

		// TODO: Replace repeated DatumType switch dispatch with static member-function tables by implementing default functions table.
		// Each table should map DatumType values to the type-specific helper used to construct,
		// destroy, or copy elements of that type.

		//using CreateDefaultFunction = void(Datum::*)(size_t index);
		//static const CreateDefaultFunction CreateDefaultFunctions[static_cast<int>(DatumType::END)];

		//using DeleteFunction = void(Datum::*)();

		//using CopyFunction = void(Datum::*)();
	};
}

#include "Datum.inl"