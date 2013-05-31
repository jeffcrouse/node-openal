#include <node.h>
#include "NodeOpenAlDevice.h"

using namespace v8;

// ------------------------------------------
NodeOpenALDevice::NodeOpenALDevice() {
	device = alcOpenDevice(NULL);
    if(device==NULL) {
		std::cout << "cannot open sound card" << std::endl;
		return;
    }
};

// ------------------------------------------
NodeOpenALDevice::~NodeOpenALDevice() {
	if(device)
		alcCloseDevice(device);
};

// ------------------------------------------
void NodeOpenALDevice::Init(Handle<Object> exports) {
	// Prepare constructor template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->SetClassName(String::NewSymbol("Device"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
	exports->Set(String::NewSymbol("Device"), constructor);
}

// ------------------------------------------
Handle<Value> NodeOpenALDevice::New(const Arguments& args) {
	HandleScope scope;

	NodeOpenALDevice* obj = new NodeOpenALDevice();
	//obj->counter_ = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
	obj->Wrap(args.This());

	return args.This();
}
