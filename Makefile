build: 
	gcc -o 352 src/main.c
debug: 
	gcc -o 352 -g src/main.c
	ctags -Rf tags src/
dev:
	ctags -Rf tags src/
clean:
	rm 352 tags
