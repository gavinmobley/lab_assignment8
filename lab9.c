#include <stdio.h>
#include <stdlib.h>

#define HASH_SIZE 10  // Size of the hash table (number of buckets)

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
};

// Node for linked list (used for chaining)
struct Node
{
    struct RecordType data;
    struct Node* next;
};

// HashType (hash table)
struct HashType
{
    struct Node* head;  // Pointer to the head of the linked list
};

// Hash function
int hash(int key)
{
    return key % HASH_SIZE;  // Simple modulus operation for hashing
}

// Parse input data from file
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i;
    struct RecordType* pRecord;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);

        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }

        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d %c %d\n", &pRecord->id, &pRecord->name, &pRecord->order);
        }

        fclose(inFile);
    }

    return dataSz;
}

// Display records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// Initialize hash table
void initHashTable(struct HashType hashTable[], int hashSize)
{
    int i;
    for (i = 0; i < hashSize; ++i)
    {
        hashTable[i].head = NULL;  // Initialize each bucket head to NULL
    }
}

// Insert record into hash table using chaining
void insertIntoHash(struct HashType hashTable[], int hashSize, struct RecordType record)
{
    int index = hash(record.id);  // Compute hash index

    // Create a new node for the record
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL)
    {
        printf("Memory allocation error\n");
        exit(-1);
    }
    newNode->data = record;
    newNode->next = NULL;

    // Insert at the beginning of the linked list
    if (hashTable[index].head == NULL)
    {
        hashTable[index].head = newNode;
    }
    else
    {
        newNode->next = hashTable[index].head;
        hashTable[index].head = newNode;
    }
}

// Display records in the hash table
void displayRecordsInHash(struct HashType hashTable[], int hashSize)
{
    int i;
    printf("\nHash Table:\n");
    for (i = 0; i < hashSize; ++i)
    {
        printf("Index %d -> ", i);
        struct Node* current = hashTable[i].head;
        while (current != NULL)
        {
            printf("%d %c %d -> ", current->data.id, current->data.name, current->data.order);
            current = current->next;
        }
        printf("NULL\n");
    }
}

int main(void)
{
    struct RecordType* pRecords;
    int recordSz = 0;
    int i;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Initialize hash table
    struct HashType hashTable[HASH_SIZE];
    initHashTable(hashTable, HASH_SIZE);

    // Insert records into hash table
    for (i = 0; i < recordSz; ++i)
    {
        insertIntoHash(hashTable, HASH_SIZE, pRecords[i]);
    }

    // Display records in the hash table
    displayRecordsInHash(hashTable, HASH_SIZE);

    // Clean up allocated memory
    for (i = 0; i < HASH_SIZE; ++i)
    {
        struct Node* current = hashTable[i].head;
        while (current != NULL)
        {
            struct Node* temp = current;
            current = current->next;
            free(temp);  // Free allocated memory for nodes
        }
    }

    free(pRecords);  // Free allocated memory for records

    return 0;
}

