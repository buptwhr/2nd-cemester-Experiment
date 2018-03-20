#include"����ҵ.h"
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
int Random(PASSENGER passenger,CHECK* OneCheck){//�������ҵ�����ʱ�� 

	return (MinSec+rand()%(MaxSec-MinSec+1));

}


void Recover(CHECK* OneCheck){//��Ϣ�ָ�����������ĳ������ڵ�ַ 
	OneCheck->State=1;//��ĳ�������ڵ�״̬Ϊ1 
}

void EfficiencyChange(CHECK* OneCheck){//Ч�ʸ��� 
	OneCheck->Efficiency=(OneCheck->Efficiency*OneCheck->CheckedPassengers+OneCheck->FirstPassenger->thispassenger.time)/(OneCheck->CheckedPassengers+1);
}

void ExistPassenger(CHECK* OneCheck){//���ڳ˿�ʱ����ڵĲ��� 
	if(OneCheck->Time==0){
		OneCheck->FirstPassenger->thispassenger.time=Random(OneCheck->FirstPassenger->thispassenger,OneCheck);//����ʼ����ʱ��һ���˷���ʱ�� 
		OneCheck->Time=OneCheck->FirstPassenger->thispassenger.time;
	} 
	OneCheck->Time--; 
	if(OneCheck->Time==0){
		//event״̬�ı� 
		if(OneCheck->ListState==0)
		OneCheck->ListState=2;
		else if(OneCheck->ListState==1)
		OneCheck->ListState=3;
		//���û���ˣ��ĳɳ��ˣ�������ˣ��ĳɼȽ��ֳ� 
		EfficiencyChange(OneCheck);//ÿ�г˿���ɰ��죬����Ч�� 
		LineNode* temp=OneCheck->FirstPassenger;//��ʱtemp������׳˿� 
		OneCheck->FirstPassenger=OneCheck->FirstPassenger->next;//�ڶ����˿��������� ��û�о�ΪNULL 
		free(temp);//�ղ��Ǹ����׳˿�free 
		OneCheck->Num--;
		OneCheck->CheckedPassengers++;
		if(OneCheck->Num>0){
		OneCheck->FirstPassenger->thispassenger.time=Random(OneCheck->FirstPassenger->thispassenger,OneCheck);//�Ժ����һ����������������˿���Ϣ������һ������������ʱ�� 
		OneCheck->Time=OneCheck->FirstPassenger->thispassenger.time;
		}			
	}
	
}

void NotExistPassenger(CHECK* OneCheck){//û�г˿�ʱ����ڵĲ��� 
	if(OneCheck->State==3){//�������Ϣ״̬ 
		OneCheck->RestRestTime--;
		OneCheck->AccumulateRestTime++;
		if(OneCheck->RestRestTime==0)
		OneCheck->State=1;
	} 
}

void OneCheckQuitOrNot(CHECK* OneCheck,Buffer* TheBuffer){//������Ƿ��°� 
	if(TheBuffer->Quit==1&&TheBuffer->PeopleWaiting==0&&OneCheck->Num==0)//����������°࣬���������ŶӶ�û�ˣ��͹ر� 
	OneCheck->State=0;
	if(OneCheck->State==4&&OneCheck->Num==0)
	OneCheck->State=0;
}

int OneChecking(CHECK* OneCheck,Buffer* TheBuffer){//������� 
	if(OneCheck->State==0)//�°�״̬��ֱ�ӷ���0 
	return 0;
	if(OneCheck->Num!=0)//����������г˿�
	ExistPassenger(OneCheck);
	else if(OneCheck->Num==0)//���û�˿��� 
	NotExistPassenger(OneCheck);
	OneCheckQuitOrNot(OneCheck,TheBuffer);	 
}

int QuitOrNot(CHECK* Checks){ //�����Ƿ���ȫ���н���  �Ƿ���1 �񷵻�0 
	int sum_state=0;
	for(int i=0;i<=7;i++){
		sum_state+=Checks[i].State;
	}
	if(sum_state==0)
	return 1;
	else
	return 0;
} 

void OutputWin(const char* name,CHECK* OneCheck,int n){//�����ʱ���  ĳһ������ڵ�״̬ ������ȫ�ֱ���T 
	FILE* ptr=NULL;
	ptr=fopen(name,"a+");
	fprintf(ptr,"WIN%d:State=",n+1);
	switch(OneCheck[n].State){
		case 0:fprintf(ptr,"δ���� ");
		break;
		case 1:
		fprintf(ptr,"������ ");
		if(OneCheck[n].Num!=0)	
		fprintf(ptr,"ServLen=%d/%d ",(OneCheck[n].FirstPassenger->thispassenger.time)-(OneCheck[n].Time),OneCheck[n].FirstPassenger->thispassenger.time);	

		break;
		case 2:fprintf(ptr,"��Ϣ������ ");
		if(OneCheck[n].Num!=0)
		fprintf(ptr,"ServLen=%d/%d ",(OneCheck[n].FirstPassenger->thispassenger.time)-(OneCheck[n].Time),OneCheck[n].FirstPassenger->thispassenger.time);	
		
		break;
		case 3:
		if(OneCheck[n].Num!=0){
		fprintf(ptr,"����׼��Ϣ ");
		fprintf(ptr,"ServLen=%d/%d ",(OneCheck[n].FirstPassenger->thispassenger.time)-(OneCheck[n].Time),OneCheck[n].FirstPassenger->thispassenger.time);		
		}
		else{
		fprintf(ptr,"��Ϣ�� "); 
		fprintf(ptr,"RestLen=%d/%d ",(OneCheck[n].ToRest)-(OneCheck[n].RestRestTime),OneCheck[n].ToRest);
		}
		break; 
		case 4: fprintf(ptr,"׼�����ܹر� ");
		if(OneCheck[n].Num!=0)
		fprintf(ptr,"ServLen=%d/%d ",(OneCheck[n].FirstPassenger->thispassenger.time)-(OneCheck[n].Time),OneCheck[n].FirstPassenger->thispassenger.time);	
		break;
	} 
	switch(OneCheck[n].IsApplyRest){
		case 1:
			fprintf(ptr,"��׼��Ϣ ");
			OneCheck[n].IsApplyRest=0;
			break; 
		case 2:
			fprintf(ptr,"�ܾ���Ϣ���� ");
			OneCheck[n].IsApplyRest=0;
			break; 
	}
	
	fprintf(ptr,"ʣ��˿���%d ",OneCheck[n].Num);
	if(OneCheck[n].State!=0)
	switch(OneCheck[n].ListState){
		case 0:fprintf(ptr,"�����û�н��� ");break;
		case 1:fprintf(ptr,"�г˿ͽ��� ");break;
		case 2:fprintf(ptr,"�г˿��뿪 ");break;
		case 3:fprintf(ptr,"�г˿ͽ�����뿪 ");break;
	}
	fprintf(ptr,"�ۼƷ���˿���%d\n",OneCheck[n].CheckedPassengers);
	fclose(ptr);
} 

