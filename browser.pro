QT       += core gui widgets webkitwidgets network
CONFIG   += c++17

# Essential for GCC 14 compatibility with older Qt 5 headers
QMAKE_CXXFLAGS += -fpermissive
SOURCES += main.cpp
