#define codeSEG_1 0
#define codeSEG_2 256
#define stop_order "00000000000000000000000000000000"
unsigned _stdcall run(void* pArguments);//线程函数 
void loading_orders();//加载指令 
void turnto_TF(char *); //补码转换为原码 
int turnto_DEC(char*,int);//将二进制串转为十进制数 
int pick_AX(char *,int);//取出操作对象 
int pick_IN(char *);//取出立即数 
void print_ax_state(int,int,int,int,int [][2]);//输出寄存器状态 
void print_code_and_data();//输出数据段和代码段 
typedef struct threadArgument{
	int ID_thread;//线程id 
	int Address_thread;//代码段起始地址 
}th;

