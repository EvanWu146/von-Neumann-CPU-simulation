#include<stdio.h>
#include<string.h>
#include<windows.h>
#include"HeadFiles.h"
#define stop_order "00000000000000000000000000000000"
extern char code_orders[128][34];
extern int data_Segment[256];
extern int order_nums[2];//��һ��Ϊdict1��ָ�������ڶ���Ϊdict2��ָ���� 
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
		}//���벢�����������ַ�����order
		j++;
	}
	return ; 
}

unsigned _stdcall run(void* pArguments){
	th *p=(th *)pArguments;
	char OC[9]="\0",orders_16[17];
	int order_flag=1,out_flag=0,i,j,serial=p->ID_thread-1;;	
	int ip=4+serial*256,flag=0,ir=0,AX_1=0,AX_2=0,ax[9][2]={0};//AX_1Ϊǰ��������ָ��9~12λ����AX_2Ϊ���������13~16λ����order_flag�ж�ͣ��ָ�� 
	/*axΪ�Ĵ������飬ax[0][1]����������ax[1~4][1]��Ӧ�Ĵ���1~4��ax[5~8][0]��Ӧ�Ĵ���5~8��ַ��ax[5~8][1]��Ӧ�Ĵ���5~8ָ�������*/ 
	for(i=0;i<=order_nums[serial]&&order_flag;i++,ip+=4){//ѭ��ִ��ָ�� 
		memmove(OC,code_orders[i+serial*64],8);
		AX_1=pick_AX(code_orders[i+serial*64],8);//ȡ��ǰ�������� 
		AX_2=pick_AX(code_orders[i+serial*64],12);//ȡ����������� 
		ax[0][1]=pick_IN(code_orders[i+serial*64]);//ȡ�������� 
		memmove(orders_16,code_orders[i+serial*64],16);
		ir=turnto_DEC(orders_16,16);
		
		if(strcmp(OC,"00000000")==0)	order_flag=0;
		else if(strcmp(OC,"00000001")==0){//���ݴ���ָ�� 
			if(AX_1>=5){
				if(AX_2==0){
					ax[AX_1][0]=ax[AX_2][1];//���������Ϊ��������ǰ��������Ϊ��ַ�Ĵ�����������ַ�� 
					ax[AX_1][1]=data_Segment[(ax[0][1]-16384)/2];//����������޸ĵ��ǵ�ַ�Ĵ����������ȡ����������ĵ�ַ��ָ����ڴ�
				}
				else if(AX_2>0){
					data_Segment[(ax[AX_1][0]-16384)/2]=ax[AX_2][1];
					ax[AX_1][1]=ax[AX_2][1];
				}
			}
			else ax[AX_1][1]=data_Segment[(ax[AX_2][0]-16384)/2];
		}	
		else if(strcmp(OC,"00000010")==0)	ax[AX_1][1]+=ax[AX_2][1];//��������ָ����ӷ� 
		else if(strcmp(OC,"00000011")==0)	ax[AX_1][1]-=ax[AX_2][1];//��������ָ������� 
		else if(strcmp(OC,"00000100")==0)	ax[AX_1][1]*=ax[AX_2][1];//��������ָ����˷�
		else if(strcmp(OC,"00000101")==0)	ax[AX_1][1]/=ax[AX_2][1];//��������ָ�������
		else if(strcmp(OC,"00000110")==0)	ax[AX_1][1]=(ax[AX_1][1]&&ax[AX_2][1]);//�߼���
		else if(strcmp(OC,"00000111")==0)	ax[AX_1][1]=(ax[AX_1][1]||ax[AX_2][1]);//�߼���
		else if(strcmp(OC,"00001000")==0){//�߼��� 
			if(AX_2==0)		ax[AX_1][1]=!ax[AX_1][1];//ǰ��������Ϊ���ݼĴ��� 
			else if(AX_1==0) 	ax[AX_2][1]=!ax[AX_2][1];//���������Ϊ��ַ�Ĵ��� 
		} 
		else if(strcmp(OC,"00001001")==0){//�Ƚ�ָ�� 
			flag=ax[AX_1][1]-ax[AX_2][1];
			if(flag>0) flag=1;
			else if(flag<0) flag=-1;
		}
		else if(strcmp(OC,"00001010")==0&&(AX_2==0||(AX_2==1&&flag==0)||(AX_2==2&&flag==1)||(AX_2==3&&flag==-1))){//��תָ��
			ip+=(ax[0][1]-4);//ip��ȥ������+4
			i=(ip-p->Address_thread)/4-1;
		}
		else if(strcmp(OC,"00001011")==0){//����ָ�� 
			printf("in:\n");
			scanf("%d",&ax[AX_1][1]);
		}
		else if(strcmp(OC,"00001100")==0)	printf("\nID = %d out: %d",serial+1,ax[AX_1][1]);//���ָ��
		else if(strcmp(OC,"00001101")==0)	WaitForSingleObject(hMutex_mem,INFINITE);//���ڴ� 
		else if(strcmp(OC,"00001110")==0)	ReleaseMutex(hMutex_mem);//�ͷ��ڴ� 
		else if(strcmp(OC,"00001111")==0)	Sleep(ax[0][1]);//����ָ�� 
		WaitForSingleObject(hMutex,INFINITE);
		print_ax_state(serial+1,ip,flag,ir,ax);
		ReleaseMutex(hMutex);
	}
	_endthreadex(0);
	return;
}
