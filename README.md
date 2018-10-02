# Linux_simple_shell

軟體下載
1.	g++ 編譯器 sudo apt-get install g++ build-essential
2.	libreadline-dev, libreadline6, libtinfo-dev, readline-doc
3.	sudo apt-get install libreadline-dev
4.	sudo apt-get install libreadline6
5.	sudo apt-get install libtinfo-dev
6.	sudo apt-get install readline-doc

使用:
1.	切到目前使用的資料夾
2.	輸入 g++ OShw2_s1043337.cpp -lreadline -o OShw2_s1043337.out
3.	執行OShw2_s1043337.out

功能:
1.可以接收並執⾏任意不帶參數的指令	
	利用fork()後,讓子程序利用execvp()執行切割好的指令。
　　 
![image](https://github.com/netleoian/Linux_simple_shell/blob/master/sp20181003_014817_930.png)

2. 可以接收並任何帶有參數的指令。
	利用fork()後,讓子程序利用execvp()執行切割好的指令。
	 
![image](https://github.com/netleoian/Linux_simple_shell/blob/master/sp20181003_014823_317.png)
4.	可以將多個不同的指令串接起來，提供pipe ‘|’的功能，讓使⽤者可以將⼀個程式的輸 出(standard output)接到另⼀個程式的輸⼊， 需要⽀援多個程式串接。(最多四個)
使用到pipe()和dup2()的部分,利用for迴圈建立每個子程序,和pipe再依序第一個,中間和最後的程序的不同,做不同的close()的部分。
　 
![image](https://github.com/netleoian/Linux_simple_shell/blob/master/sp20181003_014437_199.png)
5.	提供redirect ‘, 1>, 2> … ’的功能讓程式將標 準輸出、標準錯誤輸出等重新導向到新目的 地 （本作業僅要求從檔案導入跟導出到檔案， 會與pipe混合運用）
使用到open()的部分,再利用dup2()更改輸出的位置。
  
![image](https://github.com/netleoian/Linux_simple_shell/blob/master/sp20181003_014449_512.png)
![image](https://github.com/netleoian/Linux_simple_shell/blob/master/sp20181003_014452_305.png)
 
![image](https://github.com/netleoian/Linux_simple_shell/blob/master/sp20181003_014500_261.png)
