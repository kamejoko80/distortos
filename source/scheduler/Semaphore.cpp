/**
 * \file
 * \brief Semaphore class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-04
 */

#include "distortos/scheduler/Semaphore.hpp"

#include "distortos/scheduler/schedulerInstance.hpp"
#include "distortos/scheduler/Scheduler.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

#include <cerrno>

namespace distortos
{

namespace scheduler
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

Semaphore::Semaphore(int value) :
		blockedList_{ThreadControlBlock::State::BlockedOnSemaphore},
		value_{value}
{

}

Semaphore::~Semaphore()
{

}

void Semaphore::unlock()
{
	architecture::InterruptMaskingLock interrupt_masking_lock;

	++value_;

	if (blockedList_.empty() == false)
		schedulerInstance.unblock(blockedList_, blockedList_.begin());
}

int Semaphore::lock()
{
	architecture::InterruptMaskingLock interrupt_masking_lock;

	--value_;

	if (value_ < 0)
		schedulerInstance.block(blockedList_);

	return 0;
}

int Semaphore::tryLock()
{
	architecture::InterruptMaskingLock interrupt_masking_lock;

	if (value_ > 0)	// lock possible?
	{
		--value_;
		return 0;
	}

	return EAGAIN;
}

}	// namespace scheduler

}	// namespace distortos