#Makefile
#Eli Oceanak
#COSC 3750 Spring 2021
#makefile for Hw8

CC=gcc
CFLAGS=-Wall -gddb
RM=/bin/rm -f

wyshell : wyshell.c wyscanner.c scannerFunctions.c
	${CC} -I . ${CCFLAGS} wyshell.c wyscanner.c scannerFunctions.c -o wyshell

clean : 
	${RM} wyshell