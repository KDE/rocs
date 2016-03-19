#! /usr/bin/env bash
$EXTRACTRC `find . -name \*.ui` >> rc.cpp
$EXTRACTRC --context="Scripting API" --tag-group=none --tag=para --tag=info `find kernel/modules/* -iname '*.xml'` >> rc.cpp
$XGETTEXT `find . -name \*.cpp -o -name \*.qml` -o $podir/libgraphtheory.pot
rm -f rc.cpp
