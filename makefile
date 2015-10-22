newbaka:tasks.o newbaka.cpp newbaka.h  directoryconverter.o mainpurgeloop.cpp mainpurgeloop.h
	LC_ALL=C g++  -std=c++11 -g -rdynamic  newbaka.cpp  -o newbaka tasks.o  directoryconverter.o /usr/lib/i386-linux-gnu/libboost_filesystem.so  /usr/lib/i386-linux-gnu/libboost_thread.so -lboost_filesystem -lboost_system -lboost_thread 
tasks.o:tasks.cpp tasks.h
	LC_ALL=C g++ -std=c++11  -g -rdynamic -c -o tasks.o -lboost_filesystem -lboost_system  tasks.cpp
directoryconverter.o:directoryconverter.h directoryconverter.cpp
	LC_ALL=C g++ -std=c++11  -g -rdynamic -c -o directoryconverter.o -lboost_filesystem -lboost_system  directoryconverter.cpp
clean:
	rm *.o
