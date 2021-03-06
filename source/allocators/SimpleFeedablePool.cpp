/**
 * \file
 * \brief SimpleFeedablePool class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-09-07
 */

#include "distortos/allocators/SimpleFeedablePool.hpp"

#include <cstdlib>

namespace distortos
{

namespace allocators
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

void* SimpleFeedablePool::allocate(const size_t size)
{
	if (size > size_)
		abort();

	const auto storage = storage_;

	storage_ = nullptr;
	size_ = 0;

	return storage;
}

}	// namespace allocators

}	// namespace distortos
