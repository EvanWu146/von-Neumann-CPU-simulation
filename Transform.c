#include<stdio.h>
#include"HeadFiles.h"
int turnto_DEC(char *str,int length){//��������תΪʮ���� 
	int i,sum=0,i_2=1,end;
	if(length==4)	end=0;
	else	end=1;
	for(i=length-1;i>=end;i--){
		sum+=(str[i]-'0')*i_2;
		i_2*=2;
	}//�������ֵ 
	if(str[0]=='0'||length==4) return sum;
	else if(str[0]=='1') return -sum-1;//��������λ�����������һ 
}
void turnto_TF(char *str){//�������Ʋ����λ��ȡ�� 
	int flag,i;
	if(str[0]=='1'){
		for(i=1;i<=15;i++){
			if(str[i]=='0') str[i]='1';
			else if(str[i]=='1') str[i]='0';
		}//������λȡ��
	}
}
