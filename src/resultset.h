#pragma once
#include <nan.h>

extern "C"{
    #include <yaz/zoom.h>
}

namespace node_zoom {

class ResultSet : public Nan::ObjectWrap {
    public:
        explicit ResultSet(ZOOM_resultset resultset);
        ~ResultSet();

        static void Init();
        static NAN_METHOD(New);
        static NAN_METHOD(Destroy);
        static NAN_METHOD(GetOption);
        static NAN_METHOD(SetOption);
        static NAN_METHOD(GetRecords);
        static NAN_METHOD(Size);
        static Nan::Persistent<v8::Function> constructor;

    protected:
        ZOOM_resultset zset_;
};

class GetRecordsWorker : public Nan::AsyncWorker {
    public:
        GetRecordsWorker(Nan::Callback *callback, ZOOM_resultset resultset,
            size_t index, size_t counts) :
            Nan::AsyncWorker(callback), zresultset_(resultset),
            counts_(counts), index_(index) {};
        ~GetRecordsWorker() {};
        void Execute();
        void HandleOKCallback();

    protected:
        ZOOM_resultset zresultset_;
        ZOOM_record *zrecords_;
        size_t counts_;
        size_t index_;
};

} // namespace node_zoom
