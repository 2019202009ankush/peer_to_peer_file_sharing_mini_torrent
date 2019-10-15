#include "myheader.h"
#define BUFF_SIZE 10240
#define BACK 2500
using namespace std;
string TR1ip;
string TR2ip;
string TR1port;
string TR2port;
vector<thread> threadVector;
int threadCount;
map<string,string>userReg;
map<string,set<string> >GroupInfo;
map<pair<string,string>,set<string> >seederlist;
map<pair<string,string>,int > FileSizemap;
map<string,string>FileMap;
map<string,set<string> >GroupAndFile;

string s="ankush";
string p="harry2016";


vector<string>ArrayOfString(string s,char del)
{
  stringstream ss(s);
  vector<string>a;
  string temp;
  while(getline(ss,temp,del))
  {
  	//cout<<"hi"<<endl;
    a.push_back(temp);
  }
  return a;
}

void  create_user(int newsocketdes,string user_id,string pass)
{
	if(userReg.find(user_id)!=userReg.end())
	{
	  char status[]="0";
	  send(newsocketdes,status,sizeof(status),0);
	}
	else
	{
		char status[]="1";
		userReg[user_id]=pass;
	    send(newsocketdes,status,sizeof(status),0);

	}
	close(newsocketdes);
	goto l2;
	l2:
	  cout<<"";
}
void  create_group(int newsocketdes,string group_id)
{
	if(GroupInfo.find(group_id)!=GroupInfo.end())
	{
	  char status[]="0";
	  send(newsocketdes,status,sizeof(status),0);
	}
	else
	{
		char status[]="1";
		GroupInfo[group_id].insert("dummy");
	    send(newsocketdes,status,sizeof(status),0);

	}
	close(newsocketdes);
	goto l2;
	l2:
	  cout<<"";
}
void  list_files(int newsocketdes,string group_id)
{
	set<string>s=GroupAndFile[group_id];
	set<string>:: iterator it;
	string ans="";
	for(it=s.begin();it!=s.end();it++)
	{
       if(it==s.begin())
       {
       	ans+=(*it);
       }
       else
       {
       	ans+=";";
       	ans+=(*it);
       }
	}
	send(newsocketdes,ans.c_str(),strlen(ans.c_str()),0);
	close(newsocketdes);
	goto l2;
	l2:
	  cout<<"";
}
void  join_group(int newsocketdes,string group_id,string user_id)
{
	// cout<<group_id<<" "<<user_id<<endl;
	if(GroupInfo.find(group_id)!=GroupInfo.end())
	{
	  // cout<<"found"<<endl;
	  char  status[]="1";
	  GroupInfo[group_id].insert(user_id);
	  send(newsocketdes,status,sizeof(status),0);
	}
	else
	{
		char  status[]="0";
	    send(newsocketdes,status,sizeof(status),0);

	}
	close(newsocketdes);
	goto l2;
	l2:
	  cout<<"";
}
void  leave_group(int newsocketdes,string group_id,string user_id)
{
	// cout<<group_id<<" "<<user_id<<endl;
	if(GroupInfo.find(group_id)!=GroupInfo.end())
	{
	  if(GroupInfo[group_id].find(user_id)!=GroupInfo[group_id].end())
	  {
	  // cout<<"found"<<endl;
	  char  status[]="1";
	  GroupInfo[group_id].erase(user_id);
	  send(newsocketdes,status,sizeof(status),0);
	 }
	else
	{
		char  status[]="0";
	    send(newsocketdes,status,sizeof(status),0);

	}
    }
    else
	{
		char  status[]="0";
	    send(newsocketdes,status,sizeof(status),0);

	}
	close(newsocketdes);
	goto l2;
	l2:
	  cout<<"";
}
void  list_groups(int newsocketdes)
{
	string info="";
	for(auto &i: GroupInfo)
	{
      info+=i.first;
      info+=";";
	}
	send(newsocketdes,info.c_str(),strlen(info.c_str()),0);
	close(newsocketdes);
	goto l2;
	l2:
	  cout<<"";
}
void  login(int newsocketdes,string user_id,string pass)
{
	if(userReg.find(user_id)!=userReg.end()&&(userReg[user_id]==pass))
	{
	  char  status[]="1";
	  
	  send(newsocketdes,status,sizeof(status),0);
	}
	else
	{
		char  status[]="0";
	    send(newsocketdes,status,sizeof(status),0);

	}
	close(newsocketdes);
	goto l2;
	l2:
	  cout<<"";
}
void  logout(int newsocketdes,string IPport,string group_id)
{
	bool flag=false;
	map<pair<string,string>,set<string> >:: iterator it;
    for(it=seederlist.begin();it!=seederlist.end();it++)
    {
    	if(it->second.find(IPport)!=it->second.end())
    	{
    		flag=true;
    		it->second.erase(IPport);
    	}
    }

	if(flag)
	{
	  char  status[]="1";
	  
	  send(newsocketdes,status,sizeof(status),0);
	}
	else
	{
		char  status[]="0";
	    send(newsocketdes,status,sizeof(status),0);

	}
	close(newsocketdes);
	goto l2;
	l2:
	  cout<<"";
}
void upload_file(int newsocketdes,string group_id,string FileId,string IPport)
{
	// cout<<"in line 112"<<endl;
	pair<string,string>s;
	ifstream in(FileId,ios::ate|ios::binary);
	int size=in.tellg();
	in.close();

	s=make_pair(group_id,FileId);
	FileSizemap[s]=(size);
	FileMap[FileId]=FileId;
	seederlist[s].insert(IPport);
	set<string>ans;
	ans=seederlist[s];
	set<string>::iterator i;
	for(i=ans.begin();i!=ans.end();i++)
	{
      cout<<(*i)<<endl;
	}
	string token="";
	token+=to_string(size);

	GroupAndFile[group_id].insert(FileId);
	send(newsocketdes,token.c_str(),strlen(token.c_str()),0);
	close(newsocketdes);
	goto l2;
	l2:
	  cout<<"";
}

