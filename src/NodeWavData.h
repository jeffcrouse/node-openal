#pragma once
#include <node.h>
#include <iostream>
#include <fstream>
#include <cstring>

using namespace v8;
using namespace std;

class NodeWavData : public node::ObjectWrap {
	public:
		static void Init(v8::Handle<v8::Object> exports);
		
		char* data;
		int channel;
		int samplerate;
		int bps;
		int size;

	private:
		NodeWavData(const char* path);
		~NodeWavData();

		static v8::Handle<v8::Value> New(const v8::Arguments& args);
};