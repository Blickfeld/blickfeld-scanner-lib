#!/usr/bin/env python3
#
# Copyright (c) 2020 Blickfeld GmbH.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE.md file in the root directory of this source tree.
#

import sys
import os
import re

dir = sys.argv[1].rstrip(os.sep)

def fix_line(pattern, line, level):

    if re.match(pattern, line):
        line = line.replace("blickfeld ", ("." * level) + " ").replace("blickfeld.", "." * level)

    return line

def fix_file(pattern, source, level):
    fin = open(source, 'r')
    content = fin.readlines()
    fin.close()

    out = [fix_line(pattern, line, level) for line in content] 
    
    fin = open(source, 'w')
    fin.write("".join(out)) 
    fin.close()

for root, subdirs, files in os.walk(sys.argv[1]):
    level = len(root[len(dir):].split(os.sep))
    
    for file in files:
        if file.endswith("_pb2.py"):
            fix_file(r"^from blickfeld.*_pb2 ", os.path.join(root, file), level)
    
