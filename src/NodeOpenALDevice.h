#pragma once
#include <iostream>
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
		static void Init(v8::Handle<v8::Object> exports);
		ALCdevice* device;

	private:
		NodeOpenALDevice();
		~NodeOpenALDevice();

		static v8::Handle<v8::Value> New(const v8::Arguments& args);
};
