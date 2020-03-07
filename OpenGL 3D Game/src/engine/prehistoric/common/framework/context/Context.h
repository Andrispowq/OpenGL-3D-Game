#ifndef CONTEXT_H
#define CONTEXT_H

class Context
{
public:
	static Context& GetContext();

	virtual bool InitContext() = 0;
	virtual bool DeleteContext() = 0;
protected:
	Context() {}
	virtual ~Context() = 0;

	static Context* context;
};

#endif