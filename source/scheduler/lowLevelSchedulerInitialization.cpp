/**
 * \file
 * \brief lowLevelSchedulerInitialization() definition
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-04-16
 */

#include "distortos/scheduler/lowLevelSchedulerInitialization.hpp"

#include "distortos/StaticThread.hpp"

#include "distortos/scheduler/getScheduler.hpp"
#include "distortos/scheduler/Scheduler.hpp"
#include "distortos/scheduler/idleThreadFunction.hpp"
#include "distortos/scheduler/MainThread.hpp"
#include "distortos/scheduler/ThreadGroupControlBlock.hpp"

namespace distortos
{

namespace scheduler
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// size of idle thread's stack, bytes
constexpr size_t idleThreadStackSize {128};

/// type of idle thread
using IdleThread = decltype(makeStaticThread<idleThreadStackSize>(0, idleThreadFunction));

/// storage for idle thread instance
std::aligned_storage<sizeof(IdleThread), alignof(IdleThread)>::type idleThreadStorage;

/// storage for main thread instance
std::aligned_storage<sizeof(MainThread), alignof(MainThread)>::type mainThreadStorage;

/// storage for main thread group instance
std::aligned_storage<sizeof(ThreadGroupControlBlock), alignof(ThreadGroupControlBlock)>::type
		mainThreadGroupControlBlockStorage;

#if CONFIG_MAIN_THREAD_CAN_RECEIVE_SIGNALS == 1

/// type of StaticSignalsReceiver for main thread
using MainThreadStaticSignalsReceiver =
		StaticSignalsReceiver<CONFIG_MAIN_THREAD_QUEUED_SIGNALS, CONFIG_MAIN_THREAD_CAUGHT_SIGNALS>;

/// MainThreadStaticSignalsReceiver object for main thread
MainThreadStaticSignalsReceiver mainThreadStaticSignalsReceiver;

/// pointer to \a mainThreadStaticSignalsReceiver
constexpr auto mainThreadStaticSignalsReceiverPointer = &mainThreadStaticSignalsReceiver;

#else

/// nullptr - reception of signals is disabled for main thread
constexpr auto mainThreadStaticSignalsReceiverPointer = nullptr;

#endif	// CONFIG_MAIN_THREAD_CAN_RECEIVE_SIGNALS == 1

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void lowLevelSchedulerInitialization()
{
	auto& schedulerInstance = getScheduler();
	new (&schedulerInstance) Scheduler;

	auto& mainThreadGroupControlBlock = *new (&mainThreadGroupControlBlockStorage) ThreadGroupControlBlock;

	auto& mainThread = *new (&mainThreadStorage) MainThread {UINT8_MAX, mainThreadGroupControlBlock,
			mainThreadStaticSignalsReceiverPointer};
	schedulerInstance.initialize(mainThread);	/// \todo error handling?
	mainThread.getThreadControlBlock().switchedToHook();

	auto& idleThread = *new (&idleThreadStorage) IdleThread {0, idleThreadFunction};
	idleThread.start();
}

}	// namespace scheduler

}	// namespace distortos
