#ifndef FESTIVAL_H
#define FESTIVAL_H

#define INCLUDE_FESTIVAL 1

#ifdef INCLUDE_FESTIVAL
    #define ON_FESTIVAL(...) __VA_ARGS__
#else
    #define ON_FESTIVAL(...) 
#endif

#define ECHO "echo "
#define FESTIVAL " | festival --tts"

#endif