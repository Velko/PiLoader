#!/bin/sh

cd client
aclocal
automake --add-missing --copy
automake
autoconf