void Pseudoraphic(const char* name,Buffer* TheBuffer){//αͼ�λ� 
	FILE* ptr=NULL;
	ptr=fopen(name,"a+");
	for(int i=0;i<((TheBuffer->LinesWaiting)-1);i++){
		for(int j=0;j<MaxCustSingleLine;j++){
			fprintf(ptr,"��"); 
		}
		fprintf(ptr,"\n");
	}
	if((TheBuffer->LinesWaiting)%2==1){
		for(int i=0;i<(TheBuffer->PeopleInLine);i++)
		fprintf(ptr,"��");
		fprintf(ptr,"\n"); 
	}
	else if((TheBuffer->LinesWaiting)%2==0&&(TheBuffer->LinesWaiting)!=0){
		for(int j=0;j<(MaxCustSingleLine-(TheBuffer->PeopleInLine));j++)
		fprintf(ptr,"  ");
		for(int i=0;i<TheBuffer->PeopleInLine;i++)
		fprintf(ptr,"��");
		fprintf(ptr,"\n"); 
	}
	
	fclose(ptr);
} 

void OutputFunc(const char* name,EVENT event,CHECK* OneCheck,Buffer* TheBuffer){//������� 
	FILE* ptr=NULL;
	ptr=fopen(name,"a+");
	fprintf(ptr,"T=<%d>\n",CurrentTime);
	if(TheBuffer->Quit==1)
		fprintf(ptr,"�������°� ");
	else
		fprintf(ptr,"���������� "); 
	fprintf(ptr,"������������%d �������ܳ˿�����%d ",TheBuffer->LinesWaiting,TheBuffer->PeopleWaiting);
	fprintf(ptr,"ƽ���ȴ�ʱ��%.2lf ",TheBuffer->AverageWaitingTime); 
	fprintf(ptr,"�ۼƷ���˿���%d\n",TheBuffer->PeopleOut);
	if(CurrentTime==event.EventTime){//������ʱ���յ����¼����� 
	fprintf(ptr,"%d���˿ͽ��뻺���� %d���˿ͱ��ܾ����� ",event.PeopleAdd,event.PeopleLeft);	
	if(event.Quit==1)
	fprintf(ptr,"���յ��°����� ");
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
				else fprintf(ptr,"��%d",i+1);
				
			}		
		}
		fprintf(ptr,"�Ű����������Ϣ\n"); 
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
				else fprintf(ptr,"��%d",i+1);
				
			}		
		}
		fprintf(ptr,"�Ű��������ָ�\n"); 
	}	
	fclose(ptr); 
	for(int i=0;i<=7;i++)
	OutputWin(name,OneCheck,i);
	Pseudoraphic(name,TheBuffer);
	return;
	
}

int TheFirstUnopenCheck(CHECK* OneCheck){//Ѱ�ҵ���һ���رյİ���ڣ������������ ,����8��ȫ������ 
	int i;
	for(i=4;OneCheck[i].State!=0&&(i!=8)&&OneCheck[i].State!=4;i++);
	return i;
}

void IntelligenctControl(CHECK* OneCheck,Buffer* TheBuffer){//1-4һֱ���ŵ��°࣬5-8���ܿ��� 
	if((TheBuffer->PeopleWaiting)/(TheFirstUnopenCheck(OneCheck)+1)>(MaxSeqLen)){//����������������Կ��ŵİ����������2����MasSeqLen 
		if(TheFirstUnopenCheck(OneCheck)<8)
		OneCheck[TheFirstUnopenCheck(OneCheck)].State=1;
	}
	else{
		if((TheBuffer->PeopleWaiting)/(TheFirstUnopenCheck(OneCheck)-1)>MaxSeqLen&&TheFirstUnopenCheck(OneCheck)>4){//���С�� �͹�һ�� 
		OneCheck[(TheFirstUnopenCheck(OneCheck))-1].State=4;
		}
	}
} 
double ZeroOneRandom()  //����һ��0��1������� ���ɷֲ���Ҫ 
{
        double f;
        f = (float)(rand() % 100);
        return (f+1)/100;
}
int Possion(int expectation) //��expectationΪ���� ���ղ��ɷֲ�ģ�����ɳ˿��� 
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
