#define MEM_MAGIC  'c'
#define MEM_SET  _IOW(MEM_MAGIC, 0,  unsigned long)
#define MEM_RESTART  _IO(MEM_MAGIC, 1)
