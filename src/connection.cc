#include <uv.h>
#include <sstream>
#include "errors.h"
#include "query.h"
#include "resultset.h"
#include "connection.h"

using namespace v8;

namespace node_zoom {

Nan::Persistent<Function> Connection::constructor;

void Connection::Init(Local<Object> exports) {
    Nan::HandleScope scope;
    v8::Local<v8::Context> context = exports->CreationContext();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("Connection").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    
    // Prototype
    Nan::SetPrototypeMethod(tpl, "connect", Connect);
    Nan::SetPrototypeMethod(tpl, "destory", Destory);
    Nan::SetPrototypeMethod(tpl, "search", Search);
    Nan::SetPrototypeMethod(tpl, "update", Update);

    constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
    exports->Set(context, Nan::New("Connection").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

Connection::Connection(Options *opts) {
    zconn_ = ZOOM_connection_create(opts->zoom_options());
    ZOOM_connection_option_set(zconn_, "saveAPDU", "1");
}

Connection::~Connection() {
    ZOOM_connection_destroy(zconn_);
}

NAN_METHOD(Connection::New) {
    Nan::HandleScope scope;

    if (info.IsConstructCall()) {
        if (info.Length() < 1) {
            Nan::ThrowError(ArgsSizeError("Constructor", 1, info.Length()));
            return;
        }
        Options* opts = Nan::ObjectWrap::Unwrap<Options>(Nan::To<Object>(info[0]).ToLocalChecked());
        Connection* obj = new Connection(opts);
        obj->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    } else {
        const int argc = 1;
        Local<Value> argv[argc] = { info[0] };
        Local<Function> cons = Nan::New<Function>(constructor);
        info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
    }
}

NAN_METHOD(Connection::Connect) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
        Nan::ThrowError(ArgsSizeError("Connect", 2, info.Length()));
        return;
    }

    if (!info[0]->IsString()) {
        Nan::ThrowError(ArgTypeError("first", "string"));
        return;
    }

    if (!info[1]->IsNumber()) {
        Nan::ThrowError(ArgTypeError("second", "number"));
        return;
    }

    if (!info[2]->IsFunction()) {
        Nan::ThrowError(ArgTypeError("third", "function"));
        return;
    }

    Connection* connection = Nan::ObjectWrap::Unwrap<Connection>(info.This());

    Nan::Utf8String *host = new Nan::Utf8String(info[0]);
    int port = info[1]->Uint32Value(Nan::GetCurrentContext()).FromJust();
    Nan::Callback *callback = new Nan::Callback(info[2].As<Function>());
    ConnectWorker *worker = new ConnectWorker(
        callback, connection->zconn_, host, port);

    Nan::AsyncQueueWorker(worker);
}

NAN_METHOD(Connection::Destory) {
    Nan::HandleScope scope;

    Connection* connection = Nan::ObjectWrap::Unwrap<Connection>(info.This());
    ZOOM_connection_destroy(connection->zconn_);
}

NAN_METHOD(Connection::Update) {
    Nan::HandleScope scope;

    Connection* connection = Nan::ObjectWrap::Unwrap<Connection>(info.This());
    Options* packageOpts = Nan::ObjectWrap::Unwrap<Options>(Nan::To<Object>(info[0]).ToLocalChecked());
    
    Nan::Callback *callback = new Nan::Callback(info[1].As<Function>());
    UpdateWorker *worker = new UpdateWorker(callback, connection->zconn_, packageOpts->zoom_options());
    Nan::AsyncQueueWorker(worker);
}

NAN_METHOD(Connection::Search) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
        Nan::ThrowError(ArgsSizeError("Search", 1, info.Length()));
        return;
    }

    if (!info[0]->IsObject()) {
        Nan::ThrowError(ArgTypeError("first", "object"));
        return;
    }

    if (!info[1]->IsFunction()) {
        Nan::ThrowError(ArgTypeError("second", "function"));
        return;
    }

    Connection* connection = Nan::ObjectWrap::Unwrap<Connection>(info.This());
    Query* query = Nan::ObjectWrap::Unwrap<Query>(Nan::To<Object>(info[0]).ToLocalChecked());
    
    Nan::Callback *callback = new Nan::Callback(info[1].As<Function>());
    SearchWorker *worker = new SearchWorker(
        callback, connection->zconn_, query->zoom_query());

    Nan::AsyncQueueWorker(worker);
}

