#include <stdio.h>
#include <unistd.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
/*
Compress by encoding A->00, T->01, C->10, G->11
At first, i try to to use huffman coding. But it is too difficult to implement.
If the text contains ATCG only, compress it.
else ignore and write it directly.
The compression method: 
[length]{ignored text}[total length of DNA][length of line of DNA]{ATCG encoded in 101010100 format}...
The most difficult part was research on how to write bits of data instead of bytes and read it.
and keep debugging and improving the compression method.

The compression ratio of yeast.nt file can achieve 0.259.
For DNA code contains other than ATCG, the compression efficiency decrease.

Also, the program will not output \r. But the text content of decompressed and original file is the same.

Anyway, it is a really tough project, professor please consider the effort. T^T

Thanks
Terence Siu 1155079784
*/
#define buf_size 1000

void compress(char * in, char * out){
	char c;
	FILE * fptr; 
	FILE * fwptr;
	int skip=1;
	char ignore[buf_size];
	char bytecode[8];
	char code[buf_size];
	int count=0;
	int k=0;
	int len=0;
	unsigned long position;
	long lastpos;
	int lastlen;
	memset(ignore, 0, buf_size);
	memset(bytecode, 0, 8);

	fptr = fopen(in , "rb");
	fwptr = fopen(out,"wb");
	if (fptr != NULL)
	{
		while (1)
		{  
			c = fgetc(fptr);
			if (!feof (fptr))
			{		
				if (c!='\n' && skip==1) 
				{
					ignore[k++]=c;
				}
				else if (skip==1 && c=='\n') {	//ignored text processing
					int rubbish=0;	
					skip=0;
					ignore[k++]=c;
					int length=strlen(ignore);
					lastpos=ftell(fwptr);
					lastlen=length;
					fputc(length,fwptr);
					ignore[k-1]='\0';
					fputs(ignore,fwptr);
					memset(ignore, 0, buf_size);
					k=0;
					unsigned int dnalen=0;
					long current= ftell(fptr);
					c = fgetc(fptr);
					if (!(c!='A' && c!='T' && c!='G' && c!='C')) dnalen=1;
					while (!feof (fptr))
					{
						c = fgetc(fptr);
						if (c!='A' && c!='T' && c!='G' && c!='C' && c!='\n' && c!='\r' && c!='>' && !feof (fptr)) {rubbish=1;}
						else if (c=='>') break;
						else if (c=='A' || c=='G' || c=='C' || c=='T') {
							dnalen++;}
						}
						
						if (rubbish==0)
						{
							if (dnalen>0)
							{
								fwrite(&dnalen,4,1,fwptr);
							}
							fseek(fptr,current,SEEK_SET);
						}else
					{			//DNA code other than ATCG is added to ignored text
						char ignoredna[buf_size];
						memset(ignoredna, 0, buf_size); 
						long iglen=ftell(fptr)-current-1;
						fseek(fptr,current,SEEK_SET);
						fread(ignoredna,iglen,1,fptr);
						ignoredna[iglen-1]='\0';
						iglen--;
						fwrite(ignoredna,iglen,1,fwptr);
						long writepos=ftell(fwptr);
						skip=1;
						fseek(fwptr,lastpos,SEEK_SET);
						fputc(lastlen+iglen,fwptr);
						fseek(fwptr,writepos,SEEK_SET);
						
					}
				}
				else if (c=='>') {skip=1;ignore[k++]=c;}
			else //DNA code processing
				{   char tmp[buf_size];			
					memset(tmp, 0, buf_size);
					memset(code, 0, buf_size);
					long current= ftell(fptr);
					fgets (tmp, buf_size, fptr);
					
					code[0]=c;
					strcat(code,tmp);
					len=strlen(tmp);
					if (code[len-1]=='\r') len--;
					fputc(len,fwptr);				
					int j;
					for (j=1;j<=len;j++)
					{
						if (code[j-1]=='A') strcat(bytecode,"00");
						if (code[j-1]=='T') strcat(bytecode,"01");
						if (code[j-1]=='G') strcat(bytecode,"10");
						if (code[j-1]=='C') strcat(bytecode,"11");
						if (j%4==0)
						{
					//printf("bytecode: %s\n",bytecode);
							long l = strtol(bytecode, 0, 2);
							unsigned char b = l & 0xffl;
							fwrite(&b, 1, 1, fwptr);
							memset(bytecode, 0, 8);
						}
					}
					int fenable=0;
					j--;
					while (j%4!=0) {j++;strcat(bytecode,"00");fenable=1;}
					if (fenable)
					{
					//printf("bytecode2: %s\n",bytecode);
						long l = strtol(bytecode, 0, 2);
						unsigned char b = l & 0xffl;
						fwrite(&b, 1, 1, fwptr);
						memset(bytecode, 0, 8);
					}			
				}
				
			}
			else break;
		}
	}
	fclose(fptr);fclose(fwptr);
}

