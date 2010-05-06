#ifndef MINUITINCLUDE_HPP_
#define MINUITINCLUDE_HPP_

//#include <sys/time.h>
#include <time.h>
#include <windows.h>


//int gettimeofday(struct timeval* tp, void* tzp) {
//		DWORD t;
//		t = timeGetTime();
//		tp->tv_sec = t / 1000;
//		tp->tv_usec = (t % 1000) * 1000;
//		/* 0 indicates that the call succeeded. */
//		return 0;
//}

#define MINUIT_REQUEST_DISCOVER "?namespace"
#define MINUIT_REQUEST_GET "?get"
#define MINUIT_REQUEST_LISTEN "?listen"

#define MINUIT_ANSWER_DISCOVER ":namespace"
#define MINUIT_ANSWER_GET ":get"
#define MINUIT_ANSWER_LISTEN ":listen"

#define MINUIT_REQUEST_LISTEN_ENABLE "enable"
#define MINUIT_REQUEST_LISTEN_DISABLE "disable"

#define MINUIT_START_NODES "nodes={"
#define MINUIT_END_NODES "}"
#define MINUIT_START_LEAVES "leaves={"
#define MINUIT_END_LEAVES "}"
#define MINUIT_START_ATTRIBUTES "attributes={"
#define MINUIT_END_ATTRIBUTES "}"

#define ACCESS_GETTER "getter"
#define ACCESS_SETTER "setter"
#define ACCESS_GETSETTER "getsetter"

#define DEFAULT_TIMEOUT 1000
#define NO_TIMEOUT -1

// TODO use the define of Controller.h
#define REQUEST_NOT_SENT -2
#define TIMEOUT_EXCEEDED -1
#define NO_ANSWER 0
#define ANSWER_RECEIVED 1

#endif /*MINUITINCLUDE_HPP_*/
