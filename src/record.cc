#include "errors.h"
#include "record.h"

using namespace v8;

namespace node_zoom {

Nan::Persistent<Function> Record::constructor;

void Record::Init() {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("Record").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    
    // Prototype
    Nan::SetPrototypeMethod(tpl, "get", Get);

    constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
}

Record::Record(ZOOM_record record) : zrecord_(record) {}

Record::~Record() {
    ZOOM_record_destroy(zrecord_);
}

NAN_METHOD(Record::New) {}

NAN_METHOD(Record::Get) {
    Nan::HandleScope scope;

    Record* record = Nan::ObjectWrap::Unwrap<Record>(info.This());

    if (info.Length() < 1) {
        Nan::ThrowError(ArgsSizeError("Get", 1, info.Length()));
        return;
    }

    Nan::Utf8String type(info[0]);
    const char *value = ZOOM_record_get(record->zrecord_, *type, NULL);

    if (value) {
        info.GetReturnValue().Set(Nan::New(value).ToLocalChecked());
    }
}

} // namespace node_zoom
