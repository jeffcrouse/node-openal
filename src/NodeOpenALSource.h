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

#include "NodeWavData.h"

using namespace v8;
using namespace std;

class NodeOpenALSource : public node::ObjectWrap {
	public:
		static void Init(v8::Handle<v8::Object> exports);

	private:
		NodeOpenALSource( NodeWavData* data );
		~NodeOpenALSource();

		static v8::Handle<v8::Value> New(const v8::Arguments& args);
		static v8::Handle<v8::Value> Play(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetPosition(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetLoop(const v8::Arguments& args);

		void play();
		void setPosition(double x, double y, double z);
		void setLoop(bool loop);
		unsigned int bufferid;
		unsigned int format;
		unsigned int sourceid;
};
