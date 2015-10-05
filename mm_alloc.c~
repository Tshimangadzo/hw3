#include "mm_alloc.h"
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#define ASSERT     assert
#define align4(x)  (((((x)-1)>>2)<<2)+4)
                                                    
s_block_ptr base = NULL;            
s_block_ptr last;       
s_block_ptr find_block(size_t size)    
{  
  s_block_ptr bas = base;
  for (; bas != NULL; bas = bas->next)            
   {
      last = bas;
      if (bas->free && bas->size >= size)         
          return bas;                           
   }
  return NULL;                                
}
s_block_ptr extend_heap(size_t s)                
{    
    s_block_ptr new_block;                    
    new_block = (s_block_ptr)sbrk(0); 
      int sb = (int)sbrk(s + S_BLOCK_SIZE);                         
    if (sb < 0)      
       {                                                       
        return (NULL);
      }                                                         
    new_block->size = s;                                  
    new_block->ptr  = new_block->data;                       
    new_block->free = 1;                                 
     
    if (base == NULL)                     
    {
        new_block->prev = NULL; 
        base = new_block;                         
    }
    else 
    {
        ASSERT(last->next == NULL);               
        new_block->next = NULL;                    
        new_block->prev = last;                    
        last->next = new_block;                   
    }
    printf("%x %x %d\n", new_block , new_block->ptr, new_block->size); 
    return new_block;                             
}

void split_block(s_block_ptr p, size_t new_size)
{
    s_block_ptr new_block = NULL;              
    if (p->size >= new_size + S_BLOCK_SIZE + 4)
    {
        p->size = new_size;  
        new_block = (s_block_ptr)(p->data + new_size);
        new_block->size = p->size - new_size - S_BLOCK_SIZE;
        new_block->ptr = new_block->data;
        new_block->free = 1;
        new_block->next = p->next;
        new_block->prev = p;
        if (p->next)
            p->next->prev = new_block;
        p->next = new_block;
    }

}

s_block_ptr fusion_block(s_block_ptr block)
{
    ASSERT(block->free == 1); 
    if (block->next && block->next->free) 
    {
      block->size = block->size + BLOCK_SIZE + block->next->size;
      if (block->next->next)
        block->next->next->prev = block;
      block->next = block->next->next; 		
    }

    return (block);
}

s_block_ptr get_block(void *p)
{
    char *tmp;
    tmp = (char*)p;
    return (s_block_ptr)(tmp - BLOCK_SIZE);
}

static int is_valid_block_addr (void *p)
{
   s_block_ptr block = get_block(p);
   if (base)
   {
     if(p > base && p < sbrk(0))
	return block->ptr == p;
   }

   return 0;
}

static void copy_block(s_block_ptr src, s_block_ptr dst)
{
  int *sdata, *ddata; 
  size_t i;
  sdata = src->ptr;
  ddata = dst->ptr;
  for (i = 0; i * 4 < src->size && i * 4 < dst->size; i++)
    ddata[i] = sdata[i];
}

void* mm_malloc(size_t size)
{
#ifdef MM_USE_STUBS
    return calloc(1, size);
#else
    size_t s = align4(size);    
    s_block_ptr block;
    if (base == NULL) 
    {
       block = extend_heap(s);
       if (block == NULL)
          return NULL;
       base = block;
    }
    else 
    {
       block = find_block(s);
       if (block == NULL)
        {
          block = extend_heap(s);
          if (block == NULL)
            return NULL;
        }
       else
        {
          if (block->size - s >= S_BLOCK_SIZE + 4)
            split_block(block, s);
        }
    }
        
    block->free = 0;
    return block->ptr;
#endif
}

void* mm_realloc(void* ptr, size_t size)
{
#ifdef MM_USE_STUBS
    return realloc(ptr, size);
#else
    size_t s;
    void *newp;
    s_block_ptr block, new;
    if (ptr == NULL)
        return mm_malloc(size);
    
    if ((block=get_block(ptr)) != NULL)
    {
        s = align4(size);
        if (block->size >= s)
         {
           if (block->size - s >= (BLOCK_SIZE + 4))
             split_block(block, s);
         }
    }
    else 
    {
        if (block->next && block->next->free && (block->size + BLOCK_SIZE + block->next->size) >= s)
        {
           fusion_block(block);
           if (block->size - s >= BLOCK_SIZE + 4)
             split_block(block, s);
        }
        else
        {
           newp = mm_malloc(s);
           if (newp == NULL)
               return NULL;
           new = get_block(newp);
           copy_block(block, new);
           mm_free(block);
           return (newp);  
        }
        return (block);
    } 
    return (NULL);
#endif
}
void mm_free(void* ptr)
{
#ifdef MM_USE_STUBS
    free(ptr);
#else
    s_block_ptr block;
    if ((block=get_block(ptr)) != NULL)
    {
      block->free = 1;  
       if(block->prev && block->prev->free)
         fusion_block(block->prev);
       if(block->next)
         fusion_block(block);
       else
       {
 if (block->prev == NULL)
{
  base = NULL;
}
  else
{        
 block->prev->next = NULL;
}   
          brk(block);
       }
    }
    
#endif
}
