/**
 * \file
 * \brief ThreadControlBlockList class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-03-13
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_THREADCONTROLBLOCKLIST_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_THREADCONTROLBLOCKLIST_HPP_

#include "distortos/scheduler/ThreadControlBlock.hpp"

#include "distortos/containers/SortedContainer.hpp"

namespace distortos
{

namespace scheduler
{

/// functor which gives descending effective priority order of elements on the list
struct ThreadControlBlockDescendingEffectivePriority
{
	/**
	 * \brief ThreadControlBlockDescendingEffectivePriority's function call operator
	 *
	 * \param [in] left is the object on the left side of comparison
	 * \param [in] right is the object on the right side of comparison
	 *
	 * \return true if left's effective priority is less than right's effective priority
	 */

	bool operator()(const ThreadControlBlockListValueType& left, const ThreadControlBlockListValueType& right) const
	{
		return left.get().getEffectivePriority() < right.get().getEffectivePriority();
	}
};

/// base of ThreadControlBlockList
using ThreadControlBlockListBase = containers::SortedContainer
		<
				ThreadControlBlockUnsortedList,
				ThreadControlBlockDescendingEffectivePriority
		>;

/// List of ThreadControlBlock objects in descending order of effective priority that configures state of kept objects
class ThreadControlBlockList : private ThreadControlBlockListBase
{
public:

	/// base of ThreadControlBlockList
	using Base = ThreadControlBlockListBase;

	using typename Base::iterator;
	using typename Base::value_type;

	using Base::begin;
	using Base::empty;
	using Base::end;
	using Base::size;

	/**
	 * \brief ThreadControlBlockList's constructor
	 *
	 * \param [in] allocator is a reference to ThreadControlBlockListAllocator object used to copy-construct allocator
	 * of base container
	 * \param [in] state is the state of ThreadControlBlock objects kept in this list
	 */

	ThreadControlBlockList(const ThreadControlBlockListAllocator& allocator, const ThreadControlBlock::State state) :
			Base{allocator},
			state_{state}
	{

	}

	/**
	 * \brief ThreadControlBlockList's destructor
	 *
	 * Clears list pointers in all elements.
	 */

	~ThreadControlBlockList()
	{
		for (auto& item : *this)
			item.get().setList(nullptr);
	}

	/**
	 * \brief Wrapper for sortedEmplace()
	 *
	 * Sets list pointer, iterator and state of emplaced element.
	 *
	 * \param Args are types of argument for value_type constructor
	 *
	 * \param [in] args are arguments for value_type constructor
	 *
	 * \return iterator to emplaced element
	 */

	template<typename... Args>
	iterator sortedEmplace(Args&&... args)
	{
		const auto it = Base::sortedEmplace(std::forward<Args>(args)...);
		auto& threadControlBlock = it->get();
		threadControlBlock.setList(this);
		threadControlBlock.setIterator(it);
		threadControlBlock.setState(state_);
		return it;
	}

	/**
	 * \brief Wrapper for sortedSplice()
	 *
	 * Sets list pointer and state of transfered element.
	 *
	 * \param [in] other is the container from which the object is transfered
	 * \param [in] otherPosition is the position of the transfered object in the other container
	 */

	void sortedSplice(ThreadControlBlockList& other, const iterator otherPosition)
	{
		Base::sortedSplice(other, otherPosition);
		otherPosition->get().setList(this);
		otherPosition->get().setState(state_);
	}

private:

	/// state of ThreadControlBlock objects kept in this list
	const ThreadControlBlock::State state_;
};

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_THREADCONTROLBLOCKLIST_HPP_
