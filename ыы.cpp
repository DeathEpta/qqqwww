#include <SPI.h>                                          // ���������� ���������� ��� ������ � ����� SPI
#include <nRF24L01.h>                                     // ���������� ���� �������� �� ���������� RF24
#include <RF24.h>                                         // ���������� ���������� ��� ������ � nRF24L01+
RF24           radio(9, 10);                              // ������ ������ radio ��� ������ � ����������� RF24, �������� ������ ������� nRF24L01+ (CE, CSN)
int            mapData[4][4];  // �����, ������� �� ��������� �� ������ �������
int            comand;       
int mySpottedDirects;
  int mapp[4][4];//�����, ������� �������������� �������(��� �����)
const int varX[] = {1, 0, -1, 0 };
const int varY[] = {0, 1,  0, -1}; 
int queue[25];
bool isStepMod = 0;
int coordX = 0;
int coordY = 0;
int direct = 0;
int mycomand = 0;

int coordXold = 0;
int coordYold = 0;
int robot_id = 3;




void setup() {
      delay(1000);
   radio.begin();                                        // ���������� ������ nRF24L01+
    radio.setChannel(5);                                  // ��������� ����� ����� ������ (�� 0 �� 127), 5 - ������ ���� ������ �������������� �� ������� 2,405 ��� (�� ����� ������ ����� ���� ������ 1 ������� � �� 6 ������������)
    radio.setDataRate     (RF24_1MBPS);                   // ��������� �������� �������� ������ (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1����/���
    radio.setPALevel      (RF24_PA_HIGH);                 // ��������� �������� ����������� (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
    radio.openReadingPipe (1, 0x1234567890LL);
    radio.openWritingPipe ( 0x1234567890LL);// ��������� 1 ����� � ��������������� 0x1234567890 ��� ������ ������ (�� ����� ������ ����� ���� ������� �� 6 ������ ����, ������� ������ ���������� ������ ��������� ������ ��������������)
   
  // put your setup code here, to run once:  
}




void loop() {
  // put your main code here, to run repeatedly:

}





void coordinator()//����� ����������� ������ �� �����
{
  for(int i = 0; i < 4; i++)
    {
      if(varX[i] == coordX - coordXold && varY[i] == coordY - coordYold)
        direct = i;
    }
}




void Move(int direct, bool lool/*��������� �������� �� �� ��� ������ �����.*/)
{
  if(lool)
  {
    if(direct == 1)
    {
     analogWrite(5, 0);
     analogWrite(6, 0);
    }
    if(direct == 0)
    {
     digitalWrite(4, HIGH);
     digitalWrite(7, HIGH);
     analogWrite(5, 255);
     analogWrite(6, 255);
    }
  }
  if(!lool)
  {
    if(direct>0)
    {
      for(int j = 0; j < direct; j++)
      {
        digitalWrite(4, LOW);
        digitalWrite(7, HIGH);
        analogWrite(5, 255);
        analogWrite(6, 255);
        //encoder oneLove :* <3
      }
    }
    if(direct<0)
    {
      for(int j = 0; j < direct; j++)
      {
        digitalWrite(4, HIGH);
        digitalWrite(7, LOW);
        analogWrite(5, 255);
        analogWrite(6, 255);
        //encoder oneLove :* <3
      }
    }
  }
}




void check()//������ ����� � ��������� ������ �������
{
  for(int i = 0; i < 4; i++)
  {
    if(coordX + varX[i] < 4 && coordX + varX[i] > -1 && coordY + varY[i] < 4 && coordY + varY[i] > -1 && mapp[coordX + varX[i]][coordY + varY[i]] == 0 )
    {
      int q = direct-i;//                   }
      if(abs(q)>2&&q!=0)Move(-(q)%2,0);//    }  ������� ��� ���� ������ �����.
      else if(q!=0)Move(q,0);//             }
      if(digitalRead(A0) == 1)
       mapp[coordX + varX[i]][coordY + varY[i]] == 1;
       else
        mapp[coordX + varX[i]][coordY + varY[i]] == 2;
    //                                                     ������� ���� of ����� ����� � ������ �����
         
    }
    
  }
}



