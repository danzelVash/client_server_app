.SILENT:

run-server_one_file:
	gcc server_one_file.c priority_queue.c -o server_one_file
	./server_one_file


run-server:
	gcc server_entry.c priority_queue.c server.c -o server
	./server


test-server:
	gcc server.c priority_queue.c server_test.c -o server_test -lcunit
	./server_test

run-client:
	gcc client_entry.c client.c -o client
	./client


test-client:
	gcc client.c client_test.c -o client_test -lcunit
	./client_test


test-queue:
	gcc queue_test.c priority_queue.c -o queue_test
	./queue_test
