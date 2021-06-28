start:test.o oppBleMeshResp.o opple_sigmesh_application.o oppQueu.o oppList.o
	gcc -o test test.o oppBleMeshResp.o opple_sigmesh_application.o oppQueu.o oppList.o -lpthread
test.o:test.c
	gcc -c test.c
oppBleMeshResp.o:oppBleMeshResp.c
	gcc -c oppBleMeshResp.c
opple_sigmesh_application.o:opple_sigmesh_application.c
	gcc -c opple_sigmesh_application.c
oppQueu.o:oppQueu.c
	gcc -c oppQueu.c
oppList.o:oppList.c
	gcc -c oppList.c
clean:
	rm -rf oppBleMeshResp.o opple_sigmesh_application.o

