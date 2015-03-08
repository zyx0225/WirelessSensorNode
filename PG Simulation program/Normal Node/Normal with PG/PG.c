/*
 * CFile1.c
 *
 * Created: 2013/4/11 13:13:23
 *  Author: Administrator
 */ 
for (i=0;i<2;i++)
{
	for (j=0;j<3;j++)
	{
		for (k=2;k<3;k++)
		{
			lambda[k-2]=(sqrt((buffer[k][1]-'0'-i)*(buffer[k][1]-'0'-i)+(buffer[k][2]-'0'-j)*(buffer[k][2]-'0'-j)))/CF; //CF from char to double
			P_lambda[k-2]=(pow(lambda,(buffer[k][3]-'0'-1)))*(pow(2.71828,0-lambda));
		}
		PG[i][j]=P_lambda[0]*P_lambda[1];
	}
}

max=PG[0][0];
maxposition[2]={0,0};
for (i=0;i<2;i++)
{
	for (j=0;j<3;j++)
	{
		if(PG[i][j]>max)
		{
			max=PG[i][j];
			maxposition[0]=i;
			maxposition[1]=j;
		}
	}
}