/**
 * \file
 * \brief signalsTestCases object definition
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-05
 */

#include "signalsTestCases.hpp"

#include "SignalsWaitTestCase.hpp"
#include "SignalsWaitOperationsTestCase.hpp"
#include "SignalsGenerationOperationsTestCase.hpp"
#include "SignalsQueuedOperationsTestCase.hpp"
#include "SignalsCatchingTestCase.hpp"

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// SignalsWaitTestCase::Implementation instance
const SignalsWaitTestCase::Implementation waitTestCaseImplementation;

/// SignalsWaitTestCase instance
const SignalsWaitTestCase waitTestCase {waitTestCaseImplementation};

/// SignalsWaitOperationsTestCase instance
const SignalsWaitOperationsTestCase waitOperationsTestCase;

/// SignalsGenerationOperationsTestCase instance
const SignalsGenerationOperationsTestCase generationOperationsTestCase;

/// SignalsQueuedOperationsTestCase instance
const SignalsQueuedOperationsTestCase queuedOperationsTestCase;

/// SignalsCatchingTestCase instance
const SignalsCatchingTestCase catchingTestCase;

/// array with references to TestCase objects related to signals
const TestCaseRange::value_type messageQueueTestCases_[]
{
		TestCaseRange::value_type{waitTestCase},
		TestCaseRange::value_type{waitOperationsTestCase},
		TestCaseRange::value_type{generationOperationsTestCase},
		TestCaseRange::value_type{queuedOperationsTestCase},
		TestCaseRange::value_type{catchingTestCase},
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const TestCaseRange signalsTestCases {messageQueueTestCases_};

}	// namespace test

}	// namespace distortos
