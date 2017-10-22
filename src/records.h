#pragma once
#include <nan.h>

extern "C" {
    #include <yaz/zoom.h>
}

namespace node_zoom {

class Records : public Nan::ObjectWrap {
    public:
        explicit Records(ZOOM_record *records, size_t counts) :
            zrecords_(records), index_(0), counts_(counts) {};
        ~Records();

        static void Init();
        static NAN_METHOD(New);
        static NAN_METHOD(Next);
        static NAN_METHOD(HasNext);
        static Nan::Persistent<v8::Function> constructor;

    protected:
        ZOOM_record *zrecords_;
        size_t index_;
        size_t counts_;
};

} // namespace node_zoom
