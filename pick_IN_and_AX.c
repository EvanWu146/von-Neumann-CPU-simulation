#include<stdio.h>
#include"HeadFiles.h"
int pick_AX(char *src,int set){//取出寄存器操作对象 
	int i;
	char dest[5]={'\0'};
	for(i=0;i<=3;i++){
		dest[i]=src[i+set];
	}
	dest[4]='\0';//取出4位二进制操作对象 	
	return turnto_DEC(dest,4);//返回十进制的操作对象记号 
}
int pick_IN(char *str){//取出立即数 
	int i;
	char dest[17];
	for(i=0;i<=15;i++){
		dest[i]=str[i+16];
	}
	dest[16]='\0';//取出16位立即数 
	turnto_TF(dest);//转成原码 
	return turnto_DEC(dest,16);//返回 十进制立即数 
}
