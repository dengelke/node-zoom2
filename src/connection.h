#pragma once
#include <nan.h>
#include "options.h"

extern "C" {
    #include <yaz/zoom.h>
}

namespace node_zoom {

class Connection : public Nan::ObjectWrap {
    public:
        explicit Connection(Options *opts);
        ~Connection();

        static void Init(v8::Handle<v8::Object> exports);
        static NAN_METHOD(New);
        static NAN_METHOD(Connect);
        static NAN_METHOD(Destory);
        static NAN_METHOD(Search);
        static NAN_METHOD(Update);

    protected:
        ZOOM_connection zconn_;
        static Nan::Persistent<v8::Function> constructor;
};

class ConnectWorker : public Nan::AsyncWorker {
    public:
        ConnectWorker(Nan::Callback *callback, ZOOM_connection zconn,
            Nan::Utf8String *host, int port) :
            Nan::AsyncWorker(callback), zconn_(zconn),
            host_(host), port_(port) {};
        ~ConnectWorker();
        void Execute();

    protected:
        ZOOM_connection zconn_;
        Nan::Utf8String *host_;
        int port_;
};

class SearchWorker : public Nan::AsyncWorker {
    public:
        SearchWorker(Nan::Callback *callback, ZOOM_connection zconn,
            ZOOM_query query) :
            Nan::AsyncWorker(callback), zconn_(zconn), zquery_(query) {};
        ~SearchWorker() {};
        void Execute();
        void HandleOKCallback();

    protected:
        ZOOM_connection zconn_;
        ZOOM_query zquery_;
        ZOOM_resultset zresultset_;
};

class UpdateWorker : public Nan::AsyncWorker {
    public:
        UpdateWorker(Nan::Callback *callback, ZOOM_connection zconn,
            ZOOM_options options) :
            Nan::AsyncWorker(callback), zconn_(zconn), zoptions_(options) {};
        ~UpdateWorker() {};
        void Execute();
        void HandleOKCallback();
        void HandleErrorCallback();

    protected:
        ZOOM_connection zconn_;
        ZOOM_options zoptions_;
        ZOOM_resultset zresultset_;
        ZOOM_package z_package;
};

} // namespace node_zoom