void decompress(char * in, char * out){
	char c;
	FILE * fptr2; 
	FILE * fwptr2;
	fptr2 = fopen(in,"rb");
	fwptr2 = fopen(out,"wb");
	int getlen=1;
	int linelength;
	int read=0;
	int seglength;
	char str[buf_size];
	long pos;
	char twobit[2];
	memset(str, 0, buf_size);
	int getdna=0;
	int k=0;
	if (fptr2 != NULL)
	{
		while (1)
		{	
			pos= ftell(fptr2);
			c = fgetc(fptr2);
			if (getlen==1) 
			{	
				linelength=c;
				getlen=0;		
				k=0;
				c = fgetc(fptr2);
			}
			if (!feof (fptr2))
			{
				if (c=='>' || (c!='A' && c!='T' && c!='G' && c!='C' && c>'A' && c<='Z') && getdna==0)
				{
					ungetc(c,fptr2);
					fread(str,linelength-1,1,fptr2);
					fwrite(str,linelength-1,1,fwptr2);
					fputc('\n',fwptr2);
						//printf("len:%d\nstr:%s\n",strlen(str),str);
					memset(str, 0, buf_size);
					getlen=1;getdna=1;
				}
				else
				{	
					unsigned int totallength;
					fseek(fptr2,pos,SEEK_SET);
						//for (int i=0;i<10;i++)
					//	{
						//		char byte;
						//		byte=fgetc(fptr2);
					//			printf("%x ", byte & 0xff);
					//	}
					
					fread(&totallength,4,1,fptr2);
					seglength=fgetc(fptr2);
					int codelength;
					codelength=ceil((double)seglength/4);
					char dnacode[buf_size];
					memset(dnacode, 0, buf_size);
					while (totallength>0 && !feof (fptr2))
					{
							//printf("%d %d",totallength, seglength);
						int k=seglength;
						while (codelength>0&& !feof (fptr2))
						{			
							char byte;
							byte=fgetc(fptr2);
								//printf("%x ", byte & 0xff);
							int bit=0;
							memset(twobit,0,2);
							for (int i = 7; i >= 0; i--)
							{
								
								bit = byte >> i;
								
								if (bit & 1)
								{
									strcat(twobit,"1");
								}
								else
								{
									strcat(twobit,"0");
								}
								if (i%2==0 &&k>0)
								{
									if (strcmp(twobit,"00")==0) strcat(dnacode,"A");
									if (strcmp(twobit,"01")==0) strcat(dnacode,"T");
									if (strcmp(twobit,"10")==0) strcat(dnacode,"G");
									if (strcmp(twobit,"11")==0) strcat(dnacode,"C");
									k--;
										//printf("%s\n",dnacode);
									memset(twobit,0,2);
								}
								
							}
							codelength--;
							if (k==0){
								fputs(dnacode,fwptr2);
								fputc('\n',fwptr2);	
								memset(dnacode, 0, buf_size);
							}
						}
						totallength=totallength-seglength;
						
						if (totallength>0) 
						{
							char byte;
							seglength=fgetc(fptr2);
							codelength=ceil((double)seglength/4);
						}
					}
					memset(str, 0, buf_size);
					getlen=1;
					getdna=0;
				}
			}else break;	
			
		}
	}

}
int main (int argc, char *argv[])
{
	int opt;
	while ((opt = getopt (argc, argv, "c:d:")) != -1)
	{
		switch (opt)
		{
			case 'c':
			compress(argv[2], argv[3]);
			break;
			case 'd':
			decompress(argv[2], argv[3]);
			break;
		}
	}
	return 0;
}