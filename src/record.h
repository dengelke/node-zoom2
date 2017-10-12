#pragma once
#include <nan.h>

extern "C" {
    #include <yaz/zoom.h>
}

namespace node_zoom {

class Record : public Nan::ObjectWrap {
    public:
        explicit Record(ZOOM_record record);
        ~Record();

        static void Init();
        static NAN_METHOD(New);
        static NAN_METHOD(Get);
        static Nan::Persistent<v8::Function> constructor;

    protected:
        ZOOM_record zrecord_;
};

} // namespace node_zoom
