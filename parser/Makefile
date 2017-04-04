all:	parser imgbuild colorcvt
colorcvt: colorcvt.o
	gcc -o colorcvt colorcvt.o -lpng
colorcvt.o : colorcvt.c
	gcc -c colorcvt.c -o colorcvt.o -Wall
parser:	parser.o
	gcc -o parser parser.o
parser.o:	parser.c
	gcc -g -c parser.c -o parser.o -Wall
imgbuild:	imgbuild.o
	gcc -o imgbuild imgbuild.o -lpng
imgbuild.o:	imgbuild2.c
	gcc -g -c imgbuild2.c -o imgbuild.o -Wall
imgconvert: imgconvert.o
	gcc -o imgconvert imgconvert.o -lpng
imgconvert.o:	imgconvert.c
	gcc -g -c imgconvert.c -o imgconvert.o -Wall
clean:	
	rm -f core *.o parser imgbuild colorcvt
archive:
	cd .. ;	tar -zcvf parser.tgz ./parser/*.c ./parser/SPEC ./parser/README ./parser/Makefile ./parser/convert.sh ./parser/FAQ ./parser/ascend.pl ; cd parser
