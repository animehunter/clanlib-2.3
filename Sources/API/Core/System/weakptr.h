/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    Harry Storbacka
**    Thomas Gottschalk Larsen
*/

/// \addtogroup clanCore_System clanCore System
/// \{


#pragma once

#include "../api_core.h"
#include "mutex.h"
#include "sharedptr.h"

/// \brief Weak pointer class (pointer to a CL_SharedPtr object that dont increase reference count).
///
/// Use CL_WeakPtr when you want to have a pointer that is reference counted by CL_SharedPtr
/// but want a pointer that don't increase the reference count.
///
/// The purpose of CL_WeakPtr is to avoid circular loop issues. By using CL_WeakPtr you
/// can construct new CL_SharedPtr'ed objects based on the weak pointer.
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
template <typename Type>
class CL_API_CORE CL_WeakPtr
{
public:
	CL_WeakPtr()
	{
		d.data = 0;
	}

	CL_WeakPtr(const CL_WeakPtr<Type> &copy)
	{
		d.data = 0;
		connect(copy.d.data);
	}

	explicit CL_WeakPtr(const CL_SharedPtr<Type> &copy)
	{
		d.data = 0;
		connect(copy.d.data);
	}

	~CL_WeakPtr()
	{
		disconnect();
	}

public:
	CL_WeakPtr &operator =(const CL_SharedPtr<Type> &copy)
	{
		disconnect();
		connect(copy.d.data);
		return *this;
	}

	CL_WeakPtr &operator =(const CL_WeakPtr &copy)
	{
		if (this == &copy)
			return *this;

		disconnect();
		connect(copy.d.data);
		return *this;
	}

	operator Type* () { return get(); }
	operator const Type* () const { return get(); }

	operator CL_SharedPtr<Type>() { return to_sharedptr(); }
	operator CL_SharedPtr<Type>() const { return to_sharedptr(); }

	template <typename InitType>
	operator CL_SharedPtr<InitType>() { return to_sharedptr(); }

	template <typename InitType>
	operator CL_SharedPtr<InitType>() const { return to_sharedptr(); }

	Type *operator ->() { return get(); }
	const Type *operator ->() const { return get(); }

	template <typename OtherType>
	bool operator ==(OtherType *other) const { return get() == other; }

	template <typename OtherType>
	bool operator !=(OtherType *other) const { return get() != other; }

	template <typename OtherType>
	bool operator <=(OtherType *other) const { return get() <= other; }

	template <typename OtherType>
	bool operator >=(OtherType *other) const { return get() >= other; }

	template <typename OtherType>
	bool operator <(OtherType *other) const { return get() < other; }

	template <typename OtherType>
	bool operator >(OtherType *other) const { return get() > other; }

public:
	/// \brief Tests if the pointer is unset, or the weak link is no longer valid
	/** \return true, pointer not set (or not valid)*/
	bool is_null() const { return is_invalid_weak_link() || d.p == 0; }

	/// \brief Retrieves the pointer
	/** \return The pointer (May be NULL, if it has not been set or the weak link is no longer valid)*/
	Type *get() { return is_invalid_weak_link() ? 0 : *d.p; }

	/// \brief Retrieves the pointer
	/** \return The pointer (May be NULL, if it has not been set or the weak link is no longer valid)*/
	const Type *get() const { return is_invalid_weak_link() ? 0 : *d.p; }

	CL_SharedPtr<Type> to_sharedptr() const
	{
		if (!is_null())
			return CL_SharedPtr<Type>(d.data);
		else
			return CL_SharedPtr<Type>();
	}

private:
	void connect(CL_SharedPtrData *data)
	{
		if (data)
		{
			CL_MutexSection s(&data->mutex);
			++data->refCount;
			d.data = data;
		}
	}

	bool disconnect()
	{
		bool result = false;
		if (d.data)
		{
			CL_MutexSection s(&d.data->mutex);
			result = d.data->strongCount == 0;
			if (--d.data->refCount == 0)
			{
				s.unlock();
				delete d.data;
			}
			d.data = 0;
		}
		return result;
	}

	bool is_invalid_weak_link() const
	{
		if (d.data)
		{
			CL_MutexSection s(&d.data->mutex);
			if (d.data->strongCount > 0)
				return false;
		}
		return true;
	}

	union
	{
		Type **p;
		CL_SharedPtrData *data;
	} d;

	friend class CL_SharedPtr<Type>;
};

/// \}
