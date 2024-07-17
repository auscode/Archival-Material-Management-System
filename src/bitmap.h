#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Enums
enum MaterialType
{
    BOOK,
    JOURNAL,
    NEWSPAPER
};
enum BookType
{
    NOVEL,
    BIOGRAPHY,
    HISTORY
};
enum JournalType
{
    SCIENCE,
    LITERATURE,
    ART
};
enum NewspaperType
{
    DAILY,
    WEEKLY,
    MONTHLY
};

// Unions
union MaterialDetails
{
    struct
    {
        int pages;
        char author[50];
        enum BookType type;
    } book;
    struct
    {
        int issue;
        char publisher[50];
        enum JournalType type;
    } journal;
    struct
    {
        char editor[50];
        enum NewspaperType type;
    } newspaper;
};

// Structs
struct Material
{
    char title[50];
    enum MaterialType type;
    union MaterialDetails details;
};

struct Archive
{
    struct Material materials[100];
    int count;
};


// Functions

int addMaterial(struct Archive *archive, struct Material material);

struct Material *findMaterial(struct Archive *archive, char *title);

void filterMaterials(struct Archive *archive, enum MaterialType type);

int updateMaterial(struct Archive *archive, char *title, union MaterialDetails details);

void removeMaterial(struct Archive *archive, char *title);

struct Material *filterMaterialsByAuthor(struct Archive *archive, char *author);