#include "stdio.h"
#define BITMASK(HiBit, LoBit)    (((0x01 << ((HiBit) + 1 - (LoBit))) - 1) << (LoBit))

#define SIZE        7 //8 26 bits field 26*8 = 208 bits, ie., 7 32 bit field is needed
#define MAX_PAD     6 // every byte may have max 26 bits of data, so max padding
                      // in a byte is 32-26 = 6 bits
#define DATA_SIZE   26// size of each value 26*8 entries = 208 bits

//
int bitArr[SIZE] = {0};

int read(unsigned int addr, int *data){
    int idx = 0, pos = 0, mask = 0, mask1 = 0, mask2 = 0;
    idx = SIZE - ((addr*DATA_SIZE) / 32) - 1;
    pos = ((addr*DATA_SIZE) % 32);

    if(pos < MAX_PAD) { // value is present in a 1 byte bitArr[idx]
        mask =  BITMASK(pos+DATA_SIZE-1, pos);
        *data = (bitArr[idx] & mask) >> (pos);
    }else { // value is spread across 2 bytes bitArr[idx] and bitArr[idx-1]
        mask1 = BITMASK(31, pos);
        mask2 = BITMASK((pos-MAX_PAD-1), 0);
        *data = ((bitArr[idx] & mask1)>>pos) | ((bitArr[idx - 1] & mask2)<<(32-pos));
    }
    return 0;
}

int write(unsigned int addr, int * data){
    int idx = 0, pos = 0, mask = 0, mask1 = 0, mask2 = 0;
    idx = SIZE - ((addr*DATA_SIZE) / 32) - 1;
    pos = ((addr*DATA_SIZE) % 32);

    if(pos < MAX_PAD) { // value is present in a 1 byte bitArr[idx]
        mask = BITMASK(pos+DATA_SIZE, pos);
        bitArr[idx] = (bitArr[idx] & ~mask) | (*data << pos);
    } else { // value is spread across 2 bytes bitArr[idx] and bitArr[idx-1]
        mask1 = BITMASK(31, pos);
        mask2 = BITMASK(pos-MAX_PAD-1,0);
        bitArr[idx] = (bitArr[idx] & ~mask1) | ((*data & BITMASK(31-pos, 0)) << pos);
        bitArr[idx-1] = (bitArr[idx - 1] & ~mask2) | ((*data & BITMASK(DATA_SIZE-1,32-pos)) >> (32-pos));
    }
    return 0;
}

int main(){
    char c;
    unsigned int loc = 0, data = 0, n = 0, i = 0;
    do {
        printf("BitArray Read/write/display/quit(r/w/d/q):");
        scanf(" %c",&c);
        switch(c){
            case 'r':
                printf("enter location(0-7):");
                scanf(" %d",&loc);
                if(loc>7){
                    printf("invalid location %d\n",loc);
                    continue;
                }
                read(loc, &data);
                printf("[%x] : 0x%x\n", loc, data & 0x3ffffff);
                break;
            case 'w':
                printf("enter location(0-7):");
                scanf(" %d",&loc);
                if(loc>7){
                    printf("invalid location %d\n",loc);
                    continue;
                }
                printf("Enter data(range: 0x0-0x3FFFFFF):");
                scanf(" %x",&data);
                if (data > 0x3FFFFFF){
                    printf("data exceeding 26 bits range (0x0-0x3FFFFFF): 0x%x\n",data);
                    continue;
                }
                write(loc,&data);
                read(loc, &data);
                printf("[%x] : 0x%x\n", loc, data);
                break;
            case 'd':
                printf("enter start location(0-7):");
                scanf(" %d",&loc);
                if(loc>7){
                    printf("invalid location %d\n",loc);
                    continue;
                }
                printf("no of entry(range:[1-%d]):",(8-loc));
                scanf(" %d",&n);
                if (n > (8 - loc)){
                    printf("num entry out of range[1-%d]",(8-loc));
                    continue;
                }
                for (i=0;i<n;i++){
                    read(loc+i, &data);
                    printf("[%x] : 0x%x\n", loc+i, data);
                }
                break;
            case 'q':
                break; 
        default:
            printf("invalid option\n");
            return 0;
        }
    }while(c != 'q');
    return 0;
}
