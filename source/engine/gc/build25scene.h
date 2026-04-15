#ifndef BUILD25SCENE_H
#define BUILD25SCENE_H


void build25Dscene(const u8* roomData, size_t dataSize, u8* neightbors);
void genSolidLUT(uint8_t *LUT);
void computeNeighbors(const uint8_t *tiles, const uint8_t *lut,int width, int offset, uint8_t *neighbors, int count);
#endif
