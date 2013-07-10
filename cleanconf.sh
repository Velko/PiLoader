#!/bin/sh

cd client
rm -rf aclocal.m4 autom4te.cache/ config.log config.status configure depcomp install-sh Makefile Makefile.in missing
cd ../raspi
rm -rf autom4te.cache config.h config.log config.status configure Makefile
