#include<stdio.h>
#include<windows.h>
#include<process.h>
#include"HeadFiles.h"
HANDLE hMutex,hMutex_mem;//����Լ��ڴ����Ļ������ 
char code_orders[128][34]={"\0"};//����� 
int data_Segment[256]={100};//���ݶ� 
int order_nums[2];//��һ��Ϊdict1��ָ�������ڶ���Ϊdict2��ָ���� 

int main(void){
	HANDLE hThread1,hThread2;
	unsigned ThreadID1=1,ThreadID2=2;
	th Th1,Th2;
		
	Th1.ID_thread=1;
	Th2.ID_thread=2;
	Th1.Address_thread=codeSEG_1;
	Th2.Address_thread=codeSEG_2;
	loading_orders();//��ʼ�� 
	
	hMutex=CreateMutex(NULL,FALSE,NULL);
	hMutex_mem=CreateMutex(NULL,FALSE,NULL);
	hThread1=(HANDLE) _beginthreadex(NULL,0,run,(void*)&Th1,0,&ThreadID1);
	hThread2=(HANDLE) _beginthreadex(NULL,0,run,(void*)&Th2,0,&ThreadID2);
	WaitForSingleObject(hThread1,INFINITE);
	CloseHandle(hThread1);
	WaitForSingleObject(hThread2,INFINITE);
	CloseHandle(hThread2);
	print_code_and_data();
	return 0;
}

