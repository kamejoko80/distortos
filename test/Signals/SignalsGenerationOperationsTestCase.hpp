/**
 * \file
 * \brief SignalsGenerationOperationsTestCase class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-04-02
 */

#ifndef TEST_SIGNALS_SIGNALSGENERATIONOPERATIONSTESTCASE_HPP_
#define TEST_SIGNALS_SIGNALSGENERATIONOPERATIONSTESTCASE_HPP_

#include "TestCase.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Tests operations specific to generated signals.
 *
 * Generates the same signal multiple times for current thread. Because generated signals are not "queued", exactly one
 * call to ThisThread::Signals::tryWait() (or any other similar function) should be enough to clear the pending signal.
 */

class SignalsGenerationOperationsTestCase : public TestCase
{
private:

	/**
	 * \brief Runs the test case.
	 *
	 * \return true if the test case succeeded, false otherwise
	 */

	virtual bool run_() const override;
};

}	// namespace test

}	// namespace distortos

#endif	// TEST_SIGNALS_SIGNALSGENERATIONOPERATIONSTESTCASE_HPP_