void leftHand()//�������� ������� ��������� ��������� �� ������� ����� ����
{
  int way = (direct+3)%4;
   coordXold = coordX;
   coordYold = coordY;
     
  {
     if(mapp[coordX + varX[way]][coordY+varY[way]] == 1)
     {
      int q = way;
      if(abs(q)>2&&q!=0)Move(-(q)%2,0);
      else if(q!=0)Move(q,0);
      Move(0,1);
      coordX = coordX + varX[way];
      coordY = coordY + varY[way]; 
     }
     else 
     {
      if(mapp[coordX + varX[direct]][coordY+varY[direct]] == 1)
      {
        Move(0,1);
      coordX = coordX + varX[direct];
      coordY = coordY + varY[direct]; 
          
      }
      else
      {
        if(mapp[coordX + varX[(direct+1)%4]][coordY+varY[(direct+1)%4]] == 1)
        {
          int q = (direct+1)%4;
          if(abs(q)>2&&q!=0)Move(-(q)%2,0);
          else if(q!=0)Move(q,0);
          Move(0,1);
             coordX = coordX + varX[(direct+1)%4];
             coordY = coordY + varY[(direct+1)%4]; 
   
        }
        else
        {
          Move(2,0);
          Move(0,1);
             coordX = coordXold + coordX;
             coordXold = coordX - coordXold;
             coordX = coordX - coordXold;
             
             coordY = coordYold + coordY;
             coordYold = coordY - coordYold;
             coordY = coordY - coordYold; 
   
        }
      }
     }
     
     
  }  
}       
//��������� ������ �� �����
void recieve()
{
  
        radio.startListening  ();            
                                    // ���� � ������ ������� �������� ������
        radio.read(&comand, sizeof(comand));                  // ������ ������ � ������ data � ��������� ������� ���� ������
        radio.read(&mapData, sizeof(mapData)); 
    /*if(comand == 1)
      function();
      */
      
}

void makemap()//������� ����� ����� � ����������.
{
  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
    {
      if(mapp[i][j] != mapData[i][j] && mapData[i][j] != 0 && mapData[i][j] != 3 )
        mapp[i][j] = mapData[i][j];  
    }
}

void transmite()//�������� ������ �� ������� ������
  {
    radio.stopListening  ();           
        radio.write(&mycomand, sizeof(mycomand));
        radio.write(&mapp, sizeof(mapp)); 
  }

