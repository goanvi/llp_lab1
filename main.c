//
// Created by gav66 on 9/20/2023.
//
/* Что использовать char или unsigned char?
 * TODO: Возможно стоит передавать структуру бд через параметры, чтобы возвращать коды ошибок
 * TODO: Прокидывать ошибки, которые не должен видеть пользователь, остальные выводить в консоль
 */

#include <stdio.h>
#include "main.h"
#include <stdlib.h>
#include <memory.h>

/*
 * Возвращает указатель в файле на начало записанной страницы, иначе -1
 * TODO: Что будет если long не хватит, чтобы сместиться от начала файла?
 */
long write_page(FILE *file, uint64_t addr, char *buffer) {
    if (fseek(file, addr, SEEK_SET)) return -1;
    long pointer = ftell(file);
    if (fwrite(buffer, sizeof(char), PAGE_SIZE, file) != PAGE_SIZE) return -1;
    else return pointer;
}


/*
 * TODO: Придумать, что будет возвращать функция
 * TODO: Что будет если long не хватит, чтобы сместиться от начала файла?
 */
long read_page(FILE *file, uint64_t addr, char *buffer) {
    if (fseek(file, addr, SEEK_SET)) return -1;
    if (fread(buffer, sizeof(char), PAGE_SIZE, file) != PAGE_SIZE) return -1;
    else return 0;
}

/*
 * Открывает файл и считывает схему бд
 * TODO: Подумать как правильно обрабатывать ошибки при взаимодействии с файлом
 */
struct database* open_db(char *file_name) {
    FILE *file = fopen(file_name, "r+");
    char header_page[PAGE_SIZE];
    if (!file || read_page(file, 0, header_page) < 0) {
        printf("Не удалось прочитать данные из файла\n");
        return NULL;
    }
}

/*
 * Создает файл для записи и схему для бд
 * TODO: Подумать как обрабатывать существование файла, дать ли возможность перезаписывать файл
 * TODO: Подумать над режимами открытия файла
 * TODO: Подумать как правильно обрабатывать ошибки при взаимодействии с файлом
 */
struct database *create_db(char *file_name) {
//    FILE *file = fopen(file_name, "r+");
//    if (file) {
//        printf("Такой файл уже существует\n");
//        return NULL;
//    }
    FILE *file = fopen(file_name, "w+");//
    char page[PAGE_SIZE];
    if (write_page(file, 0, page) < 0) {
        printf("Не удалось записать данные в файл\n");
        return NULL;
    }
    struct database *db = malloc(sizeof(struct database));
    struct header *header = malloc(sizeof(struct file_header));
    *header = (struct header) {KEY, 0, 0, NULL};
    db->file = file;
    db->header = header;
    return db;
}

/*
 * Сохраняет схему бд в указанный файл
 * TODO: Подумать как правильно обрабатывать ошибки при взаимодействии с файлом
 */
int save_db(struct database *db) {
    char header_page[PAGE_SIZE];
    if (!db->file || read_page(db->file, 0, header_page) < 0) {
        printf("Не удалось прочитать данные из файла\n");
        return -1;
    }
    long offset = sizeof(struct file_page_header);
    struct file_header file_header = {db->header->key, db->header->page_metadata, db->header->free_pages};
    memcpy(header_page + offset, &file_header, sizeof(struct file_header));
    offset += sizeof(struct file_header);
    struct string_pool_elem *elem = db->header->string_pool;
    while (elem != NULL) {
        struct file_string_pool_elem el = {elem->size, elem->last_page_link}; //Просто массив из пула строк
        memcpy(header_page + offset, &el, sizeof(struct string_pool_elem));
        offset += sizeof(struct string_pool_elem);
        elem = elem->prev_elem;
    }
    struct file_page_header page_header = {0, offset};
    memcpy(header_page, &page_header, sizeof(struct file_page_header));
    if (write_page(db->file, 0, header_page) < 0) {
        printf("Не удалось записать данные в файл\n");
        return -1;
    }
    return 0;
}

int main(int arg, char **args) {
    return 0;
}
