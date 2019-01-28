#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>

#define MAX_SIZE 2048
#define MY_ERROR(s) printf(s); system("PAUSE"); exit(1);
#define SERVER_PORT 9998
using namespace std;
struct bank_sct{
	char name[21]={};
	char password[21]={};
	int money;
};

int main(int argc, char **argv) {
	
	char pre_port[5]="9998";
	if(argc!=2)
	{
		cout<<"輸入錯誤參數 將依照預設執行 或依照studentID_ser port格式重新執行。"<<endl;
		//return 0;
	}
	else{strcpy(pre_port,argv[1]);}
	
	SOCKET serverSocket, clientSocket; // create a socket
	struct sockaddr_in serverAddress, clientAddress; // sockaddr_in
	int clientAddressLen;
	int bytesRead;
	char buf[MAX_SIZE]={};

	// call WSAStartup first for Winsock
    WSADATA wsadata;
    if( WSAStartup(MAKEWORD(2,2),(LPWSADATA)&wsadata) != 0) { // ( version of winsock )
        MY_ERROR("Winsock Error\n");
	}

	serverSocket = socket(PF_INET, SOCK_STREAM, 0); // address , type , protocal  

	memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons((uint16_t)atoi(pre_port));//converts a u_short from host to TCP/IP network byte order

    if( bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        MY_ERROR("Bind Error\n");
	}

	if( listen(serverSocket, 3) < 0) {
		MY_ERROR("Listen Error\n");
	}

	//讀檔 
	char a;
	bank_sct bank[10000];
	int acct_num;
	fstream bank_data;
	bank_data.open("bank_data.txt",fstream::in);
	if(!bank_data)
	{
		bank_data.open("bank_data.txt",fstream::out);
		bank_data<<0<<endl;
		bank_data.close();
		bank_data.open("bank_data.txt",fstream::in);
	}
	bank_data>>acct_num;
	string aa;
	getline(bank_data,aa);
	for(int i=0;i<acct_num;i++)
	{
		string tmp;
		getline(bank_data,tmp);
		cout<<tmp<<endl;
		//if(tmp[0]==' '){i--;continue;}
		
		int j;
		for(j=0;j<22;j++)
		{
			if(tmp[j]==' '){break;}
			bank[i].name[j]=tmp[j];
		}
		for(j=22;j<44;j++)
		{
			if(tmp[j]==' '){break;}
			bank[i].password[j-22]=tmp[j];
		}
		for(j=44;j<59;j++)
		{
			if(tmp[j]>57||tmp[j]<48){break;}
			bank[i].money=bank[i].money*10;
			bank[i].money+=tmp[j]-48;
		}
	}
	bank_data.close();
	for(int i=0;i<acct_num;i++)
	{
		cout<<bank[i].name<<"||"<<bank[i].password<<"||"<<bank[i].money<<endl;
	} 
	//讀檔結束 
	
	//執行過程 
	while(1) {
		printf("Waiting...\n");
		clientAddressLen = sizeof(clientAddress);
		clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
		
		char acname[21]={};
		bytesRead = recv(clientSocket, acname, 21, 0);
		cout<<"客戶端IP";
		printf("Client IP is : %s \n", inet_ntoa(clientAddress.sin_addr));cout<<endl;
		if(acname[0]=='$'&&acname[1]=='Q'){break;}
		if(!((acname[0]>64&&acname[0]<91)||(acname[0]>47&&acname[0]<58)||(acname[0]>96&&acname[0]<123))){continue;}
		int check_new=0,userid;
		//for(int j=0;j<21;j++){cout<<acname[j];}
		for(int i=0;i<acct_num;i++)
		{
			int memck=0;
			for(int j=0;j<21;j++)
			{
				//cout<<bank[i].name[j]<<acname[j]<<endl;
				if(bank[i].name[j]==' '){break;}
				if(bank[i].name[j]!=acname[j]){memck=0;break;}
				memck=1;
			}
			if(memck)
			{
				userid=i;
				check_new=1;
				break;
			}
		}
		//cout<<userid<<"@@@"<<check_new<<"@@@";
		if(check_new==0)
		{
			cout<<"創立新帳號"<<endl; 
			char newacct_pass[21]={};
			string str="Crt";
			send(clientSocket,str.c_str(),str.size(),0);
			bytesRead = recv(clientSocket, newacct_pass, 21, 0);
			
			for(int i=0;i<21;i++)
			{
				bank[acct_num].name[i]=acname[i];
			}
			cout<<"    ";
			for(int i=0;i<21;i++)
			{
				cout<<newacct_pass[i];
				bank[acct_num].password[i]=newacct_pass[i];
			}
			cout<<endl;
			bank[acct_num].money=0; 
			cout<<"新帳號:"<<bank[acct_num].name<<"|"<<bank[acct_num].password<<endl; 
			acct_num++;
			
			
			str="CSU";
			send(clientSocket,str.c_str(),str.size(),0);
		}
		else
		{
			cout<<"第"<<userid<<"個帳號"<<endl; 
			char oldpass[21]={};
			string str="LoG";
			send(clientSocket,str.c_str(),str.size(),0);
			bytesRead = recv(clientSocket, oldpass, 21, 0);
			int logck=0;
			cout<<endl<<bank[userid].password<<endl;
			for(int i=0;i<21;i++)
			{
				//cout<<bank[userid].password[i]<<oldpass[i]<<endl;
				if(bank[userid].password[i]==' '){break;}
				if(bank[userid].password[i]!=oldpass[i])
				{
					logck=0;
					break;
				}
				logck=1;
			}
			if(logck)
			{
				cout<<"登入完成"<<endl; 
				str="LsU";
				send(clientSocket,str.c_str(),str.size(),0);
				char opnum[21]={};
				bytesRead = recv(clientSocket, opnum, 21, 0);
				cout<<opnum<<endl;
				if(opnum[0]=='#'){continue;}
				int num=0,opcheck=1;
				for(int i=1;i<8;i++)
				{
					if(opnum[i]<48||opnum[i]>57){break;}
					num=num*10;
					num+=opnum[i]-48;
				}
				if(opnum[0]=='+')
				{
					cout<<"帳號預期剩餘:"<<bank[userid].money<<"+"<<num<<endl; 
					if(bank[userid].money+num>213000000){opcheck=0;}
					else
					{
						bank[userid].money+=num;
					}
				}
				if(opnum[0]=='-')
				{
					cout<<"帳號預期剩餘:"<<bank[userid].money<<"-"<<num<<endl; 
					if(bank[userid].money-num<0){opcheck=0;}
					else
					{
						bank[userid].money-=num;
					}
				}
				if(opcheck==1)
				{
					str="OpS";
					send(clientSocket,str.c_str(),str.size(),0);
					char need[60]={};
					bytesRead = recv(clientSocket, need, 60, 0);
					if(need[0]='Y')
					{
						for(int i=0;i<21;i++)
						{
							need[i]=bank[userid].name[i];
						}
						string opn=to_string(bank[userid].money);
						cout<<"$"<<opn<<endl;
						for(int i=44;i<44+opn.length();i++)
						{
							need[i]=opn[i-44];
						}
					}
					cout<<need<<endl;
					send(clientSocket,need, 60,0);
				}
				if(opcheck==0)
				{
					str="OpF";
					send(clientSocket,str.c_str(),str.size(),0);
				}
				continue;
			}
			else
			{
				cout<<"登入失敗"<<endl; 
				str="LFa";
				send(clientSocket,str.c_str(),str.size(),0);
				continue;
			}
		}
		
		
		bank_data.open("bank_data.txt",fstream::out|fstream::trunc);
		bank_data<<acct_num<<endl;
		for(int i=0;i<acct_num;i++)
		{
			for(int j=0;j<21;j++)
			{
				bank_data<<bank[i].name[j];
			}
			bank_data<<',';
			for(int j=0;j<21;j++)
			{
				bank_data<<bank[i].password[j];
			}
			bank_data<<','<<bank[i].money<<endl;;
		}
		bank_data.close();
		
		
		buf[bytesRead] = '\0';
		send(clientSocket, buf, bytesRead, 0);
		printf("Client IP is : %s \n", inet_ntoa(clientAddress.sin_addr));
		printf("Receive %d byte(s): %s\n", bytesRead, buf);
		closesocket(clientSocket);
	}
	
	bank_data.open("bank_data.txt",fstream::out|fstream::trunc);
	bank_data<<acct_num<<endl;
	for(int i=0;i<acct_num;i++)
	{
		for(int j=0;j<21;j++)
		{
			bank_data<<bank[i].name[j];
		}
		bank_data<<',';
		for(int j=0;j<21;j++)
		{
			bank_data<<bank[i].password[j];
		}
		bank_data<<','<<bank[i].money<<endl;;
	}
	bank_data.close();

	return 0;
}
