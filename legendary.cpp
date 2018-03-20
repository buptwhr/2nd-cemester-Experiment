#include"大作业.h"
int Random(PASSENGER passenger,CHECK* OneCheck);
void Recover(CHECK* OneCheck);
void EfficiencyChange(CHECK* OneCheck);
void ExistPassenger(CHECK* OneCheck);
void NotExistPassenger(CHECK* OneCheck);
void OneCheckQuitOrNot(CHECK* OneCheck,Buffer* TheBuffer);
int OneChecking(CHECK* OneCheck,Buffer* TheBuffer);
int QuitOrNot(CHECK* Checks);
void OutputWin(const char* name,CHECK* OneCheck,int n);
void Pseudoraphic(const char* name,Buffer* TheBuffer);
void OutputFunc(const char* name,EVENT event,CHECK* OneCheck,Buffer* TheBuffer);
int TheFirstUnopenCheck(CHECK* OneCheck);
void IntelligenctControl(CHECK* OneCheck,Buffer* TheBuffer);
double ZeroOneRandom();
int possion(int expectation); 
int Random(PASSENGER passenger,CHECK* OneCheck){//随机生成业务办理时间 

	return (MinSec+rand()%(MaxSec-MinSec+1));

}


void Recover(CHECK* OneCheck){//休息恢复工作，参数某个安检口地址 
	OneCheck->State=1;//改某这个安检口的状态为1 
}

void EfficiencyChange(CHECK* OneCheck){//效率更改 
	OneCheck->Efficiency=(OneCheck->Efficiency*OneCheck->CheckedPassengers+OneCheck->FirstPassenger->thispassenger.time)/(OneCheck->CheckedPassengers+1);
}

void ExistPassenger(CHECK* OneCheck){//存在乘客时安检口的操作 
	if(OneCheck->Time==0){
		OneCheck->FirstPassenger->thispassenger.time=Random(OneCheck->FirstPassenger->thispassenger,OneCheck);//给开始工作时第一个人分配时间 
		OneCheck->Time=OneCheck->FirstPassenger->thispassenger.time;
	} 
	OneCheck->Time--; 
	if(OneCheck->Time==0){
		//event状态改变 
		if(OneCheck->ListState==0)
		OneCheck->ListState=2;
		else if(OneCheck->ListState==1)
		OneCheck->ListState=3;
		//如果没进人，改成出人，如果进人，改成既进又出 
		EfficiencyChange(OneCheck);//每有乘客完成安检，更改效率 
		LineNode* temp=OneCheck->FirstPassenger;//临时temp代表队首乘客 
		OneCheck->FirstPassenger=OneCheck->FirstPassenger->next;//第二个乘客来到队首 ，没有就为NULL 
		free(temp);//刚才那个队首乘客free 
		OneCheck->Num--;
		OneCheck->CheckedPassengers++;
		if(OneCheck->Num>0){
		OneCheck->FirstPassenger->thispassenger.time=Random(OneCheck->FirstPassenger->thispassenger,OneCheck);//以后定义的一个函数，传入这个乘客信息，按照一定规则分配办理时间 
		OneCheck->Time=OneCheck->FirstPassenger->thispassenger.time;
		}			
	}
	
}

void NotExistPassenger(CHECK* OneCheck){//没有乘客时安检口的操作 
	if(OneCheck->State==3){//如果是休息状态 
		OneCheck->RestRestTime--;
		OneCheck->AccumulateRestTime++;
		if(OneCheck->RestRestTime==0)
		OneCheck->State=1;
	} 
}

void OneCheckQuitOrNot(CHECK* OneCheck,Buffer* TheBuffer){//安检口是否下班 
	if(TheBuffer->Quit==1&&TheBuffer->PeopleWaiting==0&&OneCheck->Num==0)//如果缓冲区下班，缓冲区与排队都没人，就关闭 
	OneCheck->State=0;
	if(OneCheck->State==4&&OneCheck->Num==0)
	OneCheck->State=0;
}

int OneChecking(CHECK* OneCheck,Buffer* TheBuffer){//安检控制 
	if(OneCheck->State==0)//下班状态，直接返回0 
	return 0;
	if(OneCheck->Num!=0)//如果队伍里有乘客
	ExistPassenger(OneCheck);
	else if(OneCheck->Num==0)//如果没乘客了 
	NotExistPassenger(OneCheck);
	OneCheckQuitOrNot(OneCheck,TheBuffer);	 
}

