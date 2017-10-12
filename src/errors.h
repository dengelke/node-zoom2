#pragma once
#include <v8.h>

namespace node_zoom {

v8::Local<v8::Value> ArgsSizeError(const char *fnname, int expect, int actual);
v8::Local<v8::Value> ArgTypeError(const char *arg, const char *expect);

} // namespace node_zoom
