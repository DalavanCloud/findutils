#! /bin/sh
#
# import-gnulib.sh -- imports a copy of gnulib into findutils
# Copyright (C) 2003,2004 Free Software Foundation, Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
# USA.
#
##########################################################################
#
# This script is intended to populate the "gnulib" directory 
# with a subset of the gnulib code, as provided by "gnulib-tool".
#
# To use it, run this script, speficying the location of the 
# gnulib code as the only argument.   Some sanity-checking is done 
# before we commit to modifying things.   The gnulib code is placed
# in the "gnulib" subdirectory, which is where the buid files expect 
# it to be. 
# 

destdir="gnulib"


# Modules needed for findutils itself.
findutils_modules="\
alloca  argmatch  dirname error fileblocks  fnmatch-gnu  \
getline getopt human idcache  lstat malloc memcmp memset mktime	  \
modechange   pathmax quotearg realloc regex rpmatch savedir stat  \
stpcpy strdup strftime  strstr strtol strtoul strtoull strtoumax  \
xalloc xalloc-die xgetcwd  xstrtol  xstrtoumax yesno human filemode \
getline stpcpy canonicalize mountlist"

# Modules needed for the "intl" subdirectory.
#intl_modules="regex"
# We currently don't use the intl subdirectory.
intl_modules=""

modules="$findutils_modules $intl_modules"
export modules

if test $# -lt 1
then
    echo "You need to specify the name of the directory containing gnulib" >&2
    exit 1
fi

if test -d "$1"
then
    true
else
    echo "$1 is not a directory" >&2
    exit 1
fi

if test -f "$1"/gnulib-tool
then
    true
else
    echo "$1/gnulib-tool does not exist, did you specify the right directory?" >&2
    exit 1
fi

if test -x "$1"/gnulib-tool
then
    true
else
    echo "$1/gnulib-tool is not executable" >&2
    exit 1
fi

    
# exec "$1"/gnulib-tool --create-testdir --dir="$destdir" --lib=libgnulib $modules

if [ -d gnulib ]
then
    echo "Warning: directory gnulib already exists." >&2
else
    mkdir gnulib
fi


if "$1"/gnulib-tool --import --dir=. --lib=libgnulib --source-base=gnulib/lib --m4-base=gnulib/m4  $modules
then
    : OK
else
    echo "gnulib-tool failed, exiting." >&2
    exit 1
fi



cat > gnulib/Makefile.am <<EOF
# Copyright (C) 2004 Free Software Foundation, Inc.
#
# This file is free software, distributed under the terms of the GNU
# General Public License.  As a special exception to the GNU General
# Public License, this file may be distributed as part of a program
# that contains a configuration script generated by Automake, under
# the same distribution terms as the rest of that program.
#
# This file was generated by $0 $@.
#
SUBDIRS = lib m4
EOF

(
cat <<EOF
# Copyright (C) 2004 Free Software Foundation, Inc.
#
# This file is free software, distributed under the terms of the GNU
# General Public License.  As a special exception to the GNU General
# Public License, this file may be distributed as part of a program
# that contains a configuration script generated by Automake, under
# the same distribution terms as the rest of that program.
#
# This file was generated by $0 $@.
#
EOF
printf "%s" "EXTRA_DIST = "
cd  gnulib/m4
ls *.m4 | sed -e 's/$/ \\/' | sed -e '$ s/\\$//'
echo 
) > gnulib/m4/Makefile.am