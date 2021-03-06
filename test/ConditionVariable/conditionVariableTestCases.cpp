/**
 * \file
 * \brief conditionVariableTestCases object definition
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-24
 */

#include "conditionVariableTestCases.hpp"

#include "ConditionVariablePriorityTestCase.hpp"
#include "ConditionVariableOperationsTestCase.hpp"

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// ConditionVariablePriorityTestCase::Implementation instance
const ConditionVariablePriorityTestCase::Implementation priorityTestCaseImplementation;

/// ConditionVariablePriorityTestCase instance
const ConditionVariablePriorityTestCase priorityTestCase {priorityTestCaseImplementation};

/// ConditionVariableOperationsTestCase instance
const ConditionVariableOperationsTestCase operationsTestCase;

/// array with references to TestCase objects related to condition variables
const TestCaseRange::value_type conditionVariableTestCases_[]
{
		TestCaseRange::value_type{priorityTestCase},
		TestCaseRange::value_type{operationsTestCase},
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const TestCaseRange conditionVariableTestCases {conditionVariableTestCases_};

}	// namespace test

}	// namespace distortos
