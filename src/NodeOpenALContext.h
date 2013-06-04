#pragma once
#include <iostream>
#include <vector>
#include <node.h>
#ifdef __APPLE__
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#endif

#if defined (_WIN32) || defined (_WIN64)
	#include <AL/al.h>
	#include <AL/alc.h>
#endif

#include "NodeOpenALDevice.h"

using namespace v8;
using namespace std;

class NodeOpenALContext : public node::ObjectWrap {
	public:
		static void Init(v8::Handle<v8::Object> exports);
		ALCcontext* context;
		//static vector<NodeOpenALContext*> contexts;

	private:
		NodeOpenALContext(NodeOpenALDevice* dev);
		~NodeOpenALContext();

		static v8::Handle<v8::Value> New(const v8::Arguments& args);
		static v8::Handle<v8::Value> PlusOne(const v8::Arguments& args);
};
