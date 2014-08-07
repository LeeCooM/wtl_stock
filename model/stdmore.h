#include <ctime>
#include <string>
#include <codecvt>
#include <stdarg.h>
namespace stdmore
{
	


	//localtime_r���ص���ָ��,localtime_s���ص��Ǵ����typedef int errno_t�����߲���������ߵ���
	//����ʹ������,�����ִ�����������岻��,��ʹ��bool�����Ƿ�ɹ����ɡ�
	//������tm�ṹ���Ǽ���һ�ο�����������һ��bool result������һ�α������䡣inlineֱ��Ƕ�롣
	inline bool  __cdecl localtime(const std::time_t&& from, tm &to)
	{
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__))
		return (localtime_s(&to, &from)==0);
#else
		return (localtime_r(&from, &to)!=nullptr); // POSIX 
#endif
	}

	//ֱ��ת��Ϊ����
	template <typename charTrait=char>
	inline bool  __cdecl localtime(const std::time_t&& from, std::basic_string<charTrait> &to)
	{
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__))
		return (localtime_s(&to, &from) == 0);
#else
		return (localtime_r(&from, &to) != nullptr); // POSIX 
#endif
	}

	//std::to_string��to_wstring�оŸ����أ�����ֵת��Ϊ�ַ��������ǿ����ճ�ʹ�õġ�

	//��wstringת��Ϊstring
	//string֮��Ŀ�ģ����������⣿
	inline void __cdecl wstring_to_string(std::wstring from,std::string &to)
	{
		//����һ��ת����
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		to = conv.to_bytes(from);//���������ת��:conv.from_bytes(narrowStr);
	}

	//�ȼ����С���ٷ���ռ䣿
	//��ֱ��ʹ��MAX_PATH֮��ĺ�...һ��Ҳ���ᳬ��260�ֽ�
	//ͬʱ����wstring��һ����˵��û��Ҫ̫����
	//ȥ���Ǳ�׼�Ĵ���,��ʹ��std::sprintf()
	inline void format(std::string& a_string, const char* fmt, ...)
	{
		
		va_list vl;
		va_start(vl, fmt);
		int size = _vscprintf(fmt, vl);
		a_string.resize(++size);
		vsnprintf_s((char*)a_string.data(), size, _TRUNCATE, fmt, vl);
		va_end(vl);
	}

	


}