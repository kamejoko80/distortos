/**
 * \file
 * \brief SignalsReceiverControlBlock class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-04-29
 */

#include "distortos/synchronization/SignalsReceiverControlBlock.hpp"

#include "distortos/scheduler/getScheduler.hpp"
#include "distortos/scheduler/Scheduler.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

#include "distortos/SignalsCatcher.hpp"
#include "distortos/SignalInformationQueueWrapper.hpp"

#include <cerrno>

namespace distortos
{

namespace synchronization
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

SignalsReceiverControlBlock::SignalsReceiverControlBlock(SignalInformationQueueWrapper* const
		signalInformationQueueWrapper, SignalsCatcher* const signalsCatcher) :
		pendingSignalSet_{SignalSet::empty},
		waitingSignalSet_{},
		signalsCatcherControlBlock_{signalsCatcher != nullptr ? &signalsCatcher->signalsCatcherControlBlock_ : nullptr},
		signalInformationQueue_
		{
				signalInformationQueueWrapper != nullptr ? &signalInformationQueueWrapper->signalInformationQueue_ :
						nullptr
		}
{

}

std::pair<int, SignalInformation> SignalsReceiverControlBlock::acceptPendingSignal(const uint8_t signalNumber)
{
	if (signalInformationQueue_ != nullptr)
	{
		const auto acceptQueuedSignalResult = signalInformationQueue_->acceptQueuedSignal(signalNumber);
		if (acceptQueuedSignalResult.first != EAGAIN)	// signal was accepted or fatal error?
			return acceptQueuedSignalResult;
	}

	const auto testResult = pendingSignalSet_.test(signalNumber);
	if (testResult.first != 0)
		return {testResult.first, SignalInformation{uint8_t{}, SignalInformation::Code{}, sigval{}}};
	if (testResult.second == false)
		return {EAGAIN, SignalInformation{uint8_t{}, SignalInformation::Code{}, sigval{}}};
	const auto ret = pendingSignalSet_.remove(signalNumber);
	return {ret, SignalInformation{signalNumber, SignalInformation::Code::Generated, sigval{}}};
}

int SignalsReceiverControlBlock::generateSignal(const uint8_t signalNumber,
		const scheduler::ThreadControlBlock& threadControlBlock)
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	const auto ret = pendingSignalSet_.add(signalNumber);
	if (ret != 0)
		return ret;

	return postGenerate(signalNumber, threadControlBlock);
}

SignalSet SignalsReceiverControlBlock::getPendingSignalSet() const
{
	const auto pendingSignalSet = pendingSignalSet_;
	if (signalInformationQueue_ == nullptr)
		return pendingSignalSet;

	const auto queuedSignalSet = signalInformationQueue_->getQueuedSignalSet();
	return SignalSet{pendingSignalSet.getBitset() | queuedSignalSet.getBitset()};
}

std::pair<int, SignalAction> SignalsReceiverControlBlock::getSignalAction(const uint8_t signalNumber) const
{
	if (signalsCatcherControlBlock_ == nullptr)
		return {ENOTSUP, {}};

	return signalsCatcherControlBlock_->getAssociation(signalNumber);
}

SignalSet SignalsReceiverControlBlock::getSignalMask() const
{
	if (signalsCatcherControlBlock_ == nullptr)
		return SignalSet{SignalSet::full};

	return signalsCatcherControlBlock_->getSignalMask();
}

int SignalsReceiverControlBlock::queueSignal(const uint8_t signalNumber, const sigval value,
		const scheduler::ThreadControlBlock& threadControlBlock) const
{
	if (signalInformationQueue_ == nullptr)
		return ENOTSUP;

	architecture::InterruptMaskingLock interruptMaskingLock;

	const auto ret = signalInformationQueue_->queueSignal(signalNumber, value);
	if (ret != 0)
		return ret;

	return postGenerate(signalNumber, threadControlBlock);
}

std::pair<int, SignalAction> SignalsReceiverControlBlock::setSignalAction(const uint8_t signalNumber,
		const SignalAction& signalAction) const
{
	if (signalsCatcherControlBlock_ == nullptr)
		return {ENOTSUP, {}};

	return signalsCatcherControlBlock_->setAssociation(signalNumber, signalAction);
}

int SignalsReceiverControlBlock::setSignalMask(const SignalSet signalMask)
{
	if (signalsCatcherControlBlock_ == nullptr)
		return ENOTSUP;

	signalsCatcherControlBlock_->setSignalMask(signalMask);
	return 0;
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

int SignalsReceiverControlBlock::postGenerate(const uint8_t signalNumber,
		const scheduler::ThreadControlBlock& threadControlBlock) const
{
	/// \todo add some form of assertion for validity of \a signalNumber

	if (signalsCatcherControlBlock_ != nullptr)
	{
		const auto signalMask = signalsCatcherControlBlock_->getSignalMask();
		const auto testResult = signalMask.test(signalNumber);
		if (testResult.second == false)	// signal is not masked?
			return signalsCatcherControlBlock_->postGenerate(signalNumber, threadControlBlock);
	}

	if (waitingSignalSet_ == nullptr)
		return 0;

	const auto testResult = waitingSignalSet_->test(signalNumber);
	if (testResult.second == false)	// signalNumber is not "waited for"?
		return 0;

	scheduler::getScheduler().unblock(threadControlBlock.getIterator());
	return 0;
}

}	// namespace synchronization

}	// namespace distortos
