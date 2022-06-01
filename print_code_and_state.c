#include<stdio.h>
#include<windows.h>
#include<process.h>
#include"HeadFiles.h"
extern char code_orders[128][34];
extern int data_Segment[256];
void print_ax_state(int ID,int ip,int flag,int ir,int ax[][2]){//输出寄存器状态
	int j;
	
	printf("\nID = %d\nip = %d\nflag = %d\nir = %d\n",ID,ip,flag,ir);
	for(j=1;j<=4;j++){
		printf("ax%d = %d",j,ax[j][1]);
		if(j==4) printf("\n");
		else printf(" ");
	}
	for(j=5;j<=8;j++) {
		printf("ax%d = %d",j,ax[j][0]);
		if(j==8) printf("\n");
		else printf(" ");
	}
	
}
void print_code_and_data(){//输出代码段和数据段 
	int i;
	printf("\ncodeSegment :\n");
	for(i=0;i<=127;i++){
		printf("%d ",turnto_DEC(code_orders[i],32));
		if((i+1)%8==0) printf("\n");
	}
	printf("\ndataSegment :\n");
	for(i=0;i<=255;i++){
		printf("%d ",data_Segment[i]);
		if((i+1)%16==0) printf("\n");
	}
}
