#include <malloc.h> 
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 

//структура ячейки
struct node {
  struct node *next;
  int info;
};
//проверка файла
void check_file(FILE *f) {
  //проверка содержимого
  fseek(f, 0,
        SEEK_END); //переводим f в конец файла, указывает на последний элемент
  if (!ftell(f)) { //возращает текущее положение в файле. Если в самом начале,
                   //тоr вернет 0
    puts("File is empty");
    exit(2);
  }
  fseek(f, 0,
        SEEK_SET); //снова переводим в конец, чтобы работать со стеком (LIFO)
}
//формирование стека
struct node *stack() {
  int n, i = 0;   // i для указния строчки с ошибкой
  FILE *f;       
  double nc;      // number check - для проверки на int
  char fname[81]; //имя входного файла
  struct node *p, *lst; // p - указатель на текущую добавляемую запись,  lst -
                        // указатель на предыдущую запись (на вершину стека)
  puts("Enter name of file");
  //gets(fname);
  fgets(fname, sizeof(fname), stdin); 
  
  size_t len = strlen(fname);
    if (len > 0 && fname[len - 1] == '\n') {
        fname[len - 1] = '\0'; // Remove the newline character
    }
    
  if (!(f = fopen(fname, "r"))) {
    puts("File not found");
    exit(1);
  }
  //проверка содержимого
  check_file(f);

  lst = NULL; //до ввода данных стек пустой
  printf("Reading file %s\n", fname);
  int r = fscanf(f, "%lf", &nc);
  while (r == 1) { //считает число
    //ввод до Ctrl+Z или символа
    if (nc != (int)nc) { //проверка на целочисленность
      printf("Error in %d string\n", i + 1);
      exit(3);
    }
    n = nc; //если целое, присвоим n это значение
    p = (struct node *)malloc(
        sizeof(struct node)); //выделяем память в структуре под n и указатель на
                              //следующий элемент
    p->info = n;              //записываем n в структуру
    p->next = lst; //в ячейку next записываем указательна предыдущую вершину(при
                   //первой интерации NULL)
    lst = p;       // вершина = текущая запись
    i++;           //показатель введенной строки
    r = fscanf(f, "%lf", &nc);
    if (!r) {
      printf("Error in %d string\n", i + 1);
      lst = NULL;
      exit(4);
    }
  }
  fclose(f); //закрываем файл
  p = lst;
  if (p != NULL) {
    puts("");
  }
  return (lst); //вернет указатель на вершину стека
};

struct node *add_a1(struct node *lst, int a1) {
    struct node *p = lst;
    int count = 1;

    while (p != NULL) {
        if (count % 2 == 1) { // Проверяем нечетный ли номер узла
            struct node *newNode = (struct node *)malloc(sizeof(struct node));
            newNode->info = a1;
            newNode->next = p->next;
            p->next = newNode;
            p = newNode; // Переходим к следующему узлу для проверки
        }

        p = p->next;
        count++;
    }

    p = lst;
    if (p != NULL) {
        puts("After insert:");
        while (p) {
            printf("%7d", p->info);
            p = p->next;
        }
        puts("");
    }

    return lst;
}
//сортировка
int bubble(struct node *lst, int z) {
    struct node *p = lst, *firstZ = NULL;
    struct node *p2 = NULL;
    int tmp = 0, flag = 1;

    // находим первый элемент, равный Z
    while (p != NULL && firstZ == NULL) {
        if (p->info == z) {
            firstZ = p;
        }
        p = p->next;
    }

    if (firstZ == NULL) {
        puts("No element equal to Z found in the list");
        return 0; // выходим из функции, если элемента Z нет в списке
    }

    // сортировка "пузырьком" после элемента = Z
    p = firstZ->next;
    while (p != NULL) {
        p2 = p;
        while (p2 != NULL) {
            if (p->info > p2->info) { 
                tmp = p->info;
                p->info = p2->info;
                p2->info = tmp;
            }
            p2 = p2->next;
        }
        p = p->next;
    }

    // выводим отсортированный массив
    p = lst;
    puts("After bubble sort:");
    while (p) {
        printf("%7d", p->info);
        p = p->next;
    }
    puts("");

    return tmp; 
}

