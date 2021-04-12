c: src/*.c src/*.h
#	cpplint --filter=-whitespace,-legal/copyright,-readability/check,-readability/casting,-build/include,-build/header_guard --recursive src
	gcc -Wall src/*.c -o c -pthread


clean:
	rm -f c


infer:
	infer run -- make


# Valgrind calls may be adjusted (in the way it calls our program)

valgrind: c
	valgrind -q --gen-suppressions=yes --leak-check=full --show-leak-kinds=all --verbose ./c -t 3 /tmp/foo > /dev/null

valgrindf: c
	valgrind -q --gen-suppressions=yes --leak-check=full --show-leak-kinds=all --verbose --log-file=valgrind-out ./c -t 3 /tmp/foo > /dev/null
