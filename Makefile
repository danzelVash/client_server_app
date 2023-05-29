.SILENT:

run-server_one_file:
	gcc server_one_file.c priority_queue.c -o server_one_file
	./server_one_file


run-server:
	gcc server_entry.c priority_queue.c server.c -o server
	./server

run-client:
	gcc client_entry.c client.c -o client
	./client


test-client:
	gcc client.c client_test.c -o client_test -lcunit
	./client_test


test-queue:
	gcc queue_test.c priority_queue.c -o queue_test
	./queue_test
