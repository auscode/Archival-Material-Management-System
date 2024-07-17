#include <cxxtest/TestSuite.h>
#include "../src/bitmap.h"

class SampleTestSuite : public CxxTest::TestSuite
{
public:
    void testAddMaterialEmptyTitle()
    {
        struct Archive archive = {0};
        struct Material material = {"", BOOK, {.book = {200, "Author Name", NOVEL}}};
        TS_ASSERT_EQUALS(addMaterial(&archive, material), 0);
        TS_ASSERT_EQUALS(archive.count, 1);
    }

    void testAddMaterialEmptyAuthor()
    {
        struct Archive archive = {0};
        struct Material material = {"Book Title", BOOK, {.book = {200, "", NOVEL}}};
        TS_ASSERT_EQUALS(addMaterial(&archive, material), 0);
        TS_ASSERT_EQUALS(archive.count, 1);
    }

    void testAddMaterialInvalidType()
    {
        struct Archive archive = {0};
        struct Material material = {"Material Title", (enum MaterialType)3, {.book = {200, "Author Name", NOVEL}}};
        TS_ASSERT_EQUALS(addMaterial(&archive, material), 0);
        TS_ASSERT_EQUALS(archive.count, 1);
    }
    /////////////////////////////////////////////////////////////////////////////
    void testFindMaterialFirstMaterial()
    {
        // Create an archive with two Materials
        struct Material material1 = {"Pride and Prejudice", BOOK, {.book = {279, "Jane Austen", NOVEL}}};
        struct Material material2 = {"The Great Gatsby", BOOK, {.book = {180, "F. Scott Fitzgerald", NOVEL}}};
        struct Archive archive = {{material1, material2}, 2};

        // Search for the first Material in the archive
        char title[] = "Pride and Prejudice";
        struct Material *result = findMaterial(&archive, title);

        // Ensure the function returns a pointer to the first Material
        TS_ASSERT(result == &archive.materials[0]);
    }

    void testFindMaterialLastMaterial()
    {
        // Create an archive with two Materials
        struct Material material1 = {"Pride and Prejudice", BOOK, {.book = {279, "Jane Austen", NOVEL}}};
        struct Material material2 = {"The Great Gatsby", BOOK, {.book = {180, "F. Scott Fitzgerald", NOVEL}}};
        struct Archive archive = {{material1, material2}, 2};

        // Search for the last Material in the archive
        char title[] = "The Great Gatsby";
        struct Material *result = findMaterial(&archive, title);

        // Ensure the function returns a pointer to the last Material
        TS_ASSERT(result == &archive.materials[1]);
    }

    void testFindMaterialCaseSensitive()
    {
        // Create an archive with one Material
        struct Material material = {"To Kill a Mockingbird", BOOK, {.book = {281, "Harper Lee", NOVEL}}};
        struct Archive archive = {{material}, 1};

        // Search for a Material using a different case
        char title[] = "to kill a mockingbird";
        struct Material *result = findMaterial(&archive, title);

        // Ensure the function returns NULL
        TS_ASSERT(result == NULL);
    }
    ////////////////////////////////////////////////////////////////
    void testFilterMaterialsEmptyArchive()
    {
        struct Archive archive;
        archive.count = 0;
        filterMaterials(&archive, BOOK);
        TS_ASSERT_EQUALS(archive.count, 0);
        filterMaterials(&archive, JOURNAL);
        TS_ASSERT_EQUALS(archive.count, 0);
        filterMaterials(&archive, NEWSPAPER);
        TS_ASSERT_EQUALS(archive.count, 0);
    }
    void testFilterMaterialsNoMatch()
    {
        struct Archive archive;
        archive.count = 1;
        strcpy(archive.materials[0].title, "A Book");
        archive.materials[0].type = BOOK;
        archive.materials[0].details.book.pages = 100;
        strcpy(archive.materials[0].details.book.author, "An Author");
        archive.materials[0].details.book.type = NOVEL;
        filterMaterials(&archive, JOURNAL);
        TS_ASSERT_EQUALS(archive.count, 1);
    }
    void testFilterMaterialsOneMatch2()
    {
        struct Archive archive;
        archive.count = 1;
        strcpy(archive.materials[0].title, "A Book");
        archive.materials[0].type = BOOK;
        archive.materials[0].details.book.pages = 100;
        strcpy(archive.materials[0].details.book.author, "An Author");
        archive.materials[0].details.book.type = NOVEL;
        filterMaterials(&archive, BOOK);
        TS_ASSERT_EQUALS(archive.count, 1);
        TS_ASSERT_EQUALS(archive.materials[0].type, BOOK);
        TS_ASSERT_EQUALS(archive.materials[0].details.book.pages, 100);
        TS_ASSERT_EQUALS(std::strcmp(archive.materials[0].details.book.author, "An Author"), 0);
        TS_ASSERT_EQUALS(archive.materials[0].details.book.type, NOVEL);
    }
    ////////////////////////////////////////////////////////////////////

    void testUpdateMaterialBookType5()
    {
        struct Archive archive;
        archive.count = 0;
        struct Material material;
        strcpy(material.title, "The Great Gatsby");
        material.type = BOOK;
        material.details.book.pages = 180;
        strcpy(material.details.book.author, "F. Scott Fitzgerald");
        material.details.book.type = NOVEL;
        addMaterial(&archive, material);
        union MaterialDetails details;
        strcpy(details.book.author, "Francis Scott Fitzgerald");
        details.book.pages = 218;
        details.book.type = BIOGRAPHY;
        char title[] = "The Great Gatsby";
        int result = updateMaterial(&archive, title, details);
        TS_ASSERT_EQUALS(result, 0);
        TS_TRACE("testUpdateMaterialInvalidBookType");
    }
    void testUpdateMaterialMissingMaterial()
    {
        struct Archive archive;
        archive.count = 0;
        union MaterialDetails details;
        char title[] = "The Great Gatsby";
        int result = updateMaterial(&archive, title, details);
        TS_ASSERT_EQUALS(result, -2);
        TS_TRACE("testUpdateMaterialMissingMaterial");
    }

