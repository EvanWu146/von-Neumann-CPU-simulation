#include<stdio.h>
#include"HeadFiles.h"
int pick_AX(char *src,int set){//ȡ���Ĵ����������� 
	int i;
	char dest[5]={'\0'};
	for(i=0;i<=3;i++){
		dest[i]=src[i+set];
	}
	dest[4]='\0';//ȡ��4λ�����Ʋ������� 	
	return turnto_DEC(dest,4);//����ʮ���ƵĲ�������Ǻ� 
}
int pick_IN(char *str){//ȡ�������� 
	int i;
	char dest[17];
	for(i=0;i<=15;i++){
		dest[i]=str[i+16];
	}
	dest[16]='\0';//ȡ��16λ������ 
	turnto_TF(dest);//ת��ԭ�� 
	return turnto_DEC(dest,16);//���� ʮ���������� 
}
