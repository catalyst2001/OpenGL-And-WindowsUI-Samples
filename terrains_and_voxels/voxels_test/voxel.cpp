#include "voxel.h"

int chunk_alloc_voxels(chunk *pchunk, int width, int height)
{
	//����������� �������� ��������� ������ ����� �� 250 (� ������� � 5 ����)
	//��� ��� ���������� ������� � ��� ����� �� 1 �����, ������ ��������� �������� �� 0 �� 255, ����� ���������� ������������ � ������ ����� ��������
	//���������, �� ������ �� ������� � �� ������ 250 ������ � ������ �����
	if (width < 1 || width > 250 || height < 1 || height > 250)
		return CHUNK_ERROR_BAD_PARAMETER; //���� �� ������� �� �������, ������� ����������� ������� CHUNK_ERROR_BAD_PARAMETER

	//����������� ��������� ����� ������ ��� ������������ �����
	//������ ��� �� �������:  (WIDTH * HEIGHT * WIDTH) * VOXEL_STRUCT_SIZE
	int requiried_memory = voxel_compute_memory(width, height);
	pchunk->voxels = (voxel ***)malloc(requiried_memory);

	//���������, ������ �� ������� ������������� ��� ������ ������
	//��� �� ������� ��� ���� ��������
	if (!pchunk->voxels)
		return CHUNK_ALLOC_FAILURE; //���� ��� ���� �������, �� ��������� ������� � ������� CHUNK_ALLOC_FAILURE

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
