/*
* COP 3515 – Fall Semester 2024
*
* Homework #2: Spotting The Hacker
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

FILE *Parityf;
FILE *Checkf;
FILE *Parity2f;

int num;
char binary[9];
char pbinary[9];

char check_binary[20];
int sum;
int check_count;

int block_num = 1;
int bline;
char* Get_binary(int n)
{
    if (n <= 255)
    {
        int bit = 128;
        for (int i=7;i>-1;i--)
        {   
        
            if (n >= bit)
            {
                n -= bit;
                binary[7-i] = '1';
            }

            else
                binary[7-i]= '0';
            bit /= 2;
        }
        return binary;
    }
    else 
    {
        int bit = 512;
        for (int i=9;i>-1;i--)
        {   
        
            if (n >= bit)
            {
                n -= bit;
                check_binary[9-i] = '1';
            }

            else
                check_binary[9-i]= '0';
            bit /= 2;
        }
        return check_binary;
    }
}

void Line_parity(int *line)
{
    printf("\nParity byte: %d\n",line[8]);
    Get_binary(line[8]);
    char *bin = &binary[0];
    for (int i=0;i<8;i++)
    {
        pbinary[i] = *bin;
        bin++;
    }
    
    for (int i = 0;i<8;i++)
    {
        Get_binary(line[i]);
        printf("\ndata item = %d, binary = %s\n", line[i], binary);
        int bcount = 0;
        for (int j = 0;j<8;j++)
        {
            if (binary[j] == '1')
            {
                bcount++;
            }
            else
                continue;
        }
        if (pbinary[i] == '0' && bcount % 2 == 0)
        {
            printf("No error in transmission byte %d\n", i+1);
        }
        else if (pbinary[i] == '1' && bcount % 2 != 0)
        {
            printf("No error in transmission byte %d\n", i+1);
        }
        else
            printf("Error in transmission byte %d\n", i+1);
    }
    
}

void Parity_Check(FILE *Dataf)
{
    printf("\n** Part 1 - Parity Check Processing\n");
    
    int count = 9;
    int spot = 0;
    int line[9];
    while(!(feof(Dataf)))
    {
        
        fscanf(Dataf, "%d",&num);

        line[(spot)] = num;
        spot++;
        if (spot == 9)
        {
            spot = 0;
        }

        if(count % 9 == 0)
        {
            printf("\n\nTransmission line number: %d\n\n", (count/9));
            printf("Data stream:\n");
        }
        count++;
        if (count%9 != 0)
        {
            printf("%d ",num);

        }
        if ((count)%9 == 0)
        {
            Line_parity(line);
        }
        
    }
}

int Complement(int num)
{
    if (num<=256)
    {
        return 255 - num;
    }
    else if (num <=512)
    {
        return 511 - num;
    }
    else if (num <=1024)
    {
        return 1023 - num;
    }
    else
        return 2047 - num;
}

void Check_line(int *line)
{
    printf("\nChecksum: %d\n",line[8]);

    int sum = 0;
    for (int i = 0;i<8;i++)
    {
        sum += line[i];
        printf("\nparityData[i] = %d, binary = %s\n", line[i], Get_binary(line[i]));
    }

    printf("\nSum of data items = %d, binary = %s\n", sum, Get_binary(sum));
    printf("\nChecksum value = %d, binary = %s\n", line[8], Get_binary(line[8]));

    printf("\nSum after adding checksum = %d, binary = %s",sum+line[8], Get_binary(sum+line[8]));
    printf("\nSum after complement = %d, binary = %s\n",Complement(sum + line[8]), Get_binary(Complement(sum + line[8])));

    if (Complement(sum + line[8]) == 0)
    {
        check_count++;
    }
}
void Checksum(FILE *Dataf)
{
    printf("\n** Part 2 - Checksum Processing\n");
    
    int count = 9;
    
    int spot = 0;
    int line[9];
    while(!(feof(Dataf)))
    {  
        fscanf(Dataf, "%d",&num);

        line[(spot)] = num;
        spot++;
        if (spot == 9)
        {
            spot = 0;
        }

        if(count % 9 == 0)
        {
            printf("\nData stream:\n");
        }
        count++;
        if (count%9 != 0)
        {
            printf("%d ",num);

        }
        if ((count)%9 == 0)
        {
            Check_line(line);
        }
    }
    if (check_count == 5)
    {
        printf("\nChecksum: No errors in transmission\n");
    }
    else
        printf("\nChecksum: Error in transmission\n");
}

void Vert_parity(int dblock[41][11])
{
    for (int j=0;j<8;j++)
    {
        if (j+1 == bline)
        {
            int vertbcount = 0;
            printf("\n** Vertical processing transmission line %d\n", j);
            Get_binary(dblock[((block_num-1)*8)+bline-1][8]);
            char vertbinary[9];
            for (int k = 0; k<8;k++)
            {
                vertbinary[k] = binary[k];
            }
            for (int i=((block_num-1)*8);i<8*block_num;i++)
            {
                printf("val%d = %d, %s \n",i-((block_num-1)*8)+1,dblock[i][j],Get_binary(dblock[i][j]));
                int vbcount = 0;
                char vertb = '0';
                for (int b = 0;b<8;b++)
                {
                    if (binary[b] == '1')
                    {
                        vbcount++;
                    }
                    
                }
                if (vbcount%2)
                {
                    vertb = '1';
                    vertbcount++;
                }
                  
            }
            
            printf("vertParityByte[%d] = %d \n",j,dblock[((block_num-1)*8)+bline-1][8]);
            if (vertbcount%2 == 1)
            {
                printf("\nTransmission line %d passed vertical parity\n",j);
            } 
            else
                printf("\nTransmission line %d failed vertical parity\n", j);
        }
        
    }
}

void Hor_parity(int dblock[41][11])
{ 
    for (int i=((block_num-1)*8);i<8*block_num;i++)
    {
        if (i-((block_num-1)*8)+1 == bline)
        {
            int horbcount = 0;
            Get_binary(dblock[((block_num-1)*8)+bline-1][9]);
            char horbinary[9];
            for (int k = 0; k<8;k++)
            {
                horbinary[k] = binary[k];
            }
            printf("\n** Horizontal Processing transmission line %d\n", i-((block_num-1)*8));
            for (int j=0;j<8;j++)
            {
                printf("val%d = %d, %s \n",j,dblock[i][j],Get_binary(dblock[i][j]));
                int hbcount = 0;
                char horb = '0';
                for (int b = 0;b<8;b++)
                {
                    if (binary[b] == '1')
                    {
                        hbcount++;
                    }
                }
                if (hbcount%2)
                {
                    horb = '1';
                    horbcount++;
                }
            }
            printf("horizParityByte[%d] = %d \n",i%8,dblock[((block_num-1)*8)+bline-1][9]);
            if (horbcount%2 == 1)
            {
                printf("\nTransmission line %d passed vertical parity\n",i%8);
            } 
            else
                printf("\nTransmission line %d failed vertical parity\n", i%8);
        }
    }
}
void Parity_2D(FILE *Dataf)
{
    int count = 10;
    int spotv = -1;
    int spoth;
    int block[41][11];
    
    while(!(feof(Dataf)))
    {  
        fscanf(Dataf, "%d",&num);
            
        if(count % 10 == 0)
        {
            spoth = 0;
            spotv++;
            block[spotv][spoth] = num;

        }
        else
            spoth++;
            block[spotv][spoth] = num;
        count ++;
    }

    while (block_num < 6)
    {
        printf("\n==> Processing Transmission Block %d\n\n", block_num);
        printf("Data streams:\n");

        for (int i=0+((block_num-1)*8);i<8*block_num;i++)
        {
            for (int j=0;j<10;j++)
            {
                if (j<8)
                {
                    printf("%d ",block[i][j]);
                }
                else if(j==8) 
                {
                    printf("[V: %d] ",block[i][j]);
                }
                else
                    printf("[H: %d] ",block[i][j]);
                
            }
            printf("\n");
        }
        bline=1;
        while (bline<9)
        {
            Vert_parity(block);
            Hor_parity(block);
            bline++;
        }
        block_num++;
    }     
}

int main()
{
    Parityf = fopen("./White House Transmission Data - Parity.txt","r");
    Checkf = fopen("./White House Transmission Data - Checksum.txt","r");
    Parity2f = fopen("./White House Transmission Data - 2D Parity.txt","r");
    
    Parity_Check(Parityf);
    Checksum(Checkf);
    Parity_2D(Parity2f);
        
    fclose(Parityf);
    fclose(Checkf);
    fclose(Parity2f);
}
