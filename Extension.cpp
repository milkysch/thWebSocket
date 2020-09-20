#define BOOST_USE_WINDOWS_H

#include "Extension.h"
#include "server_ws.hpp"

#include <codecvt>
#include <string>

static std::thread* server_thread;
using WsServer = SimpleWeb::SocketServer<SimpleWeb::WS>;
static WsServer server;

std::string wstring_to_utf8(const std::wstring& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.to_bytes(str);
}

void startServer() {
	//server.config.address = "127.0.0.1";
	server.config.port = 21370;
	auto& echo_all = server.endpoint["^/?$"];
	server_thread = new std::thread ([]{ server.start(); });
}

void closeServer() {
	server.stop();
	server_thread->join();
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		startServer();
		break;
	case DLL_PROCESS_DETACH:
		closeServer();
		break;
	}
	return TRUE;
}

bool ProcessSentence(std::wstring& sentence, SentenceInfo sentenceInfo)
{
	if (sentenceInfo["current select"])
	{
		for (auto& a_connection : server.get_connections())
			a_connection->send(wstring_to_utf8(sentence));
	}
	return true;
}