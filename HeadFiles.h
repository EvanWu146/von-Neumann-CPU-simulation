#define codeSEG_1 0
#define codeSEG_2 256
#define stop_order "00000000000000000000000000000000"
unsigned _stdcall run(void* pArguments);//�̺߳��� 
void loading_orders();//����ָ�� 
void turnto_TF(char *); //����ת��Ϊԭ�� 
int turnto_DEC(char*,int);//�������ƴ�תΪʮ������ 
int pick_AX(char *,int);//ȡ���������� 
int pick_IN(char *);//ȡ�������� 
void print_ax_state(int,int,int,int,int [][2]);//����Ĵ���״̬ 
void print_code_and_data();//������ݶκʹ���� 
typedef struct threadArgument{
	int ID_thread;//�߳�id 
	int Address_thread;//�������ʼ��ַ 
}th;

