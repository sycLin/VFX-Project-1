alignAll: align1 align2

align1: a.out
	./a.out input/scene1 detail.txt output/scene1/aligned

align2: a.out
	./a.out input/scene2 expose.txt output/scene2/aligned

a.out: main.cpp
	g++ main.cpp `pkg-config --libs --cflags opencv` -o a.out

clean:
	@echo "removing executable: a.out"
	rm -rf a.out
	@echo "removing aligned images"
	rm -rf output/scene1/aligned/*
	rm -rf output/scene2/aligned/*
	