int QuitOrNot(CHECK* Checks){ //程序是否完全运行结束  是返回1 否返回0 
	int sum_state=0;
	for(int i=0;i<=7;i++){
		sum_state+=Checks[i].State;
	}
	if(sum_state==0)
	return 1;
	else
	return 0;
} 

void OutputWin(const char* name,CHECK* OneCheck,int n){//输出该时间点  某一个安检口的状态 ，利用全局变量T 
	FILE* ptr=NULL;
	ptr=fopen(name,"a+");
	fprintf(ptr,"WIN%d:State=",n+1);
	switch(OneCheck[n].State){
		case 0:fprintf(ptr,"未工作 ");
		break;
		case 1:
		fprintf(ptr,"工作中 ");
		if(OneCheck[n].Num!=0)	
		fprintf(ptr,"ServLen=%d/%d ",(OneCheck[n].FirstPassenger->thispassenger.time)-(OneCheck[n].Time),OneCheck[n].FirstPassenger->thispassenger.time);	

		break;
		case 2:fprintf(ptr,"休息再申请 ");
		if(OneCheck[n].Num!=0)
		fprintf(ptr,"ServLen=%d/%d ",(OneCheck[n].FirstPassenger->thispassenger.time)-(OneCheck[n].Time),OneCheck[n].FirstPassenger->thispassenger.time);	
		
		break;
		case 3:
		if(OneCheck[n].Num!=0){
		fprintf(ptr,"已批准休息 ");
		fprintf(ptr,"ServLen=%d/%d ",(OneCheck[n].FirstPassenger->thispassenger.time)-(OneCheck[n].Time),OneCheck[n].FirstPassenger->thispassenger.time);		
		}
		else{
		fprintf(ptr,"休息中 "); 
		fprintf(ptr,"RestLen=%d/%d ",(OneCheck[n].ToRest)-(OneCheck[n].RestRestTime),OneCheck[n].ToRest);
		}
		break; 
		case 4: fprintf(ptr,"准备智能关闭 ");
		if(OneCheck[n].Num!=0)
		fprintf(ptr,"ServLen=%d/%d ",(OneCheck[n].FirstPassenger->thispassenger.time)-(OneCheck[n].Time),OneCheck[n].FirstPassenger->thispassenger.time);	
		break;
	} 
	switch(OneCheck[n].IsApplyRest){
		case 1:
			fprintf(ptr,"批准休息 ");
			OneCheck[n].IsApplyRest=0;
			break; 
		case 2:
			fprintf(ptr,"拒绝休息请求 ");
			OneCheck[n].IsApplyRest=0;
			break; 
	}
	
	fprintf(ptr,"剩余乘客数%d ",OneCheck[n].Num);
	if(OneCheck[n].State!=0)
	switch(OneCheck[n].ListState){
		case 0:fprintf(ptr,"安检口没有进出 ");break;
		case 1:fprintf(ptr,"有乘客进入 ");break;
		case 2:fprintf(ptr,"有乘客离开 ");break;
		case 3:fprintf(ptr,"有乘客进入和离开 ");break;
	}
	fprintf(ptr,"累计服务乘客数%d\n",OneCheck[n].CheckedPassengers);
	fclose(ptr);
} 

void Pseudoraphic(const char* name,Buffer* TheBuffer){//伪图形化 
	FILE* ptr=NULL;
	ptr=fopen(name,"a+");
	for(int i=0;i<((TheBuffer->LinesWaiting)-1);i++){
		for(int j=0;j<MaxCustSingleLine;j++){
			fprintf(ptr,"〇"); 
		}
		fprintf(ptr,"\n");
	}
	if((TheBuffer->LinesWaiting)%2==1){
		for(int i=0;i<(TheBuffer->PeopleInLine);i++)
		fprintf(ptr,"〇");
		fprintf(ptr,"\n"); 
	}
	else if((TheBuffer->LinesWaiting)%2==0&&(TheBuffer->LinesWaiting)!=0){
		for(int j=0;j<(MaxCustSingleLine-(TheBuffer->PeopleInLine));j++)
		fprintf(ptr,"  ");
		for(int i=0;i<TheBuffer->PeopleInLine;i++)
		fprintf(ptr,"〇");
		fprintf(ptr,"\n"); 
	}
	
	fclose(ptr);
} 

