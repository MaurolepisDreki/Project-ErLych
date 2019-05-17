## Process this file with automake to produce Makfile.in

bin_PROGRAMS = migrate
lib_LTLIBRARIES = libmidget.la
# TODO: ^^ reseach benifit of using Libtool vs building .so files directly
#       the issue lies in lt_dlopen vs dlopen, or rather the overhead of 
#       libtool

libmidget_la_SOURCES = sayhi.cpp sayhi.h
#libmidget_la_LDFLAGS = -llibpq

# Migrate forms a manual testing bed for libmidget as well as a standalone 
#   database migration tool
migrate_SOURCES = driver.cpp sayhi.h
migrate_LDADD = libmidget.la