    void testUpdateMaterialWithInvalidArchive()
    {
        struct Archive *archive = NULL;
        union MaterialDetails details;
        char title[] = "The Alchemist";
        TS_ASSERT_EQUALS(updateMaterial(archive, title, details), -1);
    }

    void testUpdateMaterialWithInvalidTitle()
    {
        struct Archive archive;
        union MaterialDetails details;
        TS_ASSERT_EQUALS(updateMaterial(&archive, NULL, details), -1);
    }
    //////////////////////////////////////////////////////////////////////////////
    void testRemoveMaterialSuccess9()
    {
        // Initialize an archive with some materials
        struct Archive archive = {
            {{"The Great Gatsby", BOOK, {.book = {180, "F. Scott Fitzgerald", NOVEL}}},
             {"To Kill a Mockingbird", BOOK, {.book = {281, "Harper Lee", NOVEL}}},
             {"The Sun Also Rises", BOOK, {.book = {251, "Ernest Hemingway", NOVEL}}}},
            3};
        // Remove one of the materials
        char title[] = "To Kill a Mockingbird";
        removeMaterial(&archive, title);

        // Check that the material was removed and the count is decremented
        TS_ASSERT_EQUALS(archive.count, 2);
        TS_ASSERT_EQUALS(strcmp(archive.materials[0].title, "The Great Gatsby"), 0);
        TS_ASSERT_EQUALS(strcmp(archive.materials[1].title, "The Sun Also Rises"), 0);
        TS_TRACE("testRemoveMaterialSuccess");
    }
    void testRemoveMaterialNotFound()
    {
        // Initialize an archive with some materials
        struct Archive archive = {
            {{"The Great Gatsby", BOOK, {.book = {180, "F. Scott Fitzgerald", NOVEL}}},
             {"To Kill a Mockingbird", BOOK, {.book = {281, "Harper Lee", NOVEL}}},
             {"The Sun Also Rises", BOOK, {.book = {251, "Ernest Hemingway", NOVEL}}}},
            3};
        // Try to remove a material that does not exist in the archive
        char title[] = "The Catcher in the Rye";
        removeMaterial(&archive, title);

        // Check that the archive is unchanged
        TS_ASSERT_EQUALS(archive.count, 3);
        TS_ASSERT_EQUALS(strcmp(archive.materials[0].title, "The Great Gatsby"), 0);
        TS_ASSERT_EQUALS(strcmp(archive.materials[1].title, "To Kill a Mockingbird"), 0);
        TS_ASSERT_EQUALS(strcmp(archive.materials[2].title, "The Sun Also Rises"), 0);
        TS_TRACE("testRemoveMaterialNotFound");
    }

    void testRemoveMaterialSuccess()
    {
        // Initialize an archive with some materials
        struct Archive archive = {
            {{"The Great Gatsby", BOOK, {.book = {180, "F. Scott Fitzgerald", NOVEL}}},
             {"To Kill a Mockingbird", BOOK, {.book = {281, "Harper Lee", NOVEL}}},
             {"The Sun Also Rises", BOOK, {.book = {251, "Ernest Hemingway", NOVEL}}}},
            3};
        // Remove one of the materials
        char title[] = "To Kill a Mockingbird";
        removeMaterial(&archive, title);

        // Check that the material was removed and the count is decremented
        TS_ASSERT_EQUALS(archive.count, 2);
        TS_ASSERT_EQUALS(strcmp(archive.materials[0].title, "The Great Gatsby"), 0);
        TS_ASSERT_EQUALS(strcmp(archive.materials[1].title, "The Sun Also Rises"), 0);
        TS_TRACE("testRemoveMaterialSuccess");
    }

    void testRemoveMaterialLastMaterial()
    {
        // Initialize an archive with one material
        struct Archive archive = {
            {{"The Great Gatsby", BOOK, {.book = {180, "F. Scott Fitzgerald", NOVEL}}}},
            1};
        // Remove the only material in the archive
        char title[] = "The Great Gatsby";
        removeMaterial(&archive, title);

        // Check that the archive is empty
        TS_ASSERT_EQUALS(archive.count, 0);
        TS_TRACE("testRemoveMaterialLastMaterial");
    }
    //////////////////////////////////////////////////////////////////////////////////////////

    void testEmptyArchive()
    {
        struct Archive archive = {0};
        char author[] = "John Doe";
        struct Material *result = filterMaterialsByAuthor(&archive, author);
        TS_ASSERT(result == NULL);
        TS_TRACE("testEmptyArchive");
    }

    void testEmptyAuthor()
    {
        struct Archive archive = {0};
        char author[] = "";
        struct Material *result = filterMaterialsByAuthor(&archive, author);
        TS_ASSERT(result == NULL);
        TS_TRACE("testEmptyAuthor");
    }
    void testFilterMaterialsByAuthor_NoMatch()
    {
        struct Archive archive = {
            .materials = {
                {.title = "Material1",
                 .type = BOOK,
                 {.book = {.pages = 200, .author = "Author1", .type = NOVEL}}},
                {.title = "Material2",
                 .type = JOURNAL,
                 {.journal = {.issue = 1, .publisher = "Publisher1", .type = SCIENCE}}}},
            .count = 3};
        char author[] = "Author2";
        struct Material *result = filterMaterialsByAuthor(&archive, author);
        TS_ASSERT(result == NULL);
    }
};