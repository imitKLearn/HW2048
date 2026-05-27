#include <iostream>

class PureClass
{
public:
	virtual void PureFunction() = 0;
};

class PureClass2
{
public:
	virtual void PureFunction2() = 0;
};

class DerivedClass : public PureClass
{
public:
	virtual void PureFunction() override {}
};

class DerivedClass2 : public PureClass, public PureClass2
{
	virtual void PureFunction() override {}
	virtual void PureFunction2()override {}
};

void main()
{
	std::cout << "Size: " << sizeof(PureClass) << std::endl;
	std::cout << "Size: " << sizeof(DerivedClass) << std::endl;
	std::cout << "Size: " << sizeof(DerivedClass2) << std::endl;
}