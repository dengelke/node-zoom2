#include "errors.h"
#include "record.h"
#include "records.h"

using namespace v8;

namespace node_zoom {

Nan::Persistent<Function> Records::constructor;

void Records::Init() {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("Records").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    
    // Prototype
    Nan::SetPrototypeMethod(tpl, "next", Next);
    Nan::SetPrototypeMethod(tpl, "hasNext", HasNext);

    constructor.Reset(tpl->GetFunction());
}

Records::~Records() {
    delete[] zrecords_;
}

NAN_METHOD(Records::New) {}

NAN_METHOD(Records::Next) {
    Nan::HandleScope scope;
    Records* resset = Nan::ObjectWrap::Unwrap<Records>(info.This());

    if (resset->index_ >= resset->counts_) {
        Nan::ThrowRangeError("Out of range");
    } else {
        ZOOM_record zrecord = resset->zrecords_[resset->index_++];

        if (zrecord == NULL) {
            info.GetReturnValue().Set(Nan::Null());
        } else {
            Record* record = new Record(ZOOM_record_clone(zrecord));
            
            v8::Local<v8::Function> cons = Nan::New<v8::Function>(Record::constructor);
            Nan::MaybeLocal<v8::Object> maybeInstance = Nan::NewInstance(cons, 0, NULL);
            v8::Local<v8::Object> wrapper;
            if (maybeInstance.IsEmpty()) {
                Nan::ThrowError("Could not create new Record instance");
            } else {
                wrapper = maybeInstance.ToLocalChecked();
            }
            // Local<Object> wrapper = Nan::New(Record::constructor)->NewInstance();
            Nan::SetInternalFieldPointer(wrapper, 0, record);
            info.GetReturnValue().Set(wrapper);
        }
    }
}

NAN_METHOD(Records::HasNext) {
    Nan::HandleScope scope;
    Records* resset = Nan::ObjectWrap::Unwrap<Records>(info.This());
    info.GetReturnValue().Set(Nan::New<Boolean>(resset->index_ < resset->counts_));
}

} // namespace node_zoom
