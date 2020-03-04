#include "errors.h"
#include "records.h"
#include "resultset.h"

using namespace v8;

namespace node_zoom {

Nan::Persistent<Function> ResultSet::constructor;

void ResultSet::Init() {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("ResultSet").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    
    // Prototype
    Nan::SetPrototypeMethod(tpl, "setOption", SetOption);
    Nan::SetPrototypeMethod(tpl, "getOption", GetOption);
    Nan::SetPrototypeMethod(tpl, "size", Size);
    Nan::SetPrototypeMethod(tpl, "getRecords", GetRecords);

    constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
}

ResultSet::ResultSet(ZOOM_resultset resultset) : zset_(resultset) {}

ResultSet::~ResultSet() {
    ZOOM_resultset_destroy(zset_);
}

NAN_METHOD(ResultSet::New) {}

NAN_METHOD(ResultSet::GetOption) {
    ResultSet* resset = Nan::ObjectWrap::Unwrap<ResultSet>(info.This());

    if (info.Length() < 1) {
        Nan::ThrowError(ArgsSizeError("Get", 1, info.Length()));
        return;
    }

    Nan::Utf8String key(info[0]);
    const char *value = ZOOM_resultset_option_get(resset->zset_, *key);

    if (value) {
        info.GetReturnValue().Set(Nan::New(value).ToLocalChecked());
    }
}

NAN_METHOD(ResultSet::SetOption) {
    ResultSet* resset = Nan::ObjectWrap::Unwrap<ResultSet>(info.This());

    if (info.Length() < 2) {
        Nan::ThrowError(ArgsSizeError("Set", 1, info.Length()));
        return;
    }

    Nan::Utf8String key(info[0]);
    Nan::Utf8String value(info[1]);
    ZOOM_resultset_option_set(resset->zset_, *key, *value);

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(ResultSet::GetRecords) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
        Nan::ThrowError(ArgsSizeError("Records", 2, info.Length()));
        return;
    }

    ResultSet* resset = Nan::ObjectWrap::Unwrap<ResultSet>(info.This());
    // size_t index = info[0]->Uint32Value();
    size_t index = info[0]->Uint32Value(Nan::GetCurrentContext()).FromJust();
    // size_t counts = info[1]->Uint32Value();
    size_t counts = info[1]->Uint32Value(Nan::GetCurrentContext()).FromJust();

    Nan::Callback *callback = new Nan::Callback(info[2].As<Function>());
    GetRecordsWorker *worker = new GetRecordsWorker(
        callback, resset->zset_, index, counts);

    Nan::AsyncQueueWorker(worker);
}

NAN_METHOD(ResultSet::Size) {
    Nan::HandleScope scope;
    ResultSet * resset = Nan::ObjectWrap::Unwrap<ResultSet>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(ZOOM_resultset_size(resset->zset_)));
}

void GetRecordsWorker::Execute() {
    zrecords_ = new ZOOM_record[counts_];
    ZOOM_resultset_records(zresultset_, zrecords_, index_, counts_);
}

void GetRecordsWorker::HandleOKCallback() {
    Nan::HandleScope scope;

    Records* records = new Records(zrecords_, counts_);

    v8::Local<v8::Function> cons = Nan::New<v8::Function>(Records::constructor);
    Nan::MaybeLocal<v8::Object> maybeInstance = Nan::NewInstance(cons, 0, NULL);
    v8::Local<v8::Object> wrapper;
    if (maybeInstance.IsEmpty()) {
        Nan::ThrowError("Could not create new Records instance");
    } else {
        wrapper = maybeInstance.ToLocalChecked();
    }
    // Local<Object> wrapper = Nan::New(Records::constructor)->NewInstance();
    Nan::SetInternalFieldPointer(wrapper, 0, records);

    Local<Value> argv[] = {
        Nan::Null(),
        wrapper
    };

    callback->Call(2, argv);
}

} // namespace node_zoom
