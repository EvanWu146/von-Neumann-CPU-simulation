#include<stdio.h>
#include"HeadFiles.h"
int turnto_DEC(char *str,int length){//将二进制转为十进制 
	int i,sum=0,i_2=1,end;
	if(length==4)	end=0;
	else	end=1;
	for(i=length-1;i>=end;i--){
		sum+=(str[i]-'0')*i_2;
		i_2*=2;
	}//计算绝对值 
	if(str[0]=='0'||length==4) return sum;
	else if(str[0]=='1') return -sum-1;//分析符号位，负数则需减一 
}
void turnto_TF(char *str){//将二进制补码各位数取反 
	int flag,i;
	if(str[0]=='1'){
		for(i=1;i<=15;i++){
			if(str[i]=='0') str[i]='1';
			else if(str[i]=='1') str[i]='0';
		}//除符号位取反
	}
}
