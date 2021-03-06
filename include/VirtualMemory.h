
#ifndef VirtualMemory_h__
#define VirtualMemory_h__

#include <def.h>
#include <AssociatedMemoryManager.h>

#if defined(CODE32)
#include <Descriptor.h>
	class PTE;
	class PDE;
	typedef AssociatedMemoryManager<SegmentDescriptor,1> SegManager;
	typedef AssociatedMemoryManager<PTE,1>	PTEManager;
#endif

#if defined(CODE32) ||defined(CODE64)
#pragma pack(push,1)
//just for 32-bit paging,for others must define additional structures
struct PageAttributes{
	enum{
		PWT_ALWAYS_UPDATE=1,
		PWT_CACHE_UPDATE=0,
		PCD_CACHE_ENABLE=0,
		PCD_CACHE_DISABLE=1,
		P_PRESENT=1,
		P_NON_PRESENT=0,
		RW_WRITE_ACCESS=1,
		RW_READONLY=0,
		US_ALL=1,
		US_SYSTEM_ONLY=0,
		A_ACCESSED=1,
		A_NOT_ACCESSED=0,
		D_WRITTEN_EVER=1,
		D_NOT_WRITTEN=0,
		G_GLOBAL=1,
		G_LOCAL=0,
		PAT_SUPPORTED=1,
		PAT_UNSUPPORTED=0
	};
};
struct CR3{

	CR3(int base,int pwt/*=PageAttributes::PWT_ALWAYS_UPDATE*/,
			int pcd=PageAttributes::PCD_CACHE_DISABLE);
	AS_MACRO CR3(u32_t cr3);
	int R0:3; //reserved
	int PWT:1;
	int PCD:1;
	int R1:7;
	int PDE_BASE:20;
};

struct PDE{
	PDE(int base,char pwt /*=PageAttributes::PWT_ALWAYS_UPDATE*/,char pcd=PageAttributes::PCD_CACHE_DISABLE,
			char rw=PageAttributes::RW_WRITE_ACCESS,char us=PageAttributes::US_ALL,char present=PageAttributes::P_PRESENT,
			char a=PageAttributes::A_NOT_ACCESSED);
	AS_MACRO PDE(u32_t pde);
	int P:1;//present
	int RW:1;//read or write
	int US:1;//all,or only system
	int PWT:1;
	int PCD:1;
	int A:1;
	int R0:1;
	int R1:1;
	int R2:4;
	int PTE_BASE:20;
};
struct PTE{
	PTE(int base,char pwt/*=PageAttributes::PWT_ALWAYS_UPDATE*/,char pcd=PageAttributes::PCD_CACHE_DISABLE,
			char rw=PageAttributes::RW_WRITE_ACCESS,char us=PageAttributes::US_ALL,char global=PageAttributes::G_LOCAL,
			char pat=PageAttributes::PAT_UNSUPPORTED,char dirty=PageAttributes::D_WRITTEN_EVER,
			char present=PageAttributes::P_PRESENT,
			char a=PageAttributes::A_NOT_ACCESSED);
	AS_MACRO PTE(u32_t pte);
	int P:1;//present
	int RW:1;//read or write
	int US:1;//all,or only system
	int PWT:1;
	int PCD:1;
	int A:1;
	int D:1;
	int PAT:1;
	int G:1;
	int R0:3;
	int PAGE_BASE:20;

};

struct LinearAddress{//可以作为int直接使用
	int PAGE_OFFSET:12;
	int PTE_INDEX:10;
	int PDE_INDEX:10;
};

#pragma pack(pop)


class PDEManager:public AssociatedMemoryManager<PDE,1>{
public:
	typedef AssociatedMemoryManager<PDE,1> Father;
public:
	PDEManager();
	PDEManager(size_t nstart,size_t tstart,size_t ptemStart,size_t len,
			bool doinit=true,
			int *usedList=NULL,size_t usedLen=0);
	~PDEManager();
	/**
	 * return index
	 */
	int	prepareVisitPhysical(u32_t phyaddr,size_t size,SegManager &segman);
protected:
	union{
		size_t ptemstart;
		PTEManager **ptemans;
	};

};

class VirtualManager{
public:
	AS_MACRO static u32_t getIndex(u32_t phyaddr,u32_t reg);
	AS_MACRO static u32_t getHighBase(u32_t phyaddr);
	AS_MACRO static CR3 getCr3();
//	AS_MACRO static int igetCr3();
	AS_MACRO static void writeCr3(const CR3& cr3);
	AS_MACRO static void enablePaging();
public:

protected:
	PDEManager	pdeman;

};
#endif //CODE32 || CODE64




#endif //VirtualMemory_h__
