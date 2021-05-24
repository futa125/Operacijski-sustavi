#include <stdlib.h>
#include <stdio.h>
#include <time.h>


void print_storage (char *storage, int storage_size) {
    for (int i = 0; i < storage_size; i++) {
      printf("%c", storage[i]);
    }

    printf("\n");
}

int find_free_space (char *storage, int storage_size, int segment_size) {
    int free_space_count = 0;
    int free_space_index = 0;

    for (int i = 0; i < storage_size; i++) {
        if (storage[i] == '-') {
            free_space_count++;
        } else {
            free_space_count = 0;
            free_space_index = i + 1;
        }

        if (free_space_count == segment_size) {
            return free_space_index;
        }
    }

    return -1;
}

void write_data (char *storage, int free_space_index, int segment_size, char segment_index) {
    for (int i = free_space_index; i < free_space_index + segment_size; i++) {
        storage[i] = segment_index;
    }
}


void delete_data (char *storage, int storage_size, char segment_index) {
    for (int i = 0; i < storage_size; i++) {
        if (storage[i] == segment_index) {
            storage[i] = '-';
        }
    }
}

void defragment (char *storage, int storage_size) {
    char tmp_array[storage_size];
    int i = 0;
    int j = 0;

    for (i; i < storage_size; i++) {
        if (storage[i] != '-') {
            tmp_array[j] = storage[i];         
            j++;
        }
    }

    for (j; j < storage_size; j++) {
        tmp_array[j] = '-';
    }

    for (i = 0; i < storage_size; i++) {
        storage[i] = tmp_array[i];
    }
}


int main(int argc, char const* argv[]) {
    srand(time(NULL));
    int storage_size = atoi(argv[1]);
    char storage[storage_size];

    for (int i = 0; i < storage_size; i++) {
        storage[i] = '-';
    }

    print_storage(storage, storage_size);

    char pressed_key;
    char segment_to_delete_index;
    char segment_index = 'a';

    while (1) {
        printf("Press valid key: ");
        scanf(" %c", &pressed_key);
        
        int segment_size = (rand() % 6) + 1;
        int free_space_index;

        switch (pressed_key) {
        
        case 'Z':
            printf("Writing segment with index %c and size %d\n", segment_index, segment_size);

            free_space_index = find_free_space(storage, storage_size, segment_size);

            if (free_space_index != -1) {
                printf("Writing data to memory...\n");
                write_data(storage, free_space_index, segment_size, segment_index);
                
                print_storage(storage, storage_size);

                segment_index++;

            } else {
                printf("Not enough free space, defragmenting...\n");
                defragment(storage, storage_size);
                
                free_space_index = find_free_space(storage, storage_size, segment_size);
                
                if (free_space_index != -1) {
                    printf("Writing data to memory...\n");
                    write_data(storage, free_space_index, segment_size, segment_index);
                    segment_index++;
                } else {
                    printf("No space\n");
                } 
                
                print_storage(storage, storage_size);
            }
            
            break;
        
        case 'O':
            printf("Select segment to delete: ");
            scanf(" %c", &segment_to_delete_index);

            delete_data(storage, storage_size, segment_to_delete_index);

            print_storage(storage, storage_size);
            break;

        default:
            printf("Invalid input key!\n");
            break;
        }

        printf("\n");
    }

    return 0;
}