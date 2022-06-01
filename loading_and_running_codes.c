#include<stdio.h>
#include<string.h>
#include<windows.h>
#include"HeadFiles.h"
#define stop_order "00000000000000000000000000000000"
extern char code_orders[128][34];
extern int data_Segment[256];
extern int order_nums[2];//第一个为dict1的指令数，第二个为dict2的指令数 
extern HANDLE hMutex,hMutex_mem;

void loading_orders(){
	int i=0,j=0;
	char ch;
	FILE *fp;
	
	while(j<=1){
		if(j==0)	fp=fopen("dict1.dic","r");
		else if(j==1)	fp=fopen("dict2.dic","r");
		if(fp!=NULL){
			i=0;
			while(!feof(fp)){
				fgets(code_orders[i+j*64],34,fp);
				code_orders[i+j*64][32]='\0';
				if(strcmp(code_orders[i+j*64],stop_order)==0)	break;
				i++;
			}	
			fclose(fp);
			order_nums[j]=i;
		}//读入并保存代码段至字符数组order
		j++;
	}
	return ; 
}

unsigned _stdcall run(void* pArguments){
	th *p=(th *)pArguments;
	char OC[9]="\0",orders_16[17];
	int order_flag=1,out_flag=0,i,j,serial=p->ID_thread-1;;	
	int ip=4+serial*256,flag=0,ir=0,AX_1=0,AX_2=0,ax[9][2]={0};//AX_1为前操作对象（指令9~12位），AX_2为后操作对象（13~16位），order_flag判断停机指令 
	/*ax为寄存器数组，ax[0][1]存立即数，ax[1~4][1]对应寄存器1~4，ax[5~8][0]对应寄存器5~8地址，ax[5~8][1]对应寄存器5~8指向的数据*/ 
	for(i=0;i<=order_nums[serial]&&order_flag;i++,ip+=4){//循环执行指令 
		memmove(OC,code_orders[i+serial*64],8);
		AX_1=pick_AX(code_orders[i+serial*64],8);//取出前操作对象 
		AX_2=pick_AX(code_orders[i+serial*64],12);//取出后操作对象 
		ax[0][1]=pick_IN(code_orders[i+serial*64]);//取出立即数 
		memmove(orders_16,code_orders[i+serial*64],16);
		ir=turnto_DEC(orders_16,16);
		
		if(strcmp(OC,"00000000")==0)	order_flag=0;
		else if(strcmp(OC,"00000001")==0){//数据传送指令 
			if(AX_1>=5){
				if(AX_2==0){
					ax[AX_1][0]=ax[AX_2][1];//后操作对象为立即数且前操作对象为地址寄存器，则存入地址区 
					ax[AX_1][1]=data_Segment[(ax[0][1]-16384)/2];//如果立即数修改的是地址寄存器，则需读取立即数代表的地址所指向的内存
				}
				else if(AX_2>0){
					data_Segment[(ax[AX_1][0]-16384)/2]=ax[AX_2][1];
					ax[AX_1][1]=ax[AX_2][1];
				}
			}
			else ax[AX_1][1]=data_Segment[(ax[AX_2][0]-16384)/2];
		}	
		else if(strcmp(OC,"00000010")==0)	ax[AX_1][1]+=ax[AX_2][1];//算数运算指令——加法 
		else if(strcmp(OC,"00000011")==0)	ax[AX_1][1]-=ax[AX_2][1];//算数运算指令——减法 
		else if(strcmp(OC,"00000100")==0)	ax[AX_1][1]*=ax[AX_2][1];//算数运算指令——乘法
		else if(strcmp(OC,"00000101")==0)	ax[AX_1][1]/=ax[AX_2][1];//算数运算指令——除法
		else if(strcmp(OC,"00000110")==0)	ax[AX_1][1]=(ax[AX_1][1]&&ax[AX_2][1]);//逻辑与
		else if(strcmp(OC,"00000111")==0)	ax[AX_1][1]=(ax[AX_1][1]||ax[AX_2][1]);//逻辑或
		else if(strcmp(OC,"00001000")==0){//逻辑非 
			if(AX_2==0)		ax[AX_1][1]=!ax[AX_1][1];//前操作对象为数据寄存器 
			else if(AX_1==0) 	ax[AX_2][1]=!ax[AX_2][1];//后操作对象为地址寄存器 
		} 
		else if(strcmp(OC,"00001001")==0){//比较指令 
			flag=ax[AX_1][1]-ax[AX_2][1];
			if(flag>0) flag=1;
			else if(flag<0) flag=-1;
		}
		else if(strcmp(OC,"00001010")==0&&(AX_2==0||(AX_2==1&&flag==0)||(AX_2==2&&flag==1)||(AX_2==3&&flag==-1))){//跳转指令
			ip+=(ax[0][1]-4);//ip减去立即数+4
			i=(ip-p->Address_thread)/4-1;
		}
		else if(strcmp(OC,"00001011")==0){//输入指令 
			printf("in:\n");
			scanf("%d",&ax[AX_1][1]);
		}
		else if(strcmp(OC,"00001100")==0)	printf("\nID = %d out: %d",serial+1,ax[AX_1][1]);//输出指令
		else if(strcmp(OC,"00001101")==0)	WaitForSingleObject(hMutex_mem,INFINITE);//锁内存 
		else if(strcmp(OC,"00001110")==0)	ReleaseMutex(hMutex_mem);//释放内存 
		else if(strcmp(OC,"00001111")==0)	Sleep(ax[0][1]);//休眠指令 
		WaitForSingleObject(hMutex,INFINITE);
		print_ax_state(serial+1,ip,flag,ir,ax);
		ReleaseMutex(hMutex);
	}
	_endthreadex(0);
	return;
}
