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
		cout<<"輸入錯誤參數 將依照預設執行 或依照studentID_cli ip port格式重新執行。"<<endl;
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
			cout<<"請輸入帳號。請注意帳號內文字僅能包含英文字母大小寫及數字，且長度不可超過20位。或者輸入$Q來離開系統。"<<endl;
			cin>>in;
			if(in[0]=='$'&&in[1]=='Q'){swh_Q=1;break;}
			if(in.length()>20||in.length()<1){cout<<"帳號長度超過規定，請重新輸入。"<<endl;continue;}
			for(int i=0;i<in.length();i++)
			{
				if((in[i]>64&&in[i]<91)||(in[i]>47&&in[i]<58)||(in[i]>96&&in[i]<123)){newacct[i]=in[i];}
				else{check=1;break;}
			}
			if(check==1){cout<<"帳號包含非法字元，請重新輸入。"<<endl;check=0;continue;}
			
			ctr=1;
		}
		ctr=0;
		string str="$Q";
		if(swh_Q==1){send(serverSocket,str.c_str(),str.size(),0);cout<<"結束"<<endl;closesocket(serverSocket);break;}
		send(serverSocket, newacct, 21, 0);
		bytesRead = recv(serverSocket, buf, MAX_SIZE, 0);
		
		if(buf[0]=='C'&&buf[1]=='r'&&buf[2]=='t')
		{
			char newpass[21]={};
			while(ctr<1)
			{
				cout<<"本帳號不存在，進行創立新帳號。"<<endl;
				cout<<"請設定新帳號密碼。請注意密碼內文字僅能包含英文字母大小寫及數字，且長度不可超過20位。"<<endl;
				cin>>in;
				if(in.length()>20||in.length()<1){cout<<"密碼長度超過規定，請重新輸入。"<<endl;continue;}
				for(int i=0;i<in.length();i++)
				{
					if((in[i]>64&&in[i]<91)||(in[i]>47&&in[i]<58)||(in[i]>96&&in[i]<123)){newpass[i]=in[i];}
					else{check=1;break;}
				}
				if(check==1){cout<<"密碼包含非法字元，請重新輸入。"<<endl;check=0;continue;}
				ctr=1;
			}
			ctr=0;
			send(serverSocket, newpass, strlen(newpass), 0);
			bytesRead = recv(serverSocket, buf, MAX_SIZE, 0);
			if(buf[0]=='C'&&buf[1]=='S'&&buf[2]=='U')
			{
				cout<<"帳號創立完畢，請重新登入。"<<endl;
				continue; 
			} 
		}
		if(buf[0]=='L'&&buf[1]=='o'&&buf[2]=='G')
		{
			char oldpass[21]={};
			cout<<"本帳號已經存在，請輸入密碼以登入。"<<endl;
			while(ctr<1)
			{
				cout<<"請輸入密碼。請注意密碼內文字僅能包含英文字母大小寫及數字，且長度不可超過20位。"<<endl;
				cin>>in;
				if(in.length()>20||in.length()<1){cout<<"密碼長度超過規定，請重新輸入。"<<endl;continue;}
				for(int i=0;i<in.length();i++)
				{
					if((in[i]>64&&in[i]<91)||(in[i]>47&&in[i]<58)||(in[i]>96&&in[i]<123)){oldpass[i]=in[i];}
					else{check=1;break;}
				}
				if(check==1){cout<<"密碼包含非法字元，請重新輸入。"<<endl;check=0;continue;}
				ctr=1;
			}
			ctr=0;
			send(serverSocket, oldpass, strlen(oldpass), 0);
			bytesRead = recv(serverSocket, buf, MAX_SIZE, 0);
			if(buf[0]=='L'&&buf[1]=='s'&&buf[2]=='U')
			{
				cout<<"登入完成。"<<endl;
				cout<<"請輸入D進行存款，或輸入W進行提款。提醒您，單次存取款上限最高100萬元。"<<endl;
				cin>>in;
				int opcheck=1; 
				char opnum[21]={};
				if(in[0]=='D'&&in.length()==1)
				{
					cout<<"請輸入存款金額："<<endl; 
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
							cout<<"操作完成。"<<endl;
							char need[60]={};need[0]='Y';
							send(serverSocket, need, 60, 0);
							bytesRead = recv(serverSocket, need, 60, 0);
							cout<<"明細    ||帳戶名稱：";
							for(int i=0;i<21;i++)
							{
								if(need[i]==' '){break;}
								cout<<need[i];
							}
							cout<<"    ||帳戶餘額：";
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
						cout<<"操作失敗。"<<endl; 
					}
				}
				else if(in[0]=='W'&&in.length()==1)
				{
					cout<<"請輸入提款金額："<<endl; 
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
							cout<<"操作完成。"<<endl;
							char need[60]={};need[0]='Y';
							send(serverSocket, need, 60, 0);
							bytesRead = recv(serverSocket, need, 60, 0);
							cout<<"明細||帳戶名稱：";
							for(int i=0;i<21;i++)
							{
								if(need[i]==' '){break;}
								cout<<need[i];
							}
							cout<<"||帳戶餘額：";
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
						cout<<"操作失敗。"<<endl; 
					}
				}
				else
				{
					char fa[21]={};fa[0]='#';
					send(serverSocket, fa, 21, 0);
					cout<<"操作失敗。"<<endl; 
				}
				 
				continue; 
			}
			if(buf[0]=='L'&&buf[1]=='F'&&buf[2]=='a')
			{
				cout<<"登入失敗。"<<endl;
				continue; 
			}
		}
		
		
		if(swh_CL==1)
		{
			char newacct[21]={},check=0;
			while(ctr<1)
			{
				
				cin>>in;
				if(in.length()>20){cout<<"帳號長度超過規定，請重新輸入。"<<endl;continue;}
				for(int i=0;i<in.length();i++)
				{
					if((in[i]>64&&in[i]<91)||(in[i]>47&&in[i]<58)||(in[i]>96&&in[i]<123)){newacct[i]=in[i];}
					else{check=1;break;}
				}
				if(check==1){cout<<"帳號包含非法字元，請重新輸入。"<<endl;check=0;continue;}
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
