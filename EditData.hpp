#ifndef __EditData_HeaderPlusPlus__
#define __EditData_HeaderPlusPlus__

struct EditData
{
	//

	EditData() {}
	EditData(LPEDATA edPtr)
	{
		Deserialize(edPtr);
	}
	bool Deserialize(LPEDATA edPtr)
	{
		char *p = edPtr->data;
		//

		return true;
	}
	bool Serialize(LPMV mV, LPEDATA &edPtr)
	{
		unsigned size = 0;
		//

		LPEDATA t = (LPEDATA)mvReAllocEditData(mV, edPtr, sizeof(EDITDATA)+size);
		if(t) edPtr = t;
		else return false;

		char *p = edPtr->data;
		//

		return true;
	}

	~EditData(){}
private:
	EditData(const EditData &from);
	EditData &operator=(const EditData &from);
#ifdef UNICODE
	static void PassNull(char *&p)
	{
		while(*(p++))++p;
	}
#else
	static void PassNull(char *&p)
	{
		while(*(p++));
	}
#endif
};

#endif