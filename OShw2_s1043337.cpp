using namespace std;
#include<fcntl.h>
#include<sys/stat.h>
#include<iostream>
#include<string>
#include <sstream>
#include<vector>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
int function(string user);
void Cutword(vector<char *> &Instruction,string sentence,char token) {
	stringstream ss(sentence);
	string tmp;
	while (getline(ss, tmp,token)) {//切字
		char *ppp = new char[10240];
		strcpy(ppp,tmp.c_str());
		Instruction.push_back(ppp);
	}
}
int main(){
	string tmp,tmp1,tmp2;
	char *arg[50][10240];
	vector<char *> Instruction,symbol;
	vector<string> Location;
	struct passwd *my_info;
	//先放location	
	uid_t my_uid = getuid();
	my_info = getpwuid(my_uid);
	char buf[80];
	getcwd(buf,sizeof(buf));
	Location.push_back(my_info->pw_name);
	Location.push_back("@");
	Location.push_back(buf);	
	while (1) {
		for (int i = 0; i < Location.size(); i++)//印出LOCATION
			if (i == 0)
				cout << "\033[1;32m" << Location[i];
			else
				cout << "\033[1;34m" << Location[i];
		cout << "\033[1;0m" << ">";
		char *line = readline(" ");
		tmp = line;
		Cutword(Instruction, tmp, ' ');
		if (Instruction.size() == 0)//防呆 沒輸入東西
			continue;
		int choice = function(Instruction[0]);//程式執行
		for (int i = 0, a = 0, b = 0; i < Instruction.size(); i++) {
			if (strcmp(Instruction[i], "|") != 0 && strcmp(Instruction[i], ">") != 0
				&& strcmp(Instruction[i], "<") != 0&&strcmp(Instruction[i], "1>") != 0
				&&strcmp(Instruction[i], "2>") != 0) {
				arg[a][b] = Instruction[i];
				b++;
			}
			else {
				symbol.push_back(Instruction[i]);
				a++;
				b = 0;
			}
		}
		if (choice == 1) {
			if (symbol.size() == 0||strcmp(symbol[0], ">") == 0||strcmp(symbol[0], "1>") == 0
				|| strcmp(symbol[0], "<") == 0||strcmp(symbol[0], "2>") == 0)
			{
				pid_t pid;
				pid = fork();
				
				if (pid == 0) {
					if(symbol.size()==0){
					execvp(arg[0][0], arg[0]);
					}
					if (strcmp(symbol[0], "<") == 0) {
						//cout<<"1";
						int fd_in = open(arg[1][0], O_RDONLY);
						dup2(fd_in, 0);
						close(fd_in);
						execvp(arg[0][0], arg[0]);

					}
					else if(symbol.size()!= 0){
					//cout<<"2";
					int fd;							
					fd=open(arg[1][0],O_WRONLY | O_CREAT,0644);
					if(strcmp(symbol[0], "2>") != 0)					
					dup2(fd,1);
					if(strcmp(symbol[0], "2>") == 0)
					dup2(fd,2);
					close(fd);
					execvp(arg[0][0], arg[0]);
					}
					
										
				}
				else
				{
					//cout<<"我是老爸"<<endl;
					wait(NULL);
				}
			}	
			else
			{
				//cout<<"3";
				int symbolSize=symbol.size();
				if(strcmp(symbol[symbol.size()-1], ">") == 0||
					strcmp(symbol[symbol.size() - 1], "<")==0||strcmp(symbol[symbol.size() - 1], "1>")==0||
				strcmp(symbol[symbol.size() - 1], "2>")==0){
				symbolSize--;
				}
				pid_t *pid = new int[symbolSize + 1];
				int **pipe_fd = new  int*[symbolSize];
				int process = symbolSize + 1;
				for (int i = 0; i < symbolSize; i++)
					pipe_fd[i] = new int[2];
				for (int i = 0; i < process; i++) {
					if(i!=process-1)//最後一個 process 不用宣告
					pipe(pipe_fd[i]);
					pid[i] = fork();
					if (pid[i]== 0) {//中間
						if (i > 0 && i < process-1) {
							close(pipe_fd[i][0]);//開啟另一個
							dup2(pipe_fd[i][1], 1);
							close(pipe_fd[i][1]);

							close(pipe_fd[i - 1][1]);//關閉之前的
							dup2(pipe_fd[i - 1][0], 0);
							close(pipe_fd[i - 1][0]);
							execvp(arg[1][0], arg[1]);
						}
						else if (i == process - 1) {//最後
							//如果最後有寫檔int fd;
							if(strcmp(symbol[symbol.size()-1], ">") == 0){	
							int fd;							
							fd=open(arg[i+1][0],O_WRONLY | O_CREAT,0644);
							if(strcmp(symbol[symbol.size() - 1], "2>")!=0)
							dup2(fd,1);
							if(strcmp(symbol[symbol.size() - 1], "2>")==0)
							dup2(fd,2);					
							close(fd);
							}
							close(pipe_fd[i - 1][1]);
							dup2(pipe_fd[i - 1][0], 0);
							close(pipe_fd[i - 1][0]);
							execvp(arg[i][0], arg[i]);
						}
						else if (i == 0) {//第一個
							if (strcmp(symbol[symbol.size() - 1], "<") == 0) {
								int fd_in = open(arg[process][0], O_RDONLY);
								dup2(fd_in, 0);
								close(fd_in);
							}
							close(pipe_fd[i][0]);
							dup2(pipe_fd[i][1], 1);
							close(pipe_fd[i][1]);
							execvp(arg[i][0], arg[i]);
						}
					}
					else {
						if (i != 0) {
							close(pipe_fd[i-1][1]);
							close(pipe_fd[i-1][0]);
						}
						wait(NULL);
					}
				}
			}
		}
		else if(choice==2){//切換目錄
			if(Instruction.size()==1){
			getcwd(buf, sizeof(buf));
			Location[2] =buf;
			chdir("~");
			break;
			}
			getcwd(buf, sizeof(buf));
			tmp1 = buf;
			tmp2 = Instruction[1];
			 if (chdir(tmp2.c_str()) == -1){
				cout<<"沒有這個目錄喔～"<<endl;
				chdir(buf);
			}
			else{
				getcwd(buf, sizeof(buf));
				Location[2] =buf;
			}
		}
		else{
			cout << "ByeBye! "<<endl;
			return 0;
		}
		Instruction.clear();
		symbol.clear();
		for(int i=0;i<50;i++)
			for(int a=0;a<1024;a++)
				arg[i][a]=NULL;
	}
	
	//system("pause");
}
int function(string user){

	 if (user == "cd")
		return 2;
	else if (user == "exit")
		return 3;
	else
		return 1;
}
