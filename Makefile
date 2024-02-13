run:
	rm -rf build/* && cd build && pwd && cmake .. && make && cd .. && ./build/grepe

test:
	rm -rf build/* && cd build && pwd && cmake .. && make && cd .. && ./build/grepe-test

build-bin:
	rm -rf build/* && cd build && pwd && cmake .. && make && cd ..
