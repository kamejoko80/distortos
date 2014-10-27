/**
 * \file
 * \brief mutexTestTryLockWhenLocked() header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-10-27
 */

#ifndef TEST_MUTEX_MUTEXTESTTRYLOCKWHENLOCKED_HPP_
#define TEST_MUTEX_MUTEXTESTTRYLOCKWHENLOCKED_HPP_

namespace distortos
{

class Mutex;

namespace test
{

/**
 * \brief Tests Mutex::tryLock() when mutex is locked - it must succeed immediately and return EBUSY
 *
 * Mutex::tryLock() is called from another thread.
 *
 * \param [in] mutex is a reference to mutex that will be tested
 *
 * \return true if test succeeded, false otherwise
 */

bool mutexTestTryLockWhenLocked(Mutex& mutex);

}	// namespace test

}	// namespace distortos

#endif	// TEST_MUTEX_MUTEXTESTTRYLOCKWHENLOCKED_HPP_