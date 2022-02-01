TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = \
    Compiler \
    Test

Test.depends = Compiler
