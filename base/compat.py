from __future__ import absolute_import
from __future__ import division

import numbers as _numbers
import numpy as _np
import six as _six

def as_bytes(bytes_or_text, encoding='utf-8'):

    if isinstance(bytes_or_text, _six.text_type):
        return bytes_or_text.encode(encoding)
    elif isinstance(bytes_or_text, bytes):
        return bytes_or_text
    else:
        raise TypeError('Expected binary or unicode string, got %r' %
                    (bytes_or_text,))

def as_text(bytes_or_text, encoding='utf-8'):
    if isinstance(bytes_or_text, _six.text_type):
        return bytes_or_text
    elif isinstance(bytes_or_text, bytes):
        return bytes_or_text.decode(encoding)
    else:
        raise TypeError('Expected binary or unicode string, got %r' % bytes_or_text)

# Convert an object to a `str` in both Python 2 and 3.
if _six.PY2:
    as_str = as_bytes
else:
    as_str = as_text

def as_str_any(value):
    if isinstance(value, bytes):
        return as_str(value)
    else:
        return str(value)


integral_types = (_numbers.Integral, _np.integer)
real_types = (_numbers.Real, _np.integer, _np.floating)
complex_types = (_numbers.Complex, _np.number)


# Either bytes or text.
bytes_or_text_types = (bytes, _six.text_type)


_allowed_symbols = [
    'as_str',
    'bytes_or_text_types',
    'complex_types',
    'integral_types',
    'real_types',
]

remove_undocumented(__name__, _allowed_symbols)
