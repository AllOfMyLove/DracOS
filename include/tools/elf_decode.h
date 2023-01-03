
#ifndef __ELF_DECODE_H
#define __ELF_DECODE_H

#include "type.h"




#define EI_NIDENT (16)


/* ELF文件头结构 */
typedef struct
{
  u8	e_ident[EI_NIDENT]; /* Magic number and other info */       // 1 byte * 16
  u16	e_type;		    /* Object file type */                  // 2 bytes
  u16	e_machine;	    /* Architecture */
  u32	e_version;	    /* Object file version */               // 4 bytes
  u64	e_entry;	    /* Entry point virtual address */       // 8 bytes
  u64	e_phoff;	    /* Program header table file offset */  // 8 bytes
  u64	e_shoff;	    /* Section header table file offset */      
  u32	e_flags;	    /* Processor-specific flags */
  u16	e_ehsize;	    /* ELF header size in bytes */
  u16	e_phentsize;	/* Program header table entry size */
  u16	e_phnum;	    /* Program header table entry count */
  u16	e_shentsize;	/* Section header table entry size */
  u16	e_shnum;	    /* Section header table entry count */
  u16	e_shstrndx;	    /* Section header string table index */
} Elf64_Ehdr;


#define EI_CLASS	4		/* File class byte index */
#define ELFCLASSNONE	0		/* Invalid class */
#define ELFCLASS32	1		/* 32-bit objects */
#define ELFCLASS64	2		/* 64-bit objects */
#define ELFCLASSNUM	3

#define EI_DATA		5		/* Data encoding byte index */
#define ELFDATANONE	0		/* Invalid data encoding */
#define ELFDATA2LSB	1		/* 2's complement, little endian */
#define ELFDATA2MSB	2		/* 2's complement, big endian */
#define ELFDATANUM	3

/* Legal values for e_type (object file type).  */
#define ET_NONE		0		/* No file type */
#define ET_REL		1		/* Relocatable file */
#define ET_EXEC		2		/* Executable file */
#define ET_DYN		3		/* Shared object file */
#define ET_CORE		4		/* Core file */
#define	ET_NUM		5		/* Number of defined types */
#define ET_LOOS		0xfe00		/* OS-specific range start */
#define ET_HIOS		0xfeff		/* OS-specific range end */
#define ET_LOPROC	0xff00		/* Processor-specific range start */
#define ET_HIPROC	0xffff		/* Processor-specific range end */



typedef struct
{
  u32	p_type;			/* Segment type */               // 4 bytes
  u32	p_flags;		/* Segment flags */
  u64	p_offset;		/* Segment file offset */          // 8 bytes
  u64	p_vaddr;		/* Segment virtual address */    // 8 bytes
  u64	p_paddr;		/* Segment physical address */
  u64	p_filesz;		/* Segment size in file */       // 8 bytes
  u64	p_memsz;		/* Segment size in memory */
  u64	p_align;		/* Segment alignment */
} Elf64_Phdr;


/* Legal values for p_type (segment type).  */
#define	PT_NULL		0		 /* Program header table entry unused */
#define PT_LOAD		1		 /* Loadable program segment */
#define PT_DYNAMIC	2		 /* Dynamic linking information */
#define PT_INTERP	3		 /* Program interpreter */
#define PT_NOTE		4		 /* Auxiliary information */
#define PT_SHLIB	5		 /* Reserved */
#define PT_PHDR		6		 /* Entry for header table itself */

/* Legal values for p_flags (segment flags).  */
#define PF_X		(1 << 0)	/* Segment is executable */
#define PF_W		(1 << 1)	/* Segment is writable */
#define PF_R		(1 << 2)	/* Segment is readable */
#define PF_MASKOS	0x0ff00000	/* OS-specific */
#define PF_MASKPROC	0xf0000000	/* Processor-specific */




void elf_head_check(Elf64_Ehdr *elf);



#endif /* __ELF_DECODE_H */
