TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -Wl,--no-as-needed -lpthread

LIBS += -lcrypt


SOURCES += main.c \
    brute_force.c \
    check_pass.c \
    client.c \
    parse.c \
    run_mode.c \
    serialize.c \
    state.c \
    thread_queue.c

HEADERS += \
    brute_force.h \
    check_pass.h \
    declarations.h \
    parse.h \
    run_mode.h \
    serialize.h \
    serizalize.h \
    state.h \
    thread_queue.h

DISTFILES += \
    Makefile \
    README.md
