#pragma once
#include <iostream>
#include <vector>
#include <node.h>
#include <v8.h>

#ifdef __APPLE__
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#endif

#if defined (_WIN32) || defined (_WIN64)
	#include <AL/al.h>
	#include <AL/alc.h>
#endif

using namespace v8;
using namespace std;


class NodeOpenALDevice : public node::ObjectWrap {
	public:
		static void Init(Handle<Object> exports);
		ALCdevice* device;

		//static vector<NodeOpenALDevice*> devices;

	private:
		NodeOpenALDevice();
		~NodeOpenALDevice();

		static v8::Handle<v8::Value> New(const v8::Arguments& args);
};
