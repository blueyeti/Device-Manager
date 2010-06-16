#ifndef _Namespace_H_
#define _Namespace_H_

#include <string>

//#include "Foundation/TTFoundationAPI.h"
//#include "Foundation/TTNode.h"
//#include "Foundation/TTNodeDirectory.h"
//#include "Foundation/TTSymbolTable.h"

class Namespace
{
public:
	Namespace();
	~Namespace();

	void namespaceInit(std::string appName);
	void namespaceRelease();

	void namespaceLoadFromXml(std::string filename);


private:
	//TTNodeDirectoryPtr m_directory;
	
};

#endif /*Namespace*/