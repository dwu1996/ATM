#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#include <cstdio>
#include <string>

#define MAX_SIZE 2048
#define MY_ERROR(s) printf(s); system("PAUSE"); exit(1);
#define SERVER_PORT 9998
using namespace std;

int main(int argc, char **argv) {
	
	char IP[16]="140.114.207.107",pre_port[5]="9998";
	if(argc!=3)
	{
		cout<<"��J���~�Ѽ� �N�̷ӹw�]���� �Ψ̷�studentID_cli ip port�榡���s����C"<<endl;
		//return 0;
	}
	else{strcpy(IP,argv[1]);strcpy(pre_port,argv[2]);}
	
	SOCKET serverSocket;
	struct sockaddr_in serverAddress;
	int bytesRead;
	char buf[MAX_SIZE],pre[MAX_SIZE];

	// call WSAStartup first for Winsock
    WSADATA wsadata;
    if( WSAStartup(MAKEWORD(2,2),(LPWSADATA)&wsadata) != 0) {
        MY_ERROR("Winsock Error\n");
	}

	memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(IP); // transform to 32-bit unsigned integer
    serverAddress.sin_port = htons((uint16_t)atoi(pre_port)); //converts a u_short from host to TCP/IP network byte order

	while(1) {
		
		serverSocket = socket(PF_INET, SOCK_STREAM, 0);
		if(connect(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress))<0){
            MY_ERROR("Connect Error\n");
		}
		
		int ctr=0,swh_CL=0,swh_Q=0;
		string in;
		char newacct[21]={},check=0;
		
		while(ctr<1)
		{
			cout<<"�п�J�b���C�Ъ`�N�b������r�ȯ�]�t�^��r���j�p�g�μƦr�A�B���פ��i�W�L20��C�Ϊ̿�J$Q�����}�t�ΡC"<<endl;
			cin>>in;
			if(in[0]=='$'&&in[1]=='Q'){swh_Q=1;break;}
			if(in.length()>20||in.length()<1){cout<<"�b�����׶W�L�W�w�A�Э��s��J�C"<<endl;continue;}
			for(int i=0;i<in.length();i++)
			{
				if((in[i]>64&&in[i]<91)||(in[i]>47&&in[i]<58)||(in[i]>96&&in[i]<123)){newacct[i]=in[i];}
				else{check=1;break;}
			}
			if(check==1){cout<<"�b���]�t�D�k�r���A�Э��s��J�C"<<endl;check=0;continue;}
			
			ctr=1;
		}
		ctr=0;
		string str="$Q";
		if(swh_Q==1){send(serverSocket,str.c_str(),str.size(),0);cout<<"����"<<endl;closesocket(serverSocket);break;}
		send(serverSocket, newacct, 21, 0);
		bytesRead = recv(serverSocket, buf, MAX_SIZE, 0);
		
		if(buf[0]=='C'&&buf[1]=='r'&&buf[2]=='t')
		{
			char newpass[21]={};
			while(ctr<1)
			{
				cout<<"���b�����s�b�A�i��Х߷s�b���C"<<endl;
				cout<<"�г]�w�s�b���K�X�C�Ъ`�N�K�X����r�ȯ�]�t�^��r���j�p�g�μƦr�A�B���פ��i�W�L20��C"<<endl;
				cin>>in;
				if(in.length()>20||in.length()<1){cout<<"�K�X���׶W�L�W�w�A�Э��s��J�C"<<endl;continue;}
				for(int i=0;i<in.length();i++)
				{
					if((in[i]>64&&in[i]<91)||(in[i]>47&&in[i]<58)||(in[i]>96&&in[i]<123)){newpass[i]=in[i];}
					else{check=1;break;}
				}
				if(check==1){cout<<"�K�X�]�t�D�k�r���A�Э��s��J�C"<<endl;check=0;continue;}
				ctr=1;
			}
			ctr=0;
			send(serverSocket, newpass, strlen(newpass), 0);
			bytesRead = recv(serverSocket, buf, MAX_SIZE, 0);
			if(buf[0]=='C'&&buf[1]=='S'&&buf[2]=='U')
			{
				cout<<"�b���Хߧ����A�Э��s�n�J�C"<<endl;
				continue; 
			} 
		}
		if(buf[0]=='L'&&buf[1]=='o'&&buf[2]=='G')
		{
			char oldpass[21]={};
			cout<<"���b���w�g�s�b�A�п�J�K�X�H�n�J�C"<<endl;
			while(ctr<1)
			{
				cout<<"�п�J�K�X�C�Ъ`�N�K�X����r�ȯ�]�t�^��r���j�p�g�μƦr�A�B���פ��i�W�L20��C"<<endl;
				cin>>in;
				if(in.length()>20||in.length()<1){cout<<"�K�X���׶W�L�W�w�A�Э��s��J�C"<<endl;continue;}
				for(int i=0;i<in.length();i++)
				{
					if((in[i]>64&&in[i]<91)||(in[i]>47&&in[i]<58)||(in[i]>96&&in[i]<123)){oldpass[i]=in[i];}
					else{check=1;break;}
				}
				if(check==1){cout<<"�K�X�]�t�D�k�r���A�Э��s��J�C"<<endl;check=0;continue;}
				ctr=1;
			}
			ctr=0;
			send(serverSocket, oldpass, strlen(oldpass), 0);
			bytesRead = recv(serverSocket, buf, MAX_SIZE, 0);
			if(buf[0]=='L'&&buf[1]=='s'&&buf[2]=='U')
			{
				cout<<"�n�J�����C"<<endl;
				cout<<"�п�JD�i��s�ڡA�ο�JW�i�洣�ڡC�����z�A�榸�s���ڤW���̰�100�U���C"<<endl;
				cin>>in;
				int opcheck=1; 
				char opnum[21]={};
				if(in[0]=='D'&&in.length()==1)
				{
					cout<<"�п�J�s�ڪ��B�G"<<endl; 
					cin>>in;
					opnum[0]='+';
					for(int i=0;i<in.length();i++)
					{
						if(i>7){opcheck=0;break;}
						if(in[i]>57||in[i]<48){opcheck=0;break;}
						opnum[i+1]=in[i];
					}
					if(opcheck==1)
					{
						send(serverSocket, opnum, 21, 0);
						bytesRead = recv(serverSocket, buf, MAX_SIZE, 0);
						if(buf[0]=='O'&&buf[1]=='p'&&buf[2]=='S')
						{
							cout<<"�ާ@�����C"<<endl;
							char need[60]={};need[0]='Y';
							send(serverSocket, need, 60, 0);
							bytesRead = recv(serverSocket, need, 60, 0);
							cout<<"����    ||�b��W�١G";
							for(int i=0;i<21;i++)
							{
								if(need[i]==' '){break;}
								cout<<need[i];
							}
							cout<<"    ||�b��l�B�G";
							for(int i=44;i<59;i++)
							{
								if(need[i]==' '){break;}
								cout<<need[i];
							}
							cout<<"    ||"<<endl;
							
						}
						if(buf[0]=='O'&&buf[1]=='p'&&buf[2]=='F'){opcheck=0;}
					}
					if(opcheck==0)
					{
						cout<<"�ާ@���ѡC"<<endl; 
					}
				}
				else if(in[0]=='W'&&in.length()==1)
				{
					cout<<"�п�J���ڪ��B�G"<<endl; 
					cin>>in;
					opnum[0]='-';
					for(int i=0;i<in.length();i++)
					{
						if(i>7){opcheck=0;break;}
						if(in[i]>57||in[i]<48){opcheck=0;break;}
						opnum[i+1]=in[i];
					}
					if(opcheck==1)
					{
						send(serverSocket, opnum, 21, 0);
						bytesRead = recv(serverSocket, buf, MAX_SIZE, 0);
						if(buf[0]=='O'&&buf[1]=='p'&&buf[2]=='S')
						{
							cout<<"�ާ@�����C"<<endl;
							char need[60]={};need[0]='Y';
							send(serverSocket, need, 60, 0);
							bytesRead = recv(serverSocket, need, 60, 0);
							cout<<"����||�b��W�١G";
							for(int i=0;i<21;i++)
							{
								if(need[i]==' '){break;}
								cout<<need[i];
							}
							cout<<"||�b��l�B�G";
							for(int i=44;i<59;i++)
							{
								if(need[i]==' '){break;}
								cout<<need[i];
							}
							cout<<"||"<<endl;
						}
						if(buf[0]=='O'&&buf[1]=='p'&&buf[2]=='F'){opcheck=0;}
					}
					if(opcheck==0)
					{
						cout<<"�ާ@���ѡC"<<endl; 
					}
				}
				else
				{
					char fa[21]={};fa[0]='#';
					send(serverSocket, fa, 21, 0);
					cout<<"�ާ@���ѡC"<<endl; 
				}
				 
				continue; 
			}
			if(buf[0]=='L'&&buf[1]=='F'&&buf[2]=='a')
			{
				cout<<"�n�J���ѡC"<<endl;
				continue; 
			}
		}
		
		
		if(swh_CL==1)
		{
			char newacct[21]={},check=0;
			while(ctr<1)
			{
				
				cin>>in;
				if(in.length()>20){cout<<"�b�����׶W�L�W�w�A�Э��s��J�C"<<endl;continue;}
				for(int i=0;i<in.length();i++)
				{
					if((in[i]>64&&in[i]<91)||(in[i]>47&&in[i]<58)||(in[i]>96&&in[i]<123)){newacct[i]=in[i];}
					else{check=1;break;}
				}
				if(check==1){cout<<"�b���]�t�D�k�r���A�Э��s��J�C"<<endl;check=0;continue;}
				send(serverSocket, newacct, strlen(newacct), 0);
				ctr=1;
			}
			ctr=0;
			for(int i=0;i<strlen(newacct);i++)
			{cout<<newacct[i];
			}cout<<endl;
			
			bytesRead = recv(serverSocket, newacct, strlen(newacct), 0);
			newacct[bytesRead] = '\0';
			if(bytesRead>0) printf("Receive %d byte(s): %s\n", bytesRead, newacct);
			closesocket(serverSocket);
		}
		if(swh_CL==2)
		{
			
		}
		
		scanf("%s", buf);
		
		
		

		/*
		send(serverSocket, buf, strlen(buf), 0);
		bytesRead = recv(serverSocket, buf, MAX_SIZE, 0);
		buf[bytesRead] = '\0';
		if(bytesRead>0) printf("Receive %d byte(s): %s\n", bytesRead, buf);*/
		closesocket(serverSocket);
	}

	return 0;
}
