# -*- coding: utf-8 -*-
#
# Python interface to posix_fadvise()
# Copyright (C) Chris Lamb <chris@chris-lamb.co.uk>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. Neither the name of the University nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.

"""
Python interface to posix_fadvise()

Programs can use posix_fadvise() to announce an intention to access
file data in a specific pattern in the future, thus allowing the kernel
to perform appropriate optimizations.

The advice applies to a (not necessarily existent) region starting at
offset and extending for len bytes (or until the end of the file if len
is 0) within the file referred to by fd. The advice is not binding; it
merely constitutes an expectation on behalf of the application.
"""

import os

from fadvise._fadvise import posix_fadvise, POSIX_FADV_NORMAL, POSIX_FADV_RANDOM, \
 POSIX_FADV_SEQUENTIAL, POSIX_FADV_WILLNEED, POSIX_FADV_DONTNEED, \
 POSIX_FADV_NOREUSE

def set_advice(fileobj, advice, offset=0, len=0):
    return posix_fadvise(fileobj.fileno(), offset, len, advice)

def noreuse(*args, **kwargs):
    """
    The specified files will be accessed only once. Under Linux, this operation
    is a no-op; see contrib/copyfileobj-fadvise.py in the python-fadvise source
    tree for an example on how to achieve approximately the same effect.

    Example::

        with fadvise.noreuse('file.test') as f:
            f.read()
    """

    return _open_wrapper(POSIX_FADV_NOREUSE, *args, **kwargs)

def normal(*args, **kwargs):
    """
    Indicates that the application has no advice to give about its
    access pattern for the specified files. If no advice is given
    for an open file, this is the default assumption.
    """
    return _open_wrapper(POSIX_FADV_NORMAL, *args, **kwargs)

def sequential(*args, **kwargs):
    """
    The application expects to access the specified files
    sequentially (with lower offsets read before higher ones).
    """
    return _open_wrapper(POSIX_FADV_SEQUENTIAL, *args, **kwargs)

def random(*args, **kwargs):
    """
    The specified files will be accessed in random order.
    """
    return _open_wrapper(POSIX_FADV_RANDOM, *args, **kwargs)

def willneed(*args, **kwargs):
    """
    The specified files will be accessed in the near future.
    """
    return _open_wrapper(POSIX_FADV_WILLNEED, *args, **kwargs)

def dontneed(*args, **kwargs):
    """
    The specified files will not be accessed in the near future.
    """
    return _open_wrapper(POSIX_FADV_DONTNEED, *args, **kwargs)

##

def _open_wrapper(advice, *args, **kwargs):
    len = kwargs.pop('len', 0)
    offset = kwargs.pop('offset', 0)

    fileobj = open(*args, **kwargs)

    set_advice(fileobj, advice, offset=offset, len=len)

    return fileobj
