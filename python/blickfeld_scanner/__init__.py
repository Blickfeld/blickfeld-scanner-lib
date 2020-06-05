#!/usr/bin/python
#
# Copyright (c) 2020 Blickfeld GmbH.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE.md file in the root directory of this source tree.
#

from __future__ import print_function

__all__ = [
    "__version__",
    "scanner",
    "discover"
]

from .version import __version__
from . import scanner as bf_scanner
from . import discover

scanner = bf_scanner.scanner