//вывод списка на экран
void printspisok(struct node *lst, char num[], int f, char fname1[]) {
  struct node *p = lst;
  FILE *fout;
  if (f)
    fout = fopen(fname1, "w");
  else
    fout = fopen(fname1, "a");
    
  if (!(fout)) {
    puts("File not found");
    exit(1);
  }
  
  fprintf(fout, "Task #%s\n", num);
  //if (num == "1")
  if (strcmp(num, "1") == 0)
    fprintf(fout, "Original list:\n");
  else
    fprintf(fout, "Changed list:\n");

  while (p) {
    fprintf(fout, "%7d", p->info);
    p = p->next;
  }
  fprintf(fout, "\n\n");
  fclose(fout);
}

void printspisok_dop(struct node *dop, char num[], int f, char fname1[]) {
//   p = (struct node *)malloc(
//         sizeof(struct node));
  struct node *p = dop;
  
  FILE *fout;
  if (f)
    fout = fopen(fname1, "w");
  else
    fout = fopen(fname1, "a");
    
  if (!(fout)) {
    puts("File not found");
    exit(1);
  }
  
  fprintf(fout, "Task #%s\n", num);
  //if (num == "1")
  if (strcmp(num, "1") == 0)
    fprintf(fout, "Original list:\n");
  else
    fprintf(fout, "Changed list:\n");

  while (p) {
    fprintf(fout, "%7d", p->info);
    p = p->next;
  }
  fprintf(fout, "\n\n");
  fclose(fout);
}


//освобождение памяти
void free_memory(struct node *lst) {
  struct node *p = lst, *next = lst;
  while (next) {
    next = p->next; //на текущий ставим следующий элемент
    free(p);  //очищаем текущий
    p = next; //запоминаем место следующего. Далее, зайдя в цикл, он будет
              //текущим
  }
  puts("\nNow memory is free");
}

int find_sr_ar(struct node *lst) {
    int average;
    struct node *p = lst;
    int sum = 0;
    int count = 0;

    while (p != NULL) {
        sum += p->info;
        count++;
        p = p->next;
    }

    average = (int)sum / count;
    return average;
}

struct node *dopoln(struct node *lst, int average) {
    struct node *p = lst;
    struct node *new_head = NULL;
    //struct node *new_tail = NULL;

    while (p != NULL) {
        if (p->info > average) {
            struct node *new_node = (struct node *)malloc(sizeof(struct node));
            new_node->info = p->info;
            new_node->next = new_head;
            new_head = new_node;

            // if (new_head == NULL) {
            //     new_head = new_node;
            //     new_tail = new_node;
            // } else {
            //     new_tail->next = new_node;
            //     new_tail = new_node;
            // }
        }

        p = p->next;
    }

    // *new_lst = new_head;
    return new_head;
}

int main() {
  int min = 0, f = 1;
  char fname1[81];
  struct node *lst = stack();
  
  //первое задание
  puts("Enter the name of the output file");
  fgets(fname1, sizeof(fname1), stdin);
  if (!lst)
    puts("List is empty");
  else {
    printspisok(lst, "1", f, fname1);
  } 
    //второе задание
    puts("\nEnter A1");
    char input[100];  
    int a1;

    printf("Enter a number to insert into the list after each odd-indexed element: ");
    fgets(input, sizeof(input), stdin);  

    if (sscanf(input, "%d", &a1) != 1) {
        printf("Invalid input, please enter a valid number.\n");
        return 1; 
    }

   
    lst = add_a1(lst, a1);
    if (lst == NULL) {
      puts("\nСomplete removal of elements");
    } else {
      f = 0;
      printspisok(lst, "2", f, fname1);
    }  
      
    puts("\nEnter Z");
    char input3[100];  
    int z;
    
    printf("Enter a number after which the array will be bubble-sorted: ");
    fgets(input3, sizeof(input3), stdin);  
    
    if (sscanf(input3, "%d", &z) != 1) {
        printf("Invalid input, please enter a valid number.\n");
        return 1; 
    }

    //третье задание
    f = bubble(lst, z);
    if (!f)
     printf("No sort");
    else {
        f = 0;
        printspisok(lst, "3", f, fname1);
    }
    //free_memory(lst); // освобождение памяти, занятой списком 
    
    //дополнительное задание
    struct node *dop = NULL;
    //dop = (struct node *)malloc(sizeof(struct node));
    int average;
    average = find_sr_ar(lst);
    dop = dopoln(lst, average);
    printspisok_dop(dop, "4", f, fname1);
    free_memory(dop); // освобождение памяти, занятой списком 
 
    free_memory(lst); // освобождение памяти, занятой списком 
  return 0;
}
