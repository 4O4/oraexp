#-------------------------------------------------
#
# Project created by QtCreator 2012-12-10T13:26:29
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = GrammarParser
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ../bnfrule.cpp \
    ../bnfruleitem.cpp \
    ../codescanner.cpp \
    ../dfa.cpp \
    ../dfaitem.cpp \
    ../dfastate.cpp \
    ../dfatransition.cpp \
    ../ebnfparser.cpp \
    ../ebnfscanner.cpp \
    ../ebnftoken.cpp \
    ../firstfollowsetcomputer.cpp

HEADERS += \
    ../bnfrule.h \
    ../bnfruleitem.h \
    ../codescanner.h \
    ../dfa.h \
    ../dfaitem.h \
    ../dfastate.h \
    ../dfatransition.h \
    ../ebnfparser.h \
    ../ebnfscanner.h \
    ../ebnftoken.h \
    ../firstfollowsetcomputer.h
