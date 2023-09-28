//
// Created by gav66 on 9/20/2023.
//



#include <stdint.h>


#ifndef LLP_MAIN_H
#define LLP_MAIN_H

#endif //LLP_MAIN_H

const uint16_t PAGE_SIZE = 4096;
const uint64_t KEY = 2143243242;

enum file_status {
    success = 0,
    fail
//    failed_to_open,
//    failed_to_create,
//    failed_to_close
};


enum type_data {
    tp_int = 0,
    tp_float,
    tp_boolean,
    tp_string
};

//client struct

struct attribute {
    char *name;
    enum type_data t;
    struct attribute *next_attr;
};

// Схема таблицы
struct table {
    char *name;
    struct attribute *attr;
};

struct free_page {
    uint64_t page_link;
    struct free_page *next_page;
};

struct string_pool_elem {
    uint64_t size;
    uint64_t last_page_link;
    struct string_pool_elem *prev_elem;
};

struct header {
    uint64_t key;
    uint64_t page_metadata;
    uint64_t free_pages;
    struct string_pool_elem *string_pool;
};

struct database{
    struct header* header;
    FILE *file;
};


//file struct
struct file_page_header { // заголовок страницы
    uint64_t prev_page;
    uint64_t end_page;
};

struct file_table_metadata {  //метаданные таблицы
    uint64_t name;
    uint64_t attr;
    uint64_t size;
    uint64_t last_page;
};

struct file_header { // метаданные файла
    uint64_t key;
    uint64_t page_metadata;
    uint64_t free_page;
};

struct file_table_attribute { // информация о колонках таблиц
    uint64_t name;
    uint8_t type_data;
    uint64_t prev_attr;
};

struct file_string_pool_elem { // пулл строк определенного размера
    uint64_t size;
    uint64_t last_page_link;
//    uint64_t prev_elem;  Вроде не нужно
};

struct file_free_page {
    uint64_t page_link;
    uint64_t prev_page;
};

