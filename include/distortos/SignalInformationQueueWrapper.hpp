/**
 * \file
 * \brief SignalInformationQueueWrapper class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-03-26
 */

#ifndef INCLUDE_DISTORTOS_SIGNALINFORMATIONQUEUEWRAPPER_HPP_
#define INCLUDE_DISTORTOS_SIGNALINFORMATIONQUEUEWRAPPER_HPP_

#include "distortos/synchronization/SignalInformationQueue.hpp"

#include <array>

namespace distortos
{

namespace synchronization
{

class SignalsReceiverControlBlock;

}	// namespace synchronization

/// SignalInformationQueueWrapper class is a container for synchronization::SignalInformationQueue
class SignalInformationQueueWrapper
{
	friend class synchronization::SignalsReceiverControlBlock;

public:

	/// import Storage type alias from synchronization::SignalInformationQueue
	using Storage = synchronization::SignalInformationQueue::Storage;

	/**
	 * \brief SignalInformationQueueWrapper's constructor
	 *
	 * \param [in] storage is an array of Storage elements
	 * \param [in] maxElements is the number of elements in \a storage array
	 */

	SignalInformationQueueWrapper(Storage* storage, size_t maxElements) :
			signalInformationQueue_{storage, maxElements}
	{

	}

	/**
	 * \brief SignalInformationQueueWrapper's constructor
	 *
	 * \param N is the number of elements in \a storage array
	 *
	 * \param [in] storage is a reference to array of Storage elements
	 */

	template<size_t N>
	explicit SignalInformationQueueWrapper(Storage (& storage)[N]) :
			SignalInformationQueueWrapper{storage, sizeof(storage) / sizeof(*storage)}
	{

	}

	/**
	 * \brief SignalInformationQueueWrapper's constructor
	 *
	 * \param N is the number of elements in \a storage array
	 *
	 * \param [in] storage is a reference to std::array of Storage elements
	 */

	template<size_t N>
	explicit SignalInformationQueueWrapper(std::array<Storage, N>& storage) :
			SignalInformationQueueWrapper{storage.data(), storage.size()}
	{

	}

private:

	/// contained synchronization::SignalInformationQueue object
	synchronization::SignalInformationQueue signalInformationQueue_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SIGNALINFORMATIONQUEUEWRAPPER_HPP_
