#pragma once
#include <nan.h>
#include <node.h>
#include <sstream>
#include "errors.h"

namespace node_zoom {

v8::Local<v8::Value> ArgsSizeError(const char *fnname, int expect, int actual) {
    std::ostringstream ss;

    ss << fnname
        << " takes exactly "
        << expect
        << " arguments"
        << " (" << actual << " given)";

    return NanTypeError(ss.str().c_str());
}

v8::Local<v8::Value> ArgTypeError(const char *arg, const char *expect) {
    const char fc = tolower(expect[0]);

    int is_vowel = fc == 'a'
        || fc == 'e'
        || fc == 'i'
        || fc == 'o'
        || fc == 'u';

    std::ostringstream ss;

    ss << "The "
        << arg
        << " argument must be "
        << (is_vowel ? "an" : "a")
        << " " << expect;

    return NanTypeError(ss.str().c_str());
}

} // namespace node_zoom
