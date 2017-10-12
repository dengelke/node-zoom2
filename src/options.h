#pragma once
#include <nan.h>

extern "C" {
    #include <yaz/zoom.h>
}

namespace node_zoom {

class Options : public node::ObjectWrap {
    public:
        Options();
        explicit Options(Options *opts);
        ~Options();

        static void Init(v8::Handle<v8::Object> exports);
        static NAN_METHOD(New);
        static NAN_METHOD(Get);
        static NAN_METHOD(Set);
        ZOOM_options zoom_options();
    
    protected:
        ZOOM_options zopts_;
        static v8::Persistent<v8::Function> constructor;
};

} // namespace node_zoom
