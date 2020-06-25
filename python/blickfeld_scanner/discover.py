#!/usr/bin/python
#
# Copyright (c) 2020 Blickfeld GmbH.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE.md file in the root directory of this source tree.
#

from __future__ import print_function

try:
    from zeroconf import ServiceBrowser, Zeroconf, InterfaceChoice
except:
    Zeroconf = None

import time

__all__ = [
    "search_and_connect",
    "search_one_and_connect"
]

from .scanner import scanner

def __discover(duration):
    infos = []

    class ServiceListener:
        def remove_service(self, zeroconf, type, name):
            pass

        def add_service(self, zeroconf, type, name):
            info = zeroconf.get_service_info(type, name)
            infos.append(info)
            zeroconf.notify_all()
            
    if Zeroconf is None:
        raise Exception("Either not supported with this python version or package `zeroconf` is broken.")

    zeroconf = Zeroconf(interfaces=InterfaceChoice.All)
    ServiceBrowser(zeroconf, "_blickfeld-lidar._tcp.local.", ServiceListener())
    
    while duration > 0:
        start = time.time()
        zeroconf.wait(duration)
        duration = duration - (time.time() - start)
        
    zeroconf.close()
    
    return infos

def search_and_connect(duration = 3):
    """ Find Blickfeld devices in the network and connect to them.

    :param duration: duration of network search in seconds
    :return: list of BSL scanner objects, [:py:class:`blickfeld_scanner.scanner`]
    """
    scanners = []
    for info in __discover(duration):
        try:
            scanners.append(scanner(info.parsed_addresses()[0], name=info.server))
        except:
            pass
    return scanners
        
def search_one_and_connect(duration = 3):
    """ Find Blickfeld device in the network and connect to it, 
    if multiple devices are available, the first device discovered will be selected.

    :param duration: duration of network search in seconds
    :return: :py:class:`blickfeld_scanner.scanner` object or None if no device is found, 
    """
    infos = __discover(duration)
    if not len(infos):
        return None
    return scanner(infos[0].parsed_addresses()[0], name=infos[0].server)