void share_file_details(int newsocketdes,string group_id,string FileId,string IPport)
{
    pair<string,string>s;
	s=make_pair(group_id,FileId);
	seederlist[s].insert(IPport);
	close(newsocketdes);	
	goto l2;
	l2:
	  cout<<"";
}
void seeder_list(int newsocketdes,string group_id,string FileId)
{
	pair<string,string>s;
	s=make_pair(group_id,FileId);
	set<string>ans;
	ans=seederlist[s];
	int ansb=FileSizemap[s];
	string token=to_string(ansb);
	set<string>::iterator i;
	for(i=ans.begin();i!=ans.end();i++)
	{
      token+=";";
      token+=(*i);
	}
	send(newsocketdes,token.c_str(),strlen(token.c_str()),0);
	close(newsocketdes);
	goto l2;
	l2:
	  cout<<"";
}
void serverequest(int newsocketdes,string ip,int port)
{
	char buffer[BUFF_SIZE];
	bzero(buffer,BUFF_SIZE);
	read(newsocketdes,buffer,BUFF_SIZE);

	string r=buffer;
	// cout<<"in line 161"<<r<<endl;
	vector<string> requestarray=ArrayOfString(r,';');
	string request=requestarray[0];
	// cout<<"in line 164"<<request<<endl;
	if(request=="create_user")
	{
	   string user_id=requestarray[1];
	   string pass=requestarray[2];
       create_user(newsocketdes,user_id,pass);
	}
	else if(request=="join_group")
	{
      string group_id=requestarray[1];
      string user_id=requestarray[2];
      join_group(newsocketdes,group_id,user_id);
	}
	else if(request=="create_group")
	{
      string group_id=requestarray[1];
      create_group(newsocketdes,group_id);
	}
	else if(request=="upload_file")
	{
	  cout<<"in line 184"<<endl;
      string group_id=requestarray[1];
      string FileId=requestarray[2];
      string IPport=requestarray[3];
      cout<<"in upload "<<group_id<<" "<<FileId<<" "<<IPport<<endl;
      upload_file(newsocketdes,group_id,FileId,IPport);
	}
	else if(request=="share_file_details")
	{
      string group_id=requestarray[1];
      string FileId=requestarray[2];
      string IPport=requestarray[3];
      share_file_details(newsocketdes,group_id,FileId,IPport);
	}
	else if(request=="seeder_list")
	{
      string group_id=requestarray[1];
      string FileId=requestarray[2];
      //string IPport=requestarray[3];
      seeder_list(newsocketdes,group_id,FileId);
	}
	else if(request=="leave_group")
	{
       string group_id=requestarray[1];
      string user_id=requestarray[2];
      leave_group(newsocketdes,group_id,user_id);
	}
	else if(request=="list_requests")
	{
      
	}
	else if(request=="list_groups")
	{
		list_groups(newsocketdes);

	}
	else if(request=="list_files")
	{
      string group_id=requestarray[1];
      list_files(newsocketdes,group_id);
	}
	else if(request=="login")
	{
       string user_id=requestarray[1];
       string pass=requestarray[2];
       login(newsocketdes,user_id,pass);
	}
	else if(request=="logout")
	{
       string IPport=requestarray[1];
       string group_id=requestarray[2];
       logout(newsocketdes,IPport,group_id);
	}
	cout<<"Returning form serverrequest"<<endl;
	goto l2;
	l2:
	  cout<<"";

}
int main(int argc,char ** argv)
{
	
	if(argc!=3)
	{
		cout<<"Opps! Please give command line arguent in the format"<<endl;
		perror("Error in command line argument list ");
		return -1;
	}
	userReg[s]=p;
    GroupInfo[s].insert("ankush");
    //string clientIPort=argv[1];
    string traker_info_path=argv[1];
    int tracker_no=stoi(argv[2]);
    cout<<"tracker_no="<<tracker_no<<endl;
    char buffer[BUFF_SIZE];

   
    fstream serverfilestream(traker_info_path,ios::in);
    vector<string>IPortTrakers;
    string temp;
    while(getline(serverfilestream,temp,'\n'))
    {
    	IPortTrakers.push_back(temp);
    }
    vector<string>IPort;
    IPort=ArrayOfString(IPortTrakers[0],':');
    TR1ip=IPort[0];
 	TR1port=IPort[1];
 	IPort=ArrayOfString(IPortTrakers[1],':');
 	TR2ip=IPort[0];
 	TR2port=IPort[1];
 	
 	string request;
 	int socketdes;
 	int newsocketdes;
 	int val;
 	socklen_t size;
 	struct sockaddr_in myaddr;
 	struct sockaddr_in otheraddr;
 	if((socketdes=socket(AF_INET,SOCK_STREAM,0))<0)
 	{
 		perror("failed to obtained the socket descriptor");
        return -1;
	}
  myaddr.sin_family=AF_INET;
  myaddr.sin_port=htons(stoi(TR1port));
  inet_pton(AF_INET,TR1ip.c_str() , &myaddr.sin_addr); 
  //myaddr.sin_addr.s_addr=TR1ip;

  bzero(&(myaddr.sin_zero),8);

  if(bind(socketdes,(struct sockaddr *)&myaddr,sizeof(struct sockaddr))==-1)
  {
  	perror("failed to obtained the bind");
  	return -1;
  }
  if(listen(socketdes,BACK)==-1)
  {
     perror("error backlog overflow");
     return -1;

  }
  size=sizeof(struct sockaddr);
  while((newsocketdes=accept(socketdes,(struct sockaddr *)&otheraddr,&size))!=-1)
  {
  	string ip=inet_ntoa(otheraddr.sin_addr);
  	int port=(ntohs(otheraddr.sin_port));
  	cout<<"ip="<<ip<<"port"<<port<<endl;
  	threadVector.push_back(thread(serverequest,newsocketdes,ip,port));
  	size=sizeof(struct sockaddr);
  }
  vector<thread>:: iterator it;
  for(it=threadVector.begin();it!=threadVector.end();it++)

	{
		if(it->joinable()) 
			it->join();
	}
	cout<<"retruning form Tracker main"<<endl;
	goto l2;
	l2:
	  cout<<"";
return 0;
}