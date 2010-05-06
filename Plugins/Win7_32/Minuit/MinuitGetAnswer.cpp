#include "MinuitGetAnswer.hpp"
#include "Time2.hpp"

#include <iostream>
#include <sstream>

using namespace std;

//#if !defined(gettimeofday)
//int gettimeofday(struct timeval* tp, void* tzp) {
//		DWORD t;
//		t = timeGetTime();
//		tp->tv_sec = t / 1000;
//		tp->tv_usec = (t % 1000) * 1000;
//		/* 0 indicates that the call succeeded. */
//		return 0;
//}
//#endif

MinuitGetAnswer::MinuitGetAnswer()
{
	m_state = NO_ANSWER;

	struct timeval tv;
//	struct timezone tz;

	Time2 time2;
	time2.gettimeofday(&tv, NULL);

	m_launchTimeInMs = (tv.tv_sec * 1000000L + tv.tv_usec)/1000;
	m_timeOutInMs = NO_TIMEOUT;
}

MinuitGetAnswer::~MinuitGetAnswer()
{
}

int MinuitGetAnswer::getState()
{
	if ((m_state == NO_ANSWER) && (m_timeOutInMs != NO_TIMEOUT)) {
		struct timeval tv;
//		struct timezone tz;

		long long dt = 0;

		Time2 time2;
		time2.gettimeofday(&tv, NULL);

		dt = (tv.tv_sec * 1000000L + tv.tv_usec)/1000 - m_launchTimeInMs;

		if (dt > m_timeOutInMs) {
			m_state = TIMEOUT_EXCEEDED;
		}
	}
	return m_state;
}
void MinuitGetAnswer::setTimeOut(int timeout) {
	m_timeOutInMs = timeout;
}

void MinuitGetAnswer::parseMinuitGetAnswer(const osc::ReceivedMessage&m)
{
	osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
	std::ostringstream ossWithAddress;
	std::ostringstream ossWithoutAddress;
	std::ostringstream floatCorrection;

	// get the address
	ossWithAddress << arg->AsString();
	++arg;
	
	while(arg != m.ArgumentsEnd()) {

		if (arg->IsChar()) {
			ossWithAddress  << arg->AsChar();
			ossWithoutAddress  << arg->AsChar();
		} else if (arg->IsInt32()) {
			ossWithAddress  << arg->AsInt32();
			ossWithoutAddress  << arg->AsInt32();
		} else if (arg->IsString()) {
			ossWithAddress  << arg->AsString();
			ossWithoutAddress  << arg->AsString();
		}else if (arg->IsFloat()) {
			floatCorrection.str("");

			floatCorrection << arg->AsFloat();

			std::string floatWithPoint = floatCorrection.str();

			if(floatWithPoint.find(".") == floatWithPoint.npos) {
				floatWithPoint = floatWithPoint + ".";
			}

			ossWithAddress  << floatWithPoint;
			ossWithoutAddress  << floatWithPoint;
		}

		++arg;
		if(arg != m.ArgumentsEnd()){
			ossWithAddress << " ";
			ossWithoutAddress << " ";
		}
	}

	m_getStringWithAddress = ossWithAddress.str();
	m_getStringWithoutAddress = ossWithoutAddress.str();
	
	m_state = ANSWER_RECEIVED;
}

std::string MinuitGetAnswer::waitGetAnswer(bool withAddress)
{
	if (withAddress) {
		return m_getStringWithAddress;
	} else {
		return m_getStringWithoutAddress;
	}
}