void OutputFunc(const char* name,EVENT event,CHECK* OneCheck,Buffer* TheBuffer){//输出函数 
	FILE* ptr=NULL;
	ptr=fopen(name,"a+");
	fprintf(ptr,"T=<%d>\n",CurrentTime);
	if(TheBuffer->Quit==1)
		fprintf(ptr,"缓冲区下班 ");
	else
		fprintf(ptr,"缓冲区开放 "); 
	fprintf(ptr,"缓冲区列数：%d 缓冲区总乘客数：%d ",TheBuffer->LinesWaiting,TheBuffer->PeopleWaiting);
	fprintf(ptr,"平均等待时间%.2lf ",TheBuffer->AverageWaitingTime); 
	fprintf(ptr,"累计服务乘客数%d\n",TheBuffer->PeopleOut);
	if(CurrentTime==event.EventTime){//如果这个时刻收到了事件请求 
	fprintf(ptr,"%d名乘客进入缓冲区 %d名乘客被拒绝进入 ",event.PeopleAdd,event.PeopleLeft);	
	if(event.Quit==1)
	fprintf(ptr,"接收到下班请求 ");
	}
	fprintf(ptr,"\n"); 
		int j;
	for(int i=0;i<8;i++){
		if(event.ApplyRest[i]==1)
		j++;
	}
	if(j>0){
		for(int i=0,n=0;i<8;i++){
			if(event.ApplyRest[i]==1){
				n++;
				if(n==1)
				fprintf(ptr,"%d",i+1);
				else fprintf(ptr,"、%d",i+1);
				
			}		
		}
		fprintf(ptr,"号安检口申请休息\n"); 
	}	
	j=0;
	for(int i=0;i<8;i++){
		if(event.CancelRest[i]==1)
		j++;
	}
	if(j>0){
		for(int i=0,n=0;i<8;i++){
			if(event.CancelRest[i]==1){
				n++;
				if(n==1)
				fprintf(ptr,"%d",i+1);
				else fprintf(ptr,"、%d",i+1);
				
			}		
		}
		fprintf(ptr,"号安检口申请恢复\n"); 
	}	
	fclose(ptr); 
	for(int i=0;i<=7;i++)
	OutputWin(name,OneCheck,i);
	Pseudoraphic(name,TheBuffer);
	return;
	
}

int TheFirstUnopenCheck(CHECK* OneCheck){//寻找到第一个关闭的安检口，返回数组序号 ,返回8则全部开放 
	int i;
	for(i=4;OneCheck[i].State!=0&&(i!=8)&&OneCheck[i].State!=4;i++);
	return i;
}

void IntelligenctControl(CHECK* OneCheck,Buffer* TheBuffer){//1-4一直开放到下班，5-8智能开放 
	if((TheBuffer->PeopleWaiting)/(TheFirstUnopenCheck(OneCheck)+1)>(MaxSeqLen)){//如果缓冲区人数除以开放的安检口数大于2倍的MasSeqLen 
		if(TheFirstUnopenCheck(OneCheck)<8)
		OneCheck[TheFirstUnopenCheck(OneCheck)].State=1;
	}
	else{
		if((TheBuffer->PeopleWaiting)/(TheFirstUnopenCheck(OneCheck)-1)>MaxSeqLen&&TheFirstUnopenCheck(OneCheck)>4){//如果小于 就关一个 
		OneCheck[(TheFirstUnopenCheck(OneCheck))-1].State=4;
		}
	}
} 
double ZeroOneRandom()  //产生一个0到1的随机数 泊松分布需要 
{
        double f;
        f = (float)(rand() % 100);
        return (f+1)/100;
}
int Possion(int expectation) //以expectation为期望 按照泊松分布模型生成乘客数 
{
        int Lambda = expectation, k = 0;
        long double p = 1.0;
        long double l=exp(-Lambda);
        
        while (p>=l)
        {
                double u = ZeroOneRandom();
                p *= u;
                k++;
        }
        return k-1;
}
