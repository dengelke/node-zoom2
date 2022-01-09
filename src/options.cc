#include "errors.h"
#include "options.h"

using namespace v8;

namespace node_zoom {

Nan::Persistent<Function> Options::constructor;

void Options::Init(Local<Object> exports) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("Options").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    
    // Prototype
    Nan::SetPrototypeMethod(tpl, "get", Get);
    Nan::SetPrototypeMethod(tpl, "set", Set);

    constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
    Nan::Set(exports, Nan::New("Options").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

Options::Options() {
    zopts_ = ZOOM_options_create();
}

Options::Options(Options *opts) {
    zopts_ = ZOOM_options_create_with_parent(opts->zopts_);
}

Options::~Options() {
    ZOOM_options_destroy(zopts_);
}

ZOOM_options Options::zoom_options() {
    return zopts_;
}

NAN_METHOD(Options::New) {
    Nan::HandleScope scope;

    if (info.IsConstructCall()) {
        Options* obj;

        if (info[0]->IsUndefined()) {
            obj = new Options();
        } else {
            Options* opts = Nan::ObjectWrap::Unwrap<Options>(Nan::To<Object>(info[0]).ToLocalChecked());
            obj = new Options(opts);
        }

        obj->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    } else {
        const int argc = 1;
        Local<Value> argv[argc] = { info[0] };
        Local<Function> cons = Nan::New<Function>(constructor);
        info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
    }
}

NAN_METHOD(Options::Get) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
        Nan::ThrowError(ArgsSizeError("Set", 1, info.Length()));
        return;
    }

    Nan::Utf8String key(info[0]);
    Options* opts = Nan::ObjectWrap::Unwrap<Options>(info.This());

    const char *value = ZOOM_options_get(opts->zopts_, *key);

    if (value) {
        info.GetReturnValue().Set(Nan::New(value).ToLocalChecked());
    }
}

NAN_METHOD(Options::Set) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
        Nan::ThrowError(ArgsSizeError("Set", 2, info.Length()));
        return;
    }

    if (!info[0]->IsString()) {
        Nan::ThrowError(ArgTypeError("first", "string"));
        return;
    }

    Options* opts = Nan::ObjectWrap::Unwrap<Options>(info.This());

    Nan::Utf8String key(info[0]);
    Nan::Utf8String value(info[1]);

    ZOOM_options_set(opts->zopts_, *key, *value);

    info.GetReturnValue().Set(info.This());
}

} // namespace node_zoom