ConnectWorker::~ConnectWorker() {
    delete host_;
}

void ConnectWorker::Execute() {
    ZOOM_connection_connect(zconn_, **host_, port_);

    int error = 0;
    const char *errmsg, *addinfo;

    if ((error = ZOOM_connection_error(zconn_, &errmsg, &addinfo))) {
        std::ostringstream ss;

        ss << "error: "
            << errmsg
            << "(" << error << ") "
            << addinfo;

        SetErrorMessage(ss.str().c_str());
    }
}

void UpdateWorker::Execute() {
    z_package = ZOOM_connection_package(zconn_, zoptions_);
    ZOOM_package_send(z_package, "update");

    int error = 0;
    const char *errmsg, *addinfo;

    if ((error = ZOOM_connection_error(zconn_, &errmsg, &addinfo))) {
        std::ostringstream ss;
        ss << ZOOM_connection_option_get(zconn_, "host")
            << " error "
            << errmsg
            << "(" << error << ")"
            << " " << addinfo;

        SetErrorMessage(ss.str().c_str());
    }

}

void UpdateWorker::HandleOKCallback() {
    Nan::HandleScope scope;

    const char *targetReference = ZOOM_package_option_get(z_package, "targetReference");
    const char *xmlUpdateDoc = ZOOM_package_option_get(z_package, "xmlUpdateDoc");
    const char *apdu = ZOOM_connection_option_get(zconn_, "APDU");

    Local<Object> jsonObject = Nan::New<Object>();

    if(targetReference != NULL) {
        Local<Value> targetReferenceProp = Nan::New("targetReference").ToLocalChecked();
        Local<Value> targetReferenceValue = Nan::New(targetReference).ToLocalChecked();
        Nan::Set(jsonObject, targetReferenceProp, targetReferenceValue);
    }

    if(xmlUpdateDoc != NULL) {
        Local<Value> xmlUpdateDocProp = Nan::New("xmlUpdateDoc").ToLocalChecked();
        Local<Value> xmlUpdateDocValue = Nan::New(xmlUpdateDoc).ToLocalChecked();
        Nan::Set(jsonObject, xmlUpdateDocProp, xmlUpdateDocValue);
    }

    if(apdu != NULL) {
        Local<String> apduProp = Nan::New("apdu").ToLocalChecked();
        Local<String> apduValue = Nan::New(apdu).ToLocalChecked();
        Nan::Set(jsonObject, apduProp, apduValue);
    }


    Local<Value> argv[] = {
        Nan::Null(),
        jsonObject
    };

    ZOOM_package_destroy(z_package);
    callback->Call(2, argv);
}

void UpdateWorker::HandleErrorCallback() {
    Nan::HandleScope scope;
    v8::Local<v8::Value> argv[] = {
      Nan::New(this->ErrorMessage()).ToLocalChecked(),
      Nan::Null()
    };

    ZOOM_package_destroy(z_package);
    callback->Call(2, argv);
}


void SearchWorker::Execute() {
    zresultset_ = ZOOM_connection_search(zconn_, zquery_);

    int error = 0;
    const char *errmsg, *addinfo;

    if ((error = ZOOM_connection_error(zconn_, &errmsg, &addinfo))) {
        std::ostringstream ss;

        ss << "error: "
            << errmsg
            << "(" << error << ") "
            << addinfo;

        SetErrorMessage(ss.str().c_str());
    }
}

void SearchWorker::HandleOKCallback() {
    Nan::HandleScope scope;

    ResultSet* resultset = new ResultSet(zresultset_);
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(ResultSet::constructor);
    Nan::MaybeLocal<v8::Object> maybeInstance = Nan::NewInstance(cons, 0, NULL);
    v8::Local<v8::Object> wrapper;
    if (maybeInstance.IsEmpty()) {
        Nan::ThrowError("Could not create new ResultSet instance");
    } else {
        wrapper = maybeInstance.ToLocalChecked();
    }
    Nan::SetInternalFieldPointer(wrapper, 0, resultset);

    Local<Value> argv[] = {
        Nan::Null(),
        wrapper
    };

    callback->Call(2, argv);
}

} // namespace node_zoom
