#include "voxel.h"

int chunk_alloc_voxels(chunk *pchunk, int width, int height)
{
	//ограничиваю диапазон координат внутри чанка до 250 (с запасом в 5 байт)
	//так как трехмерные вектора у нас сжаты до 1 байта, нельзя превышать диапазон от 0 до 255, иначе произойдет переполнение и данные будут искажены
	//проверяем, не меньше ли единицы и не больше 250 ширина и высота чанка
	if (width < 1 || width > 250 || height < 1 || height > 250)
		return CHUNK_ERROR_BAD_PARAMETER; //если мы выходим за пределы, функция завершается ошибкой CHUNK_ERROR_BAD_PARAMETER

	//расчитываем требуемый объем памяти для пространства чанка
	//делаем это по формуле:  (WIDTH * HEIGHT * WIDTH) * VOXEL_STRUCT_SIZE
	int requiried_memory = voxel_compute_memory(width, height);
	pchunk->voxels = (voxel ***)malloc(requiried_memory);

	//проверяем, смогла ли система удволетворить наш запрос памяти
	//или же послала нас куда подальше
	if (!pchunk->voxels)
		return CHUNK_ALLOC_FAILURE; //если все таки послала, то завершаем функцию с ошибкой CHUNK_ALLOC_FAILURE

	//voxel *temp_ptr = ((voxel *)pchunk->voxels + );

	//for (int x = 0; x < width; x++) {
	//	for (int z = 0; z < width; z++) {
	//		for (int y = 0; y < height; y++) {
	//			pchunk->voxels + x
	//		}
	//	}
	//}
	return CHUNK_OK;
}
