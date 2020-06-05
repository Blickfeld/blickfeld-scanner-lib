#!/usr/bin/python
#
# Copyright (c) 2020 Blickfeld GmbH.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE.md file in the root directory of this source tree.
#

from __future__ import print_function

import blickfeld_scanner

def test_record_point_cloud_dump(ip_or_hostname="localhost"):
    scanner = blickfeld_scanner.scanner(ip_or_hostname)
    stream = scanner.get_point_cloud_stream()
    print(stream.get_metadata())

def test_read_point_cloud_dump():
    fn = "../../tests/assets/dump.bfpc"
    
    stream = blickfeld_scanner.scanner.file_point_cloud_stream(fn)
    stream.record_to_file("dump_simulate.bfpc")
    
    while not stream.end_of_stream():
        print("Read and recorded", stream.recv_frame())
        
    del stream
    
    stream = blickfeld_scanner.scanner.file_point_cloud_stream("dump_simulate.bfpc")
    
    while not stream.end_of_stream():
        print("Read recorded", stream.recv_frame())
        
    del stream
        
    
    