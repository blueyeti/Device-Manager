#include "Namespace.h"
//#include "xmlParser.h"

Namespace::Namespace()
{
	//m_directory = NULL;
}

Namespace::~Namespace()
{

}

void
Namespace::namespaceInit(std::string appName)
{
	//TTErr err;
	//TTSymbolTable st;

	//err = env->createInstance(TT("NodeDirectory"), TTObjectHandle(&m_directory), TT(appName));
	//err = TTObjectInstantiate(st.lookup("NodeDirectory"), TTObjectHandle(&m_directory), st.lookup(appName));
	//TT_ASSERT("NodeDirectory created successfully", !err);

}

void
Namespace::namespaceRelease()
{
	//TTObjectRelease(TTObjectHandle(&m_directory));
}

void 
Namespace::namespaceLoadFromXml(std::string filename)
{
	//xMainNode = XMLNode::openFileHelper(filename.c_str(), "deviceManager");
}