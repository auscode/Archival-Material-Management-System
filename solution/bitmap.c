#include "bitmap.h"

/*
This function adds a new material to an archive by checking if there is enough space in the archive and
if the material title does not already exist in the archive. If the material can be added, it is added
to the archive and the function returns 0. If the archive count is greater than and equal to 100 or the material title already exists,
the function returns -1 to indicate failure.
*/
int addMaterial(struct Archive *archive, struct Material material)
{
    if (archive->count >= 100)
    {
        return -1;
    }
    for (int i = 0; i < archive->count; i++)
    {
        if (strcmp(archive->materials[i].title, material.title) == 0)
        {
            return -1;
        }
    }

    // Add material to archive
    archive->materials[archive->count] = material;
    archive->count++;

    return 0; // Return 0 to indicate success
}

/*
This function searches for a Material with a given title in an Archive.
It first checks if the Archive is empty, and if so, returns NULL. If the Archive contains Materials,
the function iterates over each Material in the Archive and compares its title with the desired title.
If it finds a Material with a matching title, it returns a pointer to that Material.
If no Material with the desired title is found, the function returns NULL.
*/
struct Material *findMaterial(struct Archive *archive, char *title)
{
    // Check if the archive is empty
    if (archive->count == 0)
    {
        return NULL;
    }

    // Iterate over each Material in the archive
    for (int i = 0; i < archive->count; i++)
    {
        struct Material *current = &(archive->materials[i]);

        // Compare the titles of the current Material and the desired title
        if (strcmp(current->title, title) == 0)
        {
            // If they match, return a pointer to the current Material
            return current;
        }
    }

    // If the title is not found, return NULL
    return NULL;
}

/*
This function filters materials of a specified type from an Archive struct and stores them in a new Archive struct.
It loops through each material in the original archive and checks if it matches the specified type.
If a match is found, it copies the material to the filtered archive, including type-specific details and common fields.
If no materials of the specified type are found, it returns early. The filteredArchive struct is initialized with a
count of 0 and its materials array is updated as materials are added to it.
*/
void filterMaterials(struct Archive *archive, enum MaterialType type)
{
    // Initialize a new Archive struct to store the filtered materials
    struct Archive filteredArchive;
    filteredArchive.count = 0;

    // Loop through each material in the original archive
    for (int i = 0; i < archive->count; i++)
    {
        struct Material material = archive->materials[i];

        // If the material matches the specified type, add it to the filtered archive
        if (material.type == type)
        {
            // Check the type-specific details to determine which struct fields to copy
            switch (type)
            {
            case BOOK:
                filteredArchive.materials[filteredArchive.count].details.book = material.details.book;
                break;
            case JOURNAL:
                filteredArchive.materials[filteredArchive.count].details.journal = material.details.journal;
                break;
            case NEWSPAPER:
                filteredArchive.materials[filteredArchive.count].details.newspaper = material.details.newspaper;
                break;
            }

            // Copy over the common fields and increase the count of the filtered archive
            strcpy(filteredArchive.materials[filteredArchive.count].title, material.title);
            filteredArchive.materials[filteredArchive.count].type = material.type;
            filteredArchive.count++;
        }
    }

    // If no materials of the specified type were found, return early
    if (filteredArchive.count == 0)
    {
        return;
    }
}

/*
This function updates the details of a material (book, journal, or newspaper) stored in an archive based on its title.
It first checks if the archive and title are not null, then finds the material with the given title.
If the material is found, it updates its details based on its type (book, journal, or newspaper).
If the type of the material is invalid, the function returns an error code. If the update is successful, it returns a success code.
*/
int updateMaterial(struct Archive *archive, char *title, union MaterialDetails details)
{
    // check if archive or title is null
    if (archive == NULL || title == NULL)
    {
        return -1; // return error code
    }

    // find the material with the given title
    struct Material *material = findMaterial(archive, title);
    if (material == NULL)
    {
        return -2; // return error code
    }

    // update the material's details based on its type
    switch (material->type)
    {
    case BOOK:
        if (details.book.type < NOVEL || details.book.type > HISTORY)
        {
            return -3; // return error code for invalid book type
        }
        material->details.book = details.book;
        break;
    case JOURNAL:
        if (details.journal.type < SCIENCE || details.journal.type > ART)
        {
            return -4; // return error code for invalid journal type
        }
        material->details.journal = details.journal;
        break;
    case NEWSPAPER:
        if (details.newspaper.type < DAILY || details.newspaper.type > MONTHLY)
        {
            return -5; // return error code for invalid newspaper type
        }
        material->details.newspaper = details.newspaper;
        break;
    default:
        return -6; // return error code for invalid material type
    }

    return 0; // return success code
}

/*
This function removes a material from an archive by title. It searches for the material with the given title in the archive,
and if found, removes it by shifting all subsequent materials down by one index. The count of materials in the archive is
decremented, and the memory used by the last material in the archive is cleared. If the material is not found, the function does nothing.
*/
void removeMaterial(struct Archive *archive, char *title)
{
    int i, j;
    bool found = false;

    // Find the material with the given title
    for (i = 0; i < archive->count; i++)
    {
        if (strcmp(archive->materials[i].title, title) == 0)
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        // Material not found, do nothing
        return;
    }

    // Remove the material by shifting all subsequent materials down by one index
    for (j = i; j < archive->count - 1; j++)
    {
        archive->materials[j] = archive->materials[j + 1];
    }

    // Decrement the count of materials in the archive
    archive->count--;

    // Clear the memory used by the last material in the archive
    memset(&archive->materials[archive->count], 0, sizeof(struct Material));
}

/*
This function filters materials (books, journals, and newspapers) stored in an archive by the provided author name.
It returns an array of pointers to the materials that match the author name. If the archive or author name is NULL,
or the author name is an empty string, the function returns NULL. The matching materials are stored in a temporary
array of structs, which is then copied into a newly allocated array of structs before being returned.
*/
struct Material *filterMaterialsByAuthor(struct Archive *archive, char *author)
{
    if (archive == NULL || author == NULL)
    {
        // Error: archive or author is NULL
        return NULL;
    }

    if (strlen(author) == 0)
    {
        // Error: author string is empty
        return NULL;
    }
    struct Material matchingMaterials[100];
    int matchingCount = 0;

    for (int i = 0; i < archive->count; i++)
    {
        struct Material *currentMaterial = &archive->materials[i];
        if (currentMaterial->type == BOOK && strcmp(currentMaterial->details.book.author, author) == 0)
        {
            matchingMaterials[matchingCount++] = *currentMaterial;
        }
        else if (currentMaterial->type == JOURNAL && strcmp(currentMaterial->details.journal.publisher, author) == 0)
        {
            matchingMaterials[matchingCount++] = *currentMaterial;
        }
        else if (currentMaterial->type == NEWSPAPER && strcmp(currentMaterial->details.newspaper.editor, author) == 0)
        {
            matchingMaterials[matchingCount++] = *currentMaterial;
        }
    }

    if (matchingCount == 0)
    {
        return NULL;
    }

    struct Material *matchingMaterialsPtr = (struct Material *)malloc(sizeof(struct Material) * matchingCount);

    // struct Material *matchingMaterialsPtr = malloc(sizeof(struct Material) * matchingCount);
    memcpy(matchingMaterialsPtr, matchingMaterials, sizeof(struct Material) * matchingCount);

    return matchingMaterialsPtr;
}
