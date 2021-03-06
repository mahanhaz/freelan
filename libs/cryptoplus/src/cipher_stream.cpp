/*
 * libcryptoplus - C++ portable OpenSSL cryptographic wrapper library.
 * Copyright (C) 2010-2011 Julien Kauffmann <julien.kauffmann@freelan.org>
 *
 * This file is part of libcryptoplus.
 *
 * libcryptoplus is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * libcryptoplus is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 *
 * If you intend to use libcryptoplus in a commercial software, please
 * contact me : we may arrange this for a small fee or no fee at all,
 * depending on the nature of your project.
 */

/**
 * \file cipher_stream.cpp
 * \author Julien KAUFFMANN <julien.kauffmann@freelan.org>
 * \brief A cipher stream class.
 */

#include "cipher/cipher_stream.hpp"

#include <cassert>

namespace cryptoplus
{
	namespace cipher
	{
		cipher_stream& cipher_stream::append(const void* buf, size_t buf_len)
		{
			size_t out_len = buffer_size(m_buffer) - m_offset;

			if (out_len < algorithm().block_size() + buf_len)
			{
				m_buffer.data().resize(buffer_size(m_buffer) + algorithm().block_size() + buf_len - out_len);
				out_len = algorithm().block_size() + buf_len;
			}

			m_offset += update(buffer_cast<uint8_t*>(m_buffer) + m_offset, out_len, buf, buf_len);

			return *this;
		}

		void cipher_stream::finalize()
		{
			size_t out_len = buffer_size(m_buffer) - m_offset;

			if (out_len < algorithm().block_size())
			{
				m_buffer.data().resize(buffer_size(m_buffer) + algorithm().block_size() - out_len);
				out_len = algorithm().block_size();
			}

			m_buffer.data().resize(m_offset + finalize(buffer_cast<uint8_t*>(m_buffer) + m_offset, out_len));

			m_offset = 0;
		}
	}
}

