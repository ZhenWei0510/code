#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int original_list[] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8}; /* Assume there are even numbers */
#define N_LIST (sizeof(original_list)/sizeof(int))
#define usec_elapsed(s,e) (1000000 * ((e).tv_sec-(s).tv_sec) + ((e).tv_usec-(s).tv_usec))

void listncopy(int *src, int *dst, int n) {
    for (int i = 0; i < n; i++) dst[i] = src[i];
}

void print_list(char *id, char *msg, int *first, int n) {
    printf("%s %s: ", id, msg);
    while (n-- > 0) printf("%d ", *first++);
    printf("\n");
}

struct ThreadArgs {
    char *id;
    int *first;
    int n;
};

void *do_sort(void *args) {
    struct ThreadArgs *ptr = (struct ThreadArgs *)arg;
    char *id = ptr->id;
    int *first = ptr->first;
    int n = ptr->n;

    struct timeval start, end;
    
    print_list(id, "Sub-Old", first, n);
    gettimeofday(&start,NULL);
    /* selection sort */
    for (int i = 0; i < n - 1; i++) {
        int min_index = i;
        for (int j = i + 1; j < n; j++) {
            if (first[j] < first[min_index]) {
                min_index = j;
            }
        }
        if (min_index != i) {
            int temp = first[i];
            first[i] = first[min_index];
            first[min_index] = temp;
        }
    }
    gettimeofday(&end,NULL);
    print_list(id, "SUB-New", first, n);
    printf("%s spent %ld usec\n", id, usec_elapsed(start, end));
    
    unsigned long *usec_ptr = malloc(sizeof(unsigned long) * 1);
    *usec_ptr = usec_elapsed(start, end);
    pthread_exit((void*) usec_ptr);
}

void *do_merge(void *arg) {
    gettimeofday(&start,NULL);

    gettimeofday(&end,NULL);

    return usec_elapsed(start, end);
}


int main(int argc, char *argv[]) {

    pthread_t tid, tid0, tid1, tidm; /* the thread identifier */
    pthread_attr_t attr, attr0, attr1, attrm; /* set of thread attributes */
    /* set the default attributes of the thread */
    pthread_attr_init(&attr);
    pthread_attr_init(&attr0);
    pthread_attr_init(&attr1);
    pthread_attr_init(&attrm);
    /* create the thread */
    /* wait for the thread to exit */

    int mylist[N_LIST];
    listncopy(mylist, original_list, N_LIST);
    struct ThreadArgs threadArgs = {"A1105526-X", mylist, N_LIST};
    
    pthread_create(&tid, &attr, do_sort, &threadArgs);
    pthread_join(tid,NULL);

    print_list("A1105526-M", "All-Old", original_list, N_LIST);

    // Create sorting thread0: t0=do_sort(“A1015501#0 “, original_list, N_LIST/2);
    // Create sorting thread1: t1=do_sort(“A1015501#1 “,original_list+N_LIST/2,N_LIST/2); Step 3
    // Create merging thread: tm = do_merge(A1015501#M “, original_list, N_LIST); Step 4

    // print_list("A1015501-M", "All-New", original_list, N_LIST);

    // printf("A1015501-M spent %ld usec\n", id, t0+t1+tm);

    return 0;
}