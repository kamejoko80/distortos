/**
 * \file
 * \brief FifoQueueBase class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-01-19
 */

#ifndef INCLUDE_DISTORTOS_SYNCHRONIZATION_FIFOQUEUEBASE_HPP_
#define INCLUDE_DISTORTOS_SYNCHRONIZATION_FIFOQUEUEBASE_HPP_

#include "distortos/Semaphore.hpp"

#include "distortos/synchronization/QueueFunctor.hpp"
#include "distortos/synchronization/SemaphoreFunctor.hpp"

namespace distortos
{

namespace synchronization
{

/// FifoQueueBase class implements basic functionality of FifoQueue template class
class FifoQueueBase
{
public:

	/**
	 * \brief FifoQueueBase's constructor
	 *
	 * \param [in] storageBegin is the beginning of storage for queue elements
	 * \param [in] storageEnd is the pointer to past-the-last element of storage for queue elements
	 * \param [in] elementSize is the size of single queue element, bytes
	 * \param [in] maxElements is the number of elements in storage
	 */

	FifoQueueBase(void* storageBegin, const void* storageEnd, size_t elementSize, size_t maxElements);

	/**
	 * \return size of single queue element, bytes
	 */

	size_t getElementSize() const
	{
		return elementSize_;
	}

	/**
	 * \brief Implementation of pop() using type-erased functor
	 *
	 * \param [in] waitSemaphoreFunctor is a reference to SemaphoreFunctor which will be executed with \a popSemaphore_
	 * \param [in] functor is a reference to QueueFunctor which will execute actions related to popping - it will get
	 * readPosition_ as argument
	 *
	 * \return zero if element was popped successfully, error code otherwise:
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int pop(const SemaphoreFunctor& waitSemaphoreFunctor, const QueueFunctor& functor)
	{
		return popPush(waitSemaphoreFunctor, functor, popSemaphore_, pushSemaphore_, readPosition_);
	}

	/**
	 * \brief Implementation of push() using type-erased functor
	 *
	 * \param [in] waitSemaphoreFunctor is a reference to SemaphoreFunctor which will be executed with \a pushSemaphore_
	 * \param [in] functor is a reference to QueueFunctor which will execute actions related to pushing - it will get
	 * writePosition_ as argument
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int push(const SemaphoreFunctor& waitSemaphoreFunctor, const QueueFunctor& functor)
	{
		return popPush(waitSemaphoreFunctor, functor, pushSemaphore_, popSemaphore_, writePosition_);
	}

private:

	/**
	 * \brief Implementation of pop() and push() using type-erased functor
	 *
	 * \param [in] waitSemaphoreFunctor is a reference to SemaphoreFunctor which will be executed with \a waitSemaphore
	 * \param [in] functor is a reference to QueueFunctor which will execute actions related to popping/pushing - it
	 * will get \a storage as argument
	 * \param [in] waitSemaphore is a reference to semaphore that will be waited for, \a popSemaphore_ for pop(), \a
	 * pushSemaphore_ for push()
	 * \param [in] postSemaphore is a reference to semaphore that will be posted after the operation, \a pushSemaphore_
	 * for pop(), \a popSemaphore_ for push()
	 * \param [in] storage is a reference to appropriate pointer to storage, which will be passed to \a functor, \a
	 * readPosition_ for pop(), \a writePosition_ for push()
	 *
	 * \return zero if operation was successful, error code otherwise:
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int popPush(const SemaphoreFunctor& waitSemaphoreFunctor, const QueueFunctor& functor, Semaphore& waitSemaphore,
			Semaphore& postSemaphore, void*& storage);

	/// semaphore guarding access to "pop" functions - its value is equal to the number of available elements
	Semaphore popSemaphore_;

	/// semaphore guarding access to "push" functions - its value is equal to the number of free slots
	Semaphore pushSemaphore_;

	/// beginning of storage for queue elements
	void* const storageBegin_;

	/// pointer to past-the-last element of storage for queue elements
	const void* const storageEnd_;

	/// pointer to first element available for reading
	void* readPosition_;

	/// pointer to first free slot available for writing
	void* writePosition_;

	/// size of single queue element, bytes
	const size_t elementSize_;
};

}	// namespace synchronization

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SYNCHRONIZATION_FIFOQUEUEBASE_HPP_
