all: sbtest_ii.cpp bigraph2.h
	g++ -Wall sbtest_ii.cpp -osbtest

zip:
	cd .. && zip -u max_biclique.zip \
biclique/README.html \
biclique/bigraph2.h \
biclique/bigraph2_MS.h \
biclique/simple_timer.h \
biclique/sbtest_ii.cpp \
biclique/graph.tgif.jpeg \
biclique/compiler \
biclique/n400-x150-d80.bigraph \
biclique/bicli_break.py \
biclique/Makefile \
biclique/test2.bigraph \
