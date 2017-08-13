# Copyright 2017 The ChaosBase Authors. All Rights Reserved.
from __future__ import absolute_import
from __future__ import

import inspect as _inspect
import re as _re
import sys as _sys

def make_all(module_name, doc_string_modules=None):
    """Generates `__all__` from the docstring of one or more modules.
    Args:
        module_name: The name of the module (usually `__name__`).
        doc_string_modules: a list of modules from which to take docstring.
        If None, then a list containing only the module named `module_name` is used.
    Returns:
    A list suitable for use as `__all__`.
    """
    if doc_string_modules is None:
        doc_string_modules = [_sys.modules[module_name]]
    cur_members = set([name for name, _
                        in _inspect.getmembers(_sys.modules[module_name])])

    results = set()
    for doc_module in doc_string_modules:
        results.update([m.group(1)
                        for m in _reference_pattern.finditer(doc_module.__doc__)
                        if m.group(1) in cur_members])
    return list(results)

_HIDDEN_ATTRIBUTES = {}

def remove_undocumented(module_name, allowed_exception_list=None,
                        doc_string_modules=None):
    """Removes symbols in a module that are not referenced by a docstring.
    Returns:
        None
    """
    current_symbols = set(dir(_sys.modules[module_name]))
    should_have = make_all(module_name, doc_string_modules)
    should_have += allowed_exception_list or []
    extra_symbols = current_symbols - set(should_have)
    target_module = _sys.modules[module_name]
    for extra_symbol in extra_symbols:
    # Skip over __file__, etc. Also preserves internal symbols.
        if extra_symbol.startswith('_'): continue
        fully_qualified_name = module_name + '.' + extra_symbol
        _HIDDEN_ATTRIBUTES[fully_qualified_name] = (target_module,
                                                    getattr(target_module,
                                                            extra_symbol))
        delattr(target_module, extra_symbol)

__all__ = [
    'make_all',
    'remove_undocumented'
]
