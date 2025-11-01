/****************************************************************
 * @file    fdt.c
 * @brief   Implementation of @ref fdt.c
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#include <drivers/fdt/fdt.h>
#include <mm/kalloc.h>
#include <string.h>

#define FDT_MAGIC        0xD00DFEEDUL
#define FDT_BEGIN_NODE   0x00000001UL /**< Node begin token */
#define FDT_END_NODE     0x00000002UL /**< Node end token */
#define FDT_PROP         0x00000003UL
#define FDT_NOP          0x00000004UL
#define FDT_END          0x00000009UL

#define FDT_MAX_CHILDREN 32UL /**< The max number of children a node can have. Clamped at 32 for now */

/**
 * @brief Flattened Device Tree binary node in-memory structure
 */
typedef struct fdt_node
{
    struct fdt_node* parent;                     /* The parent node of this node*/
    struct fdt_node* children[FDT_MAX_CHILDREN]; /**< The child nodes of this node*/
    const char*      name;                       /**< The name of this node */
} fdt_node_t;

/**
 * @brief Flattened Device Tree property structure
 *
 */
typedef struct
{
    uint32_t len;     /**< The length of this property, in bytes */
    uint32_t nameoff; /**< Offset into the strings table for this property */
} fdt_prop_t;

static struct fdt_header* fdt_info_block = NULL;
static fdt_node_t*        root_node      = NULL;

/**
 * @brief Align a value to the next Device Tree word address
 *
 * @param[in] value The value to align
 *
 * @return The aligned value
 */
static inline size_t fdt_AlignToNextWord(const size_t value)
{
    return value + ((sizeof(uint32_t) - value) % sizeof(uint32_t));
}

/**
 * @brief Inserts a child node into a node's children list
 *
 * @param[in] node  The node to insert the child into
 * @param[in] child The child node to insert
 *
 * @return true   The child has been inserted
 * @return false  Child insertion failed due to many children (as specified by @ref FDT_MAX_CHILDREN)
 */
static inline bool fdt_InsertChild(fdt_node_t* const node, const fdt_node_t* const child)
{
    size_t child_index = 0ULL;

    /* Walk the child array until we find a free slot*/
    while (child_index < FDT_MAX_CHILDREN)
    {
        if (node->children[child_index] == NULL)
        {
            node->children[child_index] = child;
            return true;
        }

        child_index++;
    }

    /* If we get here, the insertion failed, so return false */
    return false;
}

/**
 * @brief Read a word from the Device Tree structure
 *
 * @param[in] ptr Pointer to the word to read
 *
 * @return The word at the pointer in the correct CPU byte order
 */
static inline uint32_t fdt_Read32(const uint8_t* ptr)
{
#ifdef SRV_SYSTEM_BIG_ENDIAN
#error "Big Endian Systems are unsupported! You probably ate a SPARC!"
#else
    uint32_t word;
    (void)memcpy((void*)&word, (const void*)ptr, sizeof(uint32_t));
    return __builtin_bswap32(word);
#endif
}

static void fdt_ParseTree(void)
{
#ifdef SRV_SYSTEM_BIG_ENDIAN
    const uint8_t* dt_start_ptr = (uint8_t*)fdt_info_block + fdt_info_block->off_dt_struct;
#else
    const uint8_t* dt_start_ptr = (uint8_t*)fdt_info_block + __builtin_bswap32(fdt_info_block->off_dt_struct);
#endif
    size_t      curr_offset = 0ULL;
    fdt_node_t* curr_node   = NULL;

    while (curr_offset < fdt_info_block->size_dt_struct)
    {
        const uint32_t token  = fdt_Read32(&dt_start_ptr[curr_offset]);
        curr_offset          += sizeof(uint32_t);

        if (token == FDT_BEGIN_NODE)
        {
            /* Get the length of the node's name */
            const char*  node_name      = (const char*)&dt_start_ptr[curr_offset];
            const size_t node_name_len  = strlen(node_name);
            curr_offset                += fdt_AlignToNextWord(node_name_len);

            fdt_node_t* new_node = (fdt_node_t*)srv_kalloc_EternalAlloc(sizeof(fdt_node_t));
            new_node->name       = node_name;

            /* This is the root node */
            if (node_name_len == 0ULL)
            {
                root_node         = (fdt_node_t*)new_node;
                root_node->parent = NULL;
                curr_node         = root_node;
            }
            else
            {
                new_node->parent = curr_node;
            }

            /* Set a pointer to the node name and insert the child */
            fdt_InsertChild(curr_node, new_node);
            curr_node = new_node;
        }
        else if (token == FDT_END_NODE)
        {
            curr_node = curr_node->parent;
        }
        else if (token == FDT_PROP)
        {
            fdt_prop_t* prop = (fdt_prop_t*)&dt_start_ptr[curr_offset];

            const uint32_t prop_len = fdt_Read32((const uint8_t*)&prop->len);

            //const uint32_t str_offset  = fdt_Read32((const uint8_t*)&dt_start_ptr[curr_offset + sizeof(uint32_t)]);
            curr_offset += sizeof(fdt_prop_t) + fdt_AlignToNextWord(prop_len); /* Take into account the second word of the prop being the name offset */
        }
        else if (token == FDT_NOP)
        {
        }
        else if (token == FDT_END)
        {
            /* Nothing left to do, get out of here! */
            break;
        }
    }
}

bool srv_fdt_Init(void* fdt_ptr)
{
    struct fdt_header* header = (struct fdt_header*)fdt_ptr;

    /* Make sure this is actually a real DTB */
    if (fdt_Read32((uint8_t*)&header->magic) != FDT_MAGIC)
    {
        return false;
    }

    /* Cache a pointer to the FDT header */
    fdt_info_block = fdt_ptr;

    /* Parse the tree */
    fdt_ParseTree();

    return true;
}

size_t srv_fdt_GetMemorySize(void)
{
    return 0ULL;
}
