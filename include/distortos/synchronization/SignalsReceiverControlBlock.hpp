/**
 * \file
 * \brief SignalsReceiverControlBlock class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-04-29
 */

#ifndef INCLUDE_DISTORTOS_SYNCHRONIZATION_SIGNALSRECEIVERCONTROLBLOCK_HPP_
#define INCLUDE_DISTORTOS_SYNCHRONIZATION_SIGNALSRECEIVERCONTROLBLOCK_HPP_

#include "distortos/SignalSet.hpp"

#include <cstdint>

union sigval;

namespace distortos
{

class SignalAction;
class SignalsCatcher;
class SignalInformation;
class SignalInformationQueueWrapper;

namespace scheduler
{

class ThreadControlBlock;

}	// namespace scheduler

namespace synchronization
{

class SignalsCatcherControlBlock;
class SignalInformationQueue;

/// SignalsReceiverControlBlock class is a structure required by threads for "receiving" of signals
class SignalsReceiverControlBlock
{
public:

	/**
	 * \brief SignalsReceiverControlBlock's constructor
	 *
	 * \param [in] signalInformationQueueWrapper is a pointer to SignalInformationQueueWrapper for this receiver,
	 * nullptr to disable queuing of signals for this receiver
	 * \param [in] signalsCatcher is a pointer to SignalsCatcher for this receiver, nullptr if this receiver cannot
	 * catch/handle signals
	 */

	explicit SignalsReceiverControlBlock(SignalInformationQueueWrapper* signalInformationQueueWrapper,
			SignalsCatcher* signalsCatcher);

	/**
	 * \brief Accepts (clears) one of signals that are pending.
	 *
	 * This should be called when the signal is "accepted".
	 *
	 * \param [in] signalNumber is the signal that will be accepted, [0; 31]
	 *
	 * \return pair with return code (0 on success, error code otherwise) and SignalInformation object for accepted
	 * signal; error codes:
	 * - EAGAIN - no signal specified by \a signalSet was pending;
	 * - EINVAL - \a signalNumber value is invalid;
	 */

	std::pair<int, SignalInformation> acceptPendingSignal(uint8_t signalNumber);

	/**
	 * \brief Generates signal for associated thread.
	 *
	 * Similar to pthread_kill() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_kill.html
	 *
	 * Adds the signalNumber to set of pending signals. If associated thread is currently waiting for this signal, it
	 * will be unblocked.
	 *
	 * \param [in] signalNumber is the signal that will be generated, [0; 31]
	 * \param [in] threadControlBlock is a reference to associated ThreadControlBlock
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - \a signalNumber value is invalid;
	 */

	int generateSignal(uint8_t signalNumber, const scheduler::ThreadControlBlock& threadControlBlock);

	/**
	 * \return set of currently pending signals
	 */

	SignalSet getPendingSignalSet() const;

	/**
	 * \brief Gets SignalAction associated with given signal number.
	 *
	 * Similar to sigaction() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sigaction.html
	 *
	 * \param [in] signalNumber is the signal for which the association is requested, [0; 31]
	 *
	 * \return pair with return code (0 on success, error code otherwise) and SignalAction that is associated with
	 * \a signalNumber, default-constructed object if no association was found;
	 * error codes:
	 * - EINVAL - \a signalNumber value is invalid;
	 * - ENOTSUP - catching/handling of signals is disabled for this receiver;
	 */

	std::pair<int, SignalAction> getSignalAction(uint8_t signalNumber) const;

	/**
	 * \brief Gets signal mask for associated thread.
	 *
	 * Similar to pthread_sigmask() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_sigmask.html#
	 * 
	 * \return SignalSet with signal mask for associated thread
	 */

	SignalSet getSignalMask() const;

	/**
	 * \brief Queues signal for associated thread.
	 *
	 * Similar to sigqueue() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sigqueue.html
	 *
	 * Queues the signalNumber and signal value (sigval union) in associated SignalInformationQueue object. If
	 * associated thread is currently waiting for this signal, it will be unblocked.
	 *
	 * \param [in] signalNumber is the signal that will be queued, [0; 31]
	 * \param [in] value is the signal value
	 * \param [in] threadControlBlock is a reference to associated ThreadControlBlock
	 *
	 * \return 0 on success, error code otherwise:
	 * - EAGAIN - no resources are available to queue the signal, maximal number of signals is already queued in
	 * associated SignalInformationQueue object;
	 * - EINVAL - \a signalNumber value is invalid;
	 * - ENOTSUP - queuing of signals is disabled for this receiver;
	 */

	int queueSignal(uint8_t signalNumber, sigval value, const scheduler::ThreadControlBlock& threadControlBlock) const;

	/**
	 * \brief Sets association for given signal number.
	 *
	 * Similar to sigaction() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sigaction.html
	 *
	 * \param [in] signalNumber is the signal for which the association will be set, [0; 31]
	 * \param [in] signalAction is a reference to SignalAction that will be associated with given signal number, object
	 * in internal storage is copy-constructed
	 *
	 * \return pair with return code (0 on success, error code otherwise) and SignalAction that was associated with
	 * \a signalNumber, default-constructed object if no association was found;
	 * error codes:
	 * - EAGAIN - no resources are available to associate \a signalNumber with \a signalAction;
	 * - EINVAL - \a signalNumber value is invalid;
	 * - ENOTSUP - catching/handling of signals is disabled for this receiver;
	 */

	std::pair<int, SignalAction> setSignalAction(uint8_t signalNumber, const SignalAction& signalAction) const;

	/**
	 * \brief Sets signal mask for associated thread.
	 *
	 * Similar to pthread_sigmask() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_sigmask.html#
	 *
	 * \param [in] signalMask is the SignalSet with new signal mask for associated thread
	 * 
	 * \return 0 on success, error code otherwise:
	 * - ENOTSUP - catching/handling of signals is disabled for this receiver;
	 */

	int setSignalMask(SignalSet signalMask);

	/**
	 * \param [in] signalSet is a pointer to set of signals that will be "waited for", nullptr when wait was terminated
	 */

	void setWaitingSignalSet(const SignalSet* const signalSet)
	{
		waitingSignalSet_ = signalSet;
	}

private:

	/**
	 * \brief Actions executed after signal is "generated" with generateSignal() or queueSignal().
	 *
	 * If associated thread is currently waiting for the signal that was generated, it will be unblocked.
	 *
	 * \param [in] signalNumber is the signal that was generated, [0; 31]
	 * \param [in] threadControlBlock is a reference to associated ThreadControlBlock
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - \a signalNumber value is invalid;
	 */

	int postGenerate(uint8_t signalNumber, const scheduler::ThreadControlBlock& threadControlBlock) const;

	/// set of pending signals
	SignalSet pendingSignalSet_;

	/// pointer to set of "waited for" signals, nullptr if associated thread is not waiting for any signals
	const SignalSet* waitingSignalSet_;

	/// pointer to SignalsCatcherControlBlock for this receiver, nullptr if this receiver cannot catch/handle signals
	SignalsCatcherControlBlock* signalsCatcherControlBlock_;

	/// pointer to SignalInformationQueue for this receiver, nullptr if this receiver cannot queue signals
	SignalInformationQueue* signalInformationQueue_;
};

}	// namespace synchronization

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SYNCHRONIZATION_SIGNALSRECEIVERCONTROLBLOCK_HPP_
