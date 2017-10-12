#pragma once
#include <nan.h>
#include "options.h"

extern "C" {
    #include <yaz/zoom.h>
}

namespace node_zoom {

class Connection : public node::ObjectWrap {
    public:
        explicit Connection(Options *opts);
        ~Connection();

        static void Init(v8::Handle<v8::Object> exports);
        static NAN_METHOD(New);
        static NAN_METHOD(Connect);
        static NAN_METHOD(Destory);
        static NAN_METHOD(Search);

    protected:
        ZOOM_connection zconn_;
        static v8::Persistent<v8::Function> constructor;
};

class ConnectWorker : public NanAsyncWorker {
    public:
        ConnectWorker(NanCallback *callback, ZOOM_connection zconn,
            NanUtf8String *host, int port) :
            NanAsyncWorker(callback), zconn_(zconn),
            host_(host), port_(port) {};
        ~ConnectWorker();
        void Execute();

    protected:
        ZOOM_connection zconn_;
        NanUtf8String *host_;
        int port_;
};

class SearchWorker : public NanAsyncWorker {
    public:
        SearchWorker(NanCallback *callback, ZOOM_connection zconn,
            ZOOM_query query) :
            NanAsyncWorker(callback), zconn_(zconn), zquery_(query) {};
        ~SearchWorker() {};
        void Execute();
        void HandleOKCallback();

    protected:
        ZOOM_connection zconn_;
        ZOOM_query zquery_;
        ZOOM_resultset zresultset_;
};

} // namespace node_zoom
