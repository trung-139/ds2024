#include <stdio.h>
#include <stdlib.h>
#include <rpc/rpc.h>

// Define the program number for RPC service
#define FILE_TRANSFER_PROG 0x30000001
#define FILE_TRANSFER_VERS 1  // Example version number, you can choose your own

// Define the structure for file transfer
struct file_transfer {
    char *filename;
    char *data;
    int size;
};

// Define the RPC function for file transfer
int *file_transfer_rpc(struct file_transfer *file, CLIENT *client) {
    static int result = 0;
    FILE *fp;

    // Open the file in write mode
    fp = fopen(file->filename, "wb");
    if (fp == NULL) {
        printf("Error opening file!\n");
        result = -1;
        return &result;
    }

    // Write the data to the file
    fwrite(file->data, sizeof(char), file->size, fp);

    // Close the file
    fclose(fp);

    // Return success
    result = 1;
    return &result;
}

int main(int argc, char *argv[]) {
    // Create the RPC client
    CLIENT *client;
    char *server;
    struct file_transfer file;

    // Check if the server address is provided
    if (argc < 2) {
        printf("Usage: %s server_address\n", argv[0]);
        exit(1);
    }

    // Get the server address
    server = argv[1];

    // Create the RPC client
    client = clnt_create(server, FILE_TRANSFER_PROG, FILE_TRANSFER_VERS, "tcp");
    if (client == NULL) {
        clnt_pcreateerror(server);
        exit(1);
    }

    // Set the file details
    file.filename = "file.txt";
    file.data = "This is a test file.";
    file.size = strlen(file.data);

    // Call the RPC function for file transfer
    if (file_transfer_rpc(&file, client) == NULL) {
        clnt_perror(client, server);
        exit(1);
    }

    // Destroy the RPC client
    clnt_destroy(client);

    return 0;
}
