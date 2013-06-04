#include <node.h>
#include "NodeOpenAlDevice.h"

using namespace v8;

//vector<NodeOpenALDevice*> NodeOpenALDevice::devices;

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
	if(device) {
		cout << "destroying device" << endl;
		alcCloseDevice(device);
	}
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
	//devices.push_back( obj );
	obj->Wrap( args.This() );

	return args.This();
}
