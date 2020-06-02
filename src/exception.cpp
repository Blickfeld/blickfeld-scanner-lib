/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */
#include "blickfeld/scanner-config.h"

#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif

#include <stdio.h>
#include <errno.h>

#include "blickfeld/string_utils.h"
#include "blickfeld/exception.h"

using namespace std;

namespace blickfeld {

errno_exception::errno_exception(const char* format, ...) {
	vsn_format(format, size, len, local_msg);
#ifdef HAVE_WINDOWS_H
	LPVOID lpMsgBuf;
	DWORD last_error = GetLastError();
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		last_error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );
	append_format_string(
		&local_msg, &size, &len,
		": %s (GetLastError(): %d)",
		lpMsgBuf, last_error);
	LocalFree(lpMsgBuf);
#else
	append_format_string(
		&local_msg, &size, &len,
		": %s (errno %d)", strerror(errno), errno);
#endif
	msg.assign(local_msg, len);
	free(local_msg);
}

errno_exception::errno_exception(int retval, const char* format, ...) {
	vsn_format(format, size, len, local_msg);
	append_format_string(&local_msg, &size, &len, ": %s (retval %d)", strerror(retval), retval);
	msg.assign(local_msg, size);
	free(local_msg);
}

str_exception::str_exception(const char* format, ...) {
	vsn_format(format, size, len, local_msg);
	msg.assign(local_msg, len);
	free(local_msg);
}

} // namespace blickfeld
