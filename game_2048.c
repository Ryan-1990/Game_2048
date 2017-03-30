#include "game_console.h" 
#include "FM25L16.h"


unsigned int x[4][4]={0};
unsigned int x1[4][4]={0};
unsigned int score=0;

void start(void)
{
    unsigned char num=0;
    unsigned char t[4][4]={0};
	unsigned char r1=0,r2=0;
	unsigned char i=0,j=0;

    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            if(x[i][j]==0)
            {
                num++;
                t[i][j]=num;
            }
        }
    }

    r1=TCNT2%num+1;
    r2=TCNT2%8;

    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            if(t[i][j]==r1)
            {
                if(r2==0){x[i][j]=4;break;}
                if(r2!=0){x[i][j]=2;break;}
            }
        }
    }
}

unsigned char check(void)
{
    unsigned char flag=1;
	unsigned char i=0;
	
	for(i=0;i<16;i++)
    {
        if(x[i/4][i%4]==2048)
        {
            flag=2;
            break;
        }
    }
    if(flag==1)
	{
	    for(i=0;i<9;i++)
	    {
	        if(x[i/3][i%3]==x[i/3][i%3+1] || x[i/3][i%3]==x[i/3+1][i%3] || x[i/3][i%3]==0)
	        {flag=0;break;}
	        if(x[3][i%3]==x[3][i%3+1] || x[i/3][3]==x[i/3+1][3] || x[3][i%3]*x[i/3][3]*x[3][3]==0)
	        {flag=0;break;}
	    }
	}
	return (flag);
}

unsigned char compare(void)
{
	unsigned char i=0,j=0;
	unsigned char f=0;

    for(i=0;i<4;i++)
        for(j=0;j<4;j++)
            if(x[i][j]!=x1[i][j])
            {
                f=1;
                break;
            }
	return (f);
}

void move(unsigned char way)
{
    unsigned char i=0,j=0,m=0,k=0;

	for(i=0;i<4;i++)
        for(j=0;j<4;j++)
            x1[i][j]=x[i][j];
	
	if(way==1)//right
    {
        for(i=0;i<4;i++)
        for(m=0;m<3;m++)
            for(j=3;j>0;j--)
            {
                if(x[i][j]==0)
                {
                    for(k=j;k>0;k--)
                        x[i][k]=x[i][k-1];
                    x[i][0]=0;
                }
            }
        for(i=0;i<4;i++)
        {
            for(j=3;j>0;j--)
            {
                if(x[i][j]==x[i][j-1])
                {
                    x[i][j]=x[i][j]+x[i][j-1];
                    score+=x[i][j];
                    for(k=j-1;k>0;k--)
                        x[i][k]=x[i][k-1];
                    x[i][0]=0;
                }
            }
        }
    }
/////////////////////////////
    if(way==2)//down
    {
        for(i=0;i<4;i++)
        for(m=0;m<3;m++)
            for(j=3;j>0;j--)
            {
                if(x[j][i]==0)
                {
                    for(k=j;k>0;k--)
                        x[k][i]=x[k-1][i];
                    x[0][i]=0;
                }
            }
        for(i=0;i<4;i++)
        {
            for(j=3;j>0;j--)
            {
                if(x[j][i]==x[j-1][i])
                {
                    x[j][i]=x[j][i]+x[j-1][i];
                    score+=x[j][i];
                    for(k=j-1;k>0;k--)
                        x[k][i]=x[k-1][i];
                    x[0][i]=0;
                }
            }
        }
    }
////////////////////////
    if(way==3)//left
    {
        for(i=0;i<4;i++)
        for(m=0;m<3;m++)
            for(j=0;j<3;j++)
            {
                if(x[i][j]==0)
                {
                    for(k=j;k<3;k++)
                        x[i][k]=x[i][k+1];
                    x[i][3]=0;
                }
            }
        for(i=0;i<4;i++)
        {
            for(j=0;j<3;j++)
            {
                if(x[i][j]==x[i][j+1])
                {
                    x[i][j]=x[i][j]+x[i][j+1];
                    score+=x[i][j];
                    for(k=j+1;k<3;k++)
                        x[i][k]=x[i][k+1];
                    x[i][3]=0;
                }
            }
        }
    }
////////////////////////
    if(way==4)//up
    {
        for(i=0;i<4;i++)
        for(m=0;m<3;m++)
            for(j=0;j<3;j++)
            {
                if(x[j][i]==0)
                {
                    for(k=j;k<3;k++)
                        x[k][i]=x[k+1][i];
                    x[3][i]=0;
                }
            }
        for(i=0;i<4;i++)
        {
            for(j=0;j<3;j++)
            {
                if(x[j][i]==x[j+1][i])
                {
                    x[j][i]=x[j][i]+x[j+1][i];
                    score+=x[j][i];
                    for(k=j+1;k<3;k++)
                        x[k][i]=x[k+1][i];
                    x[3][i]=0;
                }
            }
        }
    }
	FRAM_WriteMemory(0,20,score>>8);
	FRAM_WriteMemory(0,21,score&0xFF);
	if(compare()==1) 
	{
		start();
		unsigned char n=0;
		for(i=0;i<4;i++)
        {
            for(j=0;j<4;j++)
            {
				FRAM_WriteMemory(0,n,x[i][j]>>8); //high 8 bits
				FRAM_WriteMemory(1,n,x[i][j]&0xFF); //low 8 bits
				n++;
			}
		}
	}
}
