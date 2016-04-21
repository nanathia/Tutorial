#include "DebugMilliSecondLapper.h"
#include "INCLUDES.h"
#include "Director.h"
#include "DebugDrawer.h"
#include "GyuDon.h"
#include <sstream>


DebugMilliSecondLapper::DebugMilliSecondLapper()
{
}


DebugMilliSecondLapper::~DebugMilliSecondLapper()
{
}




void DebugMilliSecondLapper::Start(const std::string& name){
	auto it = m_nameList.find(name);
	LapInfo* target = 0;
	if (it == m_nameList.end()){
		// create
		target = new LapInfo();
		m_nameList[name] = target;
		m_nameList[name]->labelName = name;
	}
	else{
		target = it->second;
	}
	target->startTime = timeGetTime();
	target->bEnd = false;
}// count start

void DebugMilliSecondLapper::End(const std::string& name){
	auto it = m_nameList.find(name);
	ASSERT(it != m_nameList.end() && "��x���g���Ă��Ȃ��̂ɏI�����悤�Ƃ��Ă���");
	LapInfo* p = it->second;
	ASSERT(!p->bEnd && "�g���Ă��Ȃ��̂ɏI�����悤�Ƃ��Ă���");
	p->bEnd = true;
	p->bDrawed = false;
	DWORD lapTime = timeGetTime() - p->startTime;

	p->time[p->currentIndex] = lapTime;
	p->currentIndex++;
	if (p->currentIndex >= 60){
		p->currentIndex = 0;
	}

}// counst end

// �f�o�b�O�����N���X�ɂU�O�t���̕��σ~���b�����ꂼ��o�͂���B
void DebugMilliSecondLapper::draw(){
	DebugDrawer* d = Director::instance()->framework()->debugDrawer();
	d->AddDebugString("<MilliSecond_Measurement>");
	for (auto it = m_nameList.begin(); it != m_nameList.end(); it++){
		LapInfo* p = it->second;
		if (p->bDrawed){
			continue;
		}
		ASSERT(p->bEnd && "�v�����I�����Ă��Ȃ����`�悵�悤�Ƃ��Ă���");
		int all = 0;
		for (int i = 0; i < 60; i++){
			all += p->time[i];
		}
		all /= 60;
		std::ostringstream oss;
		oss << p->labelName << " = " << all << "ms";
		d->AddDebugString(oss.str().c_str());
		p->bDrawed = true;
	}
	d->AddDebugString("<MilliSecond_Measurement/>");
}