CXX = g++
CFLAGS = -std=c++17 -O2 -Wall -g 

TARGET = webserver
OBJS = ../code/log/*.cpp ../code/pool/*.cpp ../code/timer/*.cpp \
       ../code/http/*.cpp ../code/server/*.cpp \
       ../code/buffer/*.cpp ../code/main.cpp

all: $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o ../bin/$(TARGET)  -pthread -lmysqlclient -L /usr/lib64/mysql

# clean:
# 	rm -rf ../bin/$(OBJS) $(TARGET)




