#pragma once
#include <nan.h>

extern "C" {
    #include <yaz/zoom.h>
}

namespace node_zoom {

class Query : public Nan::ObjectWrap {
    public:
        explicit Query();
        ~Query();

        static void Init(v8::Local<v8::Object> exports);
        static NAN_METHOD(New);
        static NAN_METHOD(Prefix);
        static NAN_METHOD(CQL);
        static NAN_METHOD(SortBy);
        ZOOM_query zoom_query();

    protected:
        ZOOM_query zquery_;
        static Nan::Persistent<v8::Function> constructor;
};

} // namespace node_zoom
