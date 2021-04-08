c: src/*.c src/*.h
	cpplint --filter=-whitespace,-legal/copyright,-readability/check,-readability/casting,-build/include,-build/header_guard --recursive src
	gcc -Wall src/*.c -o c

clean:
	rm -f c
