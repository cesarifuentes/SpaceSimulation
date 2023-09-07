#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/cesarfuentes/Documents/school/Animation/A6/build
  make -f /Users/cesarfuentes/Documents/school/Animation/A6/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/cesarfuentes/Documents/school/Animation/A6/build
  make -f /Users/cesarfuentes/Documents/school/Animation/A6/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/cesarfuentes/Documents/school/Animation/A6/build
  make -f /Users/cesarfuentes/Documents/school/Animation/A6/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/cesarfuentes/Documents/school/Animation/A6/build
  make -f /Users/cesarfuentes/Documents/school/Animation/A6/build/CMakeScripts/ReRunCMake.make
fi

