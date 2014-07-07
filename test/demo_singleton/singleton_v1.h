#pragma once

//��һ����ʵ��singleton�Ļ�������
//��һ�������⣺��̬����Ķ���û����������Ȼnewִֻ��һ�Σ�����ʹϵͳ�˳���Ҳû��delete
//��ˣ���"��๤"ģʽ������һ��˽��Ƕ����������⡣���ַ�ʽ��Ȼ�鷳

class singleton_v1
{
public:
	static singleton_v1 * GetInstance(int); //Ҫ��2��ֻ����һ����ȡʵ��ָ��ķ�ʽ

	//Ϊ����ʾ������һ��int���͵�id�����̲߳�������
	int id;
private:
	singleton_v1(int); //Ҫ��1��˽�й��캯��������ʹ�ø��ֻ࣬��ͨ��GetInstance��ȡ
	static singleton_v1 *pInstance_;	

	//���µĴ��룺��ϵͳ�˳�ʱ����pInstance
	class garbo   //��๤�࣬��ϵͳ�˳�ǰ��garbo_��Ա�����Զ��������ɴ�ִ�������delete�����಻���κ������ط�ʹ�ã����˽��Ƕ���� 
	{
	public:
		~garbo()
		{
			if (singleton_v1::pInstance_)
				delete singleton_v1::pInstance_;
		}
	};
	static garbo garbo_;  //�������ʱ�����Զ���������̬��Ա�������Ӷ�ִ��delete����  
};

