# Outline

This readme file currently covers the following topics (further topics will be
included in the future):

* [Building](#Building)
* [Licensing](#Licensing)

## Building

Rocs uses `CMake` as its build system. Please refer to the `CMake` output to see
the list of needed packages and versions.

## Licenses

Rocs follows the
[KDE Licensing policy](https://community.kde.org/Policies/Licensing_Policy).

### Source Files

 All
`*.cpp`, `*.h`, and `*.cmake` source files are licensed under one of the
following licenses:

- GPL-2
- GPL-2+
- LGPL-2.1
 
It is a medium term goal to provide all sources under inside `src/` as GPL-2+
and all sources in `libgraphtheory/` as LGPL-2+.

### Documentation

The documentation files (to be found at: `doc/`) are licensed under GFDL-NIV-1.2+.

### Test Files

For the automatic unit tests, special test files are provided and are, due to
their license holders, under non GPL-compatible licenses.

These test files are only processed while executing the unit tests and can be 
omitted if no unit tests or no unit tests for a specific plugin, respectively,
shall be executed:

- `libgraphtheory/fileformats/dot/autotests/testfiles/`: EPL-1.0
  For further information see 
  `libgraphtheory/fileformats/dot/autotests/testfiles/README`.
