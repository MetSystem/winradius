// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� RLM_EXEC_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// RLM_EXEC_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef RLM_EXEC_EXPORTS
#define RLM_EXEC_API __declspec(dllexport)
#else
#define RLM_EXEC_API __declspec(dllimport)
#endif

// �����Ǵ� rlm_exec.dll ������
/*class RLM_EXEC_API Crlm_exec {
public:
	Crlm_exec(void);
	// TODO: �ڴ�������ķ�����
};
*/
extern RLM_EXEC_API int nrlm_exec;

RLM_EXEC_API int fnrlm_exec(void);
