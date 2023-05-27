#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "client.h"

void test_receive_message() {
    // Dummy socket file descriptor
    int sockfd = 0;

    // Redirect stdout to a file
    freopen("output.txt", "w", stdout);

    // Call the function under test
    receive_message(sockfd);

    // Restore stdout
    freopen("/dev/tty", "w", stdout);

    // Read the output file
    FILE *file = fopen("output.txt", "r");
    char buffer[BUFFER_SIZE];
    fgets(buffer, sizeof(buffer), file);
    fclose(file);

    // Assert the received message
    CU_ASSERT_STRING_EQUAL(buffer, "Connection closed by the server.\n");
}

void test_send_message() {
    // Dummy socket file descriptor
    int sockfd = 0;

    // Redirect stdin to a file
    freopen("input.txt", "w", stdin);
    fprintf(stdin, "Hello, server!\n");
    fprintf(stdin, "exit\n");
    fclose(stdin);

    // Call the function under test
    send_message(sockfd);

    // Assert the sent messages
    FILE *file = fopen("input.txt", "r");
    char buffer[BUFFER_SIZE];
    fgets(buffer, sizeof(buffer), file);
    CU_ASSERT_STRING_EQUAL(buffer, "Hello, server!\n");
    fgets(buffer, sizeof(buffer), file);
    CU_ASSERT_STRING_EQUAL(buffer, "exit\n");
    fclose(file);
}

int main() {
    CU_initialize_registry();

    CU_pSuite suite = CU_add_suite("Client Suite", NULL, NULL);
    CU_add_test(suite, "Test receive_message", test_receive_message);
    CU_add_test(suite, "Test send_message", test_send_message);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
