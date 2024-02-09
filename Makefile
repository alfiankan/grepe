run:
	rm -rf build/* && cd build && pwd && cmake .. && make && cd .. && ./build/grepe
