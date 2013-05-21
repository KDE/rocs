#! /usr/bin/env bash
$EXTRACTRC `find . -name \*.rc -o -name \*.ui -o -name \*.kcfg` >> rc.cpp
extractrc --context="Scripting API Description" --tag-group=none --tag=para `find DataStructures/ -name '*.xml'` >> rc.cpp
$XGETTEXT `find . -name \*.cpp` -o $podir/rocscore.pot
rm -f rc.cpp
