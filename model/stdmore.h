
#include <time.h>
namespace stdmore
{
	//��time_tתΪtm��ʽ
	//�򵥷�װlocaltime_s������Ҫ���޸�
	inline errno_t __cdecl to_tm(time_t &from, tm &to)
	{
		return localtime_s(&to, &from);
	}

}