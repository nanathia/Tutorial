#pragma once
#include "Ref.h"
#include <list>
#include <map>
#include <Windows.h>

class DebugMilliSecondLapper :
	public Ref
{
	struct LapInfo{
		std::string labelName; // name
		DWORD startTime;
		DWORD time[60]; // avelage
		int currentIndex; // avelage frame index
		bool bEnd;
		bool bDrawed;
		LapInfo():labelName(""){
			bEnd = true;
			for (int i = 0; i < 60; i++){
				time[i] = 0;
			}
			startTime = 0;
			bDrawed = true;
			currentIndex = 0;
		};
	};
	int m_lapCount = 0;
	bool m_lapFlag = true;
	std::map<std::string, LapInfo*> m_nameList;

public:

	void Start(const std::string& name); // count start
	void End(const std::string& name); // counst end

	void draw(); // frame end draw

	DebugMilliSecondLapper();
	~DebugMilliSecondLapper();
};

