/**
 * \file
 * \brief RawMessageQueuePriorityTestCase class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-01-19
 */

#ifndef TEST_RAWMESSAGEQUEUE_RAWMESSAGEQUEUEPRIORITYTESTCASE_HPP_
#define TEST_RAWMESSAGEQUEUE_RAWMESSAGEQUEUEPRIORITYTESTCASE_HPP_

#include "PrioritizedTestCase.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Tests priority scheduling of raw message queue.
 *
 * Starts 10 small threads (in various order) with varying priorities which wait either for a message from the raw
 * message queue or for free space in the raw message queue, asserting that they start and finish in the expected order,
 * using exact number of context switches, that the data received from the rwa message queue matches what was
 * transferred and is delivered in order which respects the priority of individual messages.
 */

class RawMessageQueuePriorityTestCase : public PrioritizedTestCase
{
	/// priority at which this test case should be executed
	constexpr static uint8_t testCasePriority_ {1};

public:

	/// internal implementation of RawMessageQueuePriorityTestCase
	class Implementation : public TestCase
	{
	private:

		/**
		 * \brief Runs the test case.
		 *
		 * \return true if the test case succeeded, false otherwise
		 */

		virtual bool run_() const override;
	};

	/**
	 * \brief RawMessageQueuePriorityTestCase's constructor
	 *
	 * \param [in] implementation is a reference to RawMessageQueuePriorityTestCase::Implementation object used by this
	 * instance
	 */

	constexpr explicit RawMessageQueuePriorityTestCase(const Implementation& implementation) :
			PrioritizedTestCase{implementation, testCasePriority_}
	{

	}
};

}	// namespace test

}	// namespace distortos

#endif	// TEST_RAWMESSAGEQUEUE_RAWMESSAGEQUEUEPRIORITYTESTCASE_HPP_