void searchLines()
{
  bool chek=false;
	int crd1X,crd1Y,crd2X,crd2Y;
	int step = 100;
	int used[4][4];
	int search[4][4];
	int used2[4][4];
	int q = 0;
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			if(mapp[i][j]!=1)search[i][j] = -1;
			else search[i][j] = 0;
		}
	}
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			used[i][j] = search[i][j]; 
	search[coordX][coordY]=0;
	used[coordX][coordY]=2;//��� �� ������ ����(��������)
	while(true)
	{
		chek = false;
		q++;
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
			{
				if(used[i][j]==2)
				{
					chek=true;
					for(int w = 0; w < 4; w++)
					{
						if(i+ varX[w] < 4 && i + varX[w] > -1 && j+ varY[w] < 4 && j + varY[w] > -1 && used[i + varX[w]][j + varY[w]] == 0)
						{
							search[i + varX[w]][j + varY[w]] = q;
							used2[i + varX[w]][j + varY[w]] = 2;
						} 
					}
					used[i][j] = -1;
				}
			}
			for(int i = 0; i < 4; i++)
				for(int j = 0; j < 4; j++)//������� �� ��������� �������(q++) ������� used
				{
					if(used2[i][j]==2)
					{
						used[i][j]=2;
						used2[i][j]=0;
					}
				}

				/*			cout << "\n\n";
				for(int i = 0; i < 4; i++)
				{
				for(int j = 0; j < 4; j++)
				cout << search[i][j];
				cout << "\n";
				}*/
				if(!chek)break;
	}

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)//������� ���� �� �����
		{
			if(mapp[i][j]==-2){crd1X=i;crd1Y=j;}
		}
	}

	for(int i = 0; i < 4; i++){
		if(mapp[crd1X+varX[i]][crd1Y+varY[i]]>2)crd1X+=varX[i]; crd1Y+=varY[i];}// ��������� ���������� �� ������, ����� � ��������

	if(/*����������� ����������� = 0*/)crd2X=crd1X+2;
	if(/*����������� ����������� = 1*/)crd2Y=crd1y+2;      //������� �����, �������� 1�� ������(����� ����)
	if(/*����������� ����������� = 2*/)crd2X=crd1X-2;
	if(/*����������� ����������� = 3*/)crd2Y=crd1Y-2;

	if(/*����������� ����������� = 0*/)crd1X-=1;
	if(/*����������� ����������� = 1*/)crd1Y-=1;      //������� �����, ������ 1�� ������
	if(/*����������� ����������� = 2*/)crd1X+=1;
	if(/*����������� ����������� = 3*/)crd1Y+=1;
	//crd2X=crd1X;crd2Y=crd1Y-1; //����� ���� ��� �������� ���� �� -2
	if(search[crd2X][crd2Y]>0)//����� �������� ������ � ������ ������� �����. �������� � crd2 XY
	{
		step=search[crd2X][crd2Y];
		search[crd2X][crd2Y]=-2;
		while(true)//������ ���� �� -2 �� ����� �� ����������� ������
		{
			if(step==0)break;
			for(int i = 0; i < 4; i++)
			{
				if(search[crd2X+varX[i]][crd2Y+varY[i]]==step-1){search[crd2X][crd2Y]=-2;crd2X+=varX[i];crd2Y+=varY[i];}
			}
			step--;
		}

		while(true)//����� ���� �� ������ ����������
		{
			if(chek)break;
			chek=true;
			for(int i = 0; i < 4; i++)
			{
				if(search[coordX+varX[i]][coordY+varY[i]]==-2)//�������������� �� -2 � ���� � ��� ������.
				{
					int z = direct-i;
					if(abs(z)>2&&z!=0)Move(-(z)%2,0);
					else if(z!=0)Move(z,0);  
					if(z==0)Move(0,1);
					chek=false;
				}
			}
			search[coordX+varX[i]][coordY+varY[i]]=0;
		}
		for(int i = 0; i < 4; i++)
		{
			if(mapp[coordX+varX[i]][coordY+varY[i]]==-2)//�������������� ����� � �����.
			{
				int z = direct-i;
				if(abs(z)>2&&z!=0)Move(-(z)%2,0);
				else if(z!=0)Move(z,0);  
			}
		}
	}
	else //����� ����� ������. �������� � crd1 XY
	{
		step=search[crd1X][crd1Y];
		search[crd1X][crd1Y]=-2;

		while(true)//������ ���� �� -2 �� ������� ����� �� ����������� ������
		{
			if(step==0)break;
			for(int i = 0; i < 4; i++)
			{
				if(search[crd1X+varX[i]][crd1Y+varY[i]]==step-1){search[crd1X][crd1Y]=-2;crd1X+=varX[i];crd1Y+=varY[i];}
			}
			step--;
		}
		while(true)//����� ���� �� ������ ����������
		{
			if(chek)break;
			chek=true;
			for(int i = 0; i < 4; i++)
			{
				if(search[coordX+varX[i]][coordY+varY[i]]==-2)//�������������� �� -2 � ���� � ��� ������.
				{
					int z = direct-i;
					if(abs(z)>2&&z!=0)Move(-(z)%2,0);
					else if(z!=0)Move(z,0);  
					if(z==0)Move(0,1);
					chek=false;
				}
			}
			search[coordX+varX[i]][coordY+varY[i]]=0;
		}
		for(int i = 0; i < 4; i++)
		{
			if(mapp[coordX+varX[i]][coordY+varY[i]]>2)//�������������� ����� �� 2�� ������.
			{
				int z = direct-i;
				if(abs(z)>2&&z!=0)Move(-(z)%2,0);
				else if(z!=0)Move(z,0);  
			}
		}
	}
  
}