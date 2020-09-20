#include "extension.h"

bool ProcessSentence(std::wstring& sentence, SentenceInfo sentenceInfo);

extern "C" __declspec(dllexport) wchar_t* OnNewSentence(wchar_t* sentence, const InfoForExtension * sentenceInfo)
{
	try
	{
		std::wstring sentenceStr(sentence);
		int origLength = sentenceStr.size();
		if (ProcessSentence(sentenceStr, SentenceInfo{ sentenceInfo }))
		{
			if (sentenceStr.size() > origLength) sentence = (wchar_t*)HeapReAlloc(GetProcessHeap(), HEAP_GENERATE_EXCEPTIONS, sentence, (sentenceStr.size() + 1) * sizeof(wchar_t));
			wcscpy_s(sentence, sentenceStr.size() + 1, sentenceStr.c_str());
		}
	}
	catch (SKIP)
	{
		*sentence = L'\0';
	}
	return sentence;
}