#ifndef __Descriptor__
#define __Descriptor__
/**
*提供了对intel x86的8字节描述符的结构化描述
*
*/
class Descriptor{
public:
    Descriptor();
    ~Descriptor();
protected:

private:

};
//多于一个字节的用数组类型，其他用直接量
//10个属性 = 8直接属性 + 2指针属性
class SegmentDescriptor:public Descriptor{
public:
//conforming can let low privilage keeps
    const static int TYPE_U_STACK,TYPE_U_DATA,TYPE_U_CODE_NONCONFORMING,TYPE_U_CODE_CONFORMING;
    const static int TYPE_S_UNUSED_1,TYPE_S_ZERO_UNUSED_2,TYPE_S_ZERO_UNUSED_3,TYPE_S_ZERO_UNUSED_4,TYPE_S_TSS_16_AVL,TYPE_S_TSS_16_BUSY,TYPE_S_TSS_32_AVL,TYPE_S_TSS_32_BUSY,TYPE_S_CALLGATE_16,TYPE_S_CALLGATE_32,TYPE_S_TASKGATE,TYPE_S_INTGATE_16,TYPE_S_TRAPGATE_16,TYPE_S_INTGATE_32,TYPE_S_TRAPGATE_32;
    const static int DPL_0,DPL_1,DPL_2,DPL_3;
public:
    char   BaseAddr[4];//4 bytes

    union{
        char Granularity;//if set,the unit is 4KB
        char G;
    };
    union{ //always be 1 for 32-bit code and data
        char DefaultOperationSize;//0=16 1=32
        char D;//for Code Segment,it is
        
        char LittleUpperBound;//for stack(expand down),it is 0xffff(64K),else if 0xffffffff(4G)
        char B;
    };
    union{
        char Is64Segment;//only for IA-32e
        char L;
    }; 
    union{
        char Available;
        char AVL;
    };
    char Limit[3];
    union{
        char Present;
        char P;
    };
    char DPL;
    union{
        char System;//0=system
        char S;
    };
    union{
        char Type;//4位
        char DEWA;//=0,data Normal/Expand-down ;Read Only/Writeable;Accessed
        char CCRA;//=1,code /Conforming ; Execute Only/Readable ; Accessed
    };
    
    SegmentDescriptor(char* baseaddr=0,int limit=0,char type=TYPE_U_DATA,char dpl=DPL_0);
    ~SegmentDescriptor();
    void writeToMemory(int seg,char* addr);//0x10000 means use current seg
    /**
    *相同返回1，不同返回0
    */
    int equals(SegmentDescriptor &sd2);
    static void fromMemory(SegmentDescriptor *sd,int seg,char* addr);
    
    
    
};

#endif