#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <assert.h>
#include <conio.h>
using namespace std;

typedef float vec3_t[3];

struct bone_t {
	int id;
	char name[64];
};

struct bone_pos {
	vec3_t position;
	vec3_t rotation;
};

bool Load_Smd(const char *szFilename)
{
	FILE *fp = fopen(szFilename, "r");
	if (!fp) {
		printf("Error open file!\n");
		return false;
	}

	int version = 0;
	vector<bone_t> bones;
	vector<vector<bone_pos>> frames;
	char line[1024];
	while (!feof(fp) && !ferror(fp)) {
		fgets(line, sizeof(line), fp);
		if (!strncmp(line, "version", 7)) {
			if (sscanf(line, "version %d", &line) != 1) {
				printf("Invalid smd version! : %d\n", line);
				return false;
			}
			continue;
		}

		//Read skeleton
		if (!strncmp(line, "node", 4)) {
			while (!feof(fp) && !ferror(fp)) {
				fgets(line, sizeof(line), fp);
				if (!strncmp(line, "end", 3))
					break;

				int index = 0;
				bone_t bone;
				if (sscanf(line, "%d %s %d", &index, &bone.name, &bone.id) == 3) {
					bones.push_back(bone);
				}
			}
		}

		if (!strncmp(line, "skeleton", 8)) {
			while (!feof(fp) && !ferror(fp)) {
				fgets(line, sizeof(line), fp);
				if (strstr(line, "end"))
					break;

				if (strstr(line, "time")) {
					int frameidx = 0;
					if (sscanf(line, "time %d", &frameidx) == 1) {
						printf("Readed Frame: %d\n", frameidx);
						int boneindex = 0;
						bone_pos bonedata;
						int num_of_bones = bones.size();
						assert(num_of_bones);
						vector<bone_pos> frame(num_of_bones);
						for (int i = 0; i < num_of_bones; i++) {
							fgets(line, sizeof(line), fp);
							if (sscanf(line, "%d %f %f %f %f %f %f", &boneindex, &bonedata.position[0], &bonedata.position[1], &bonedata.position[2], &bonedata.rotation[0], &bonedata.rotation[1], &bonedata.position[2]) != 7) {
								printf("Error reading in frame %d\n", frameidx);
							}
							frame.push_back(bonedata);
						}
						frames.push_back(frame);
					}
				}
			}
		}
	}

	//Display bones
	for (int k = 0; k < bones.size(); k++) {
		printf("index: %5.d  name: %-64s  id: %5.d\n", k, bones[k].name, bones[k].id);
	}

	//Display frames
	printf("Frames %d\n", frames.size() - 1);
	for (int i = 0; i < frames.size(); i++) {
		printf("Frame %d {\n", i);

		int j = 0;
		for (auto oneframe : frames[i]) {
			printf("\t%d ( %f %f %f ) ( %f %f %f )\n", j,
				oneframe.position[0], oneframe.position[1], oneframe.position[2],
				oneframe.rotation[1], oneframe.rotation[1], oneframe.rotation[2]);
			_getch();
			j++;
		}
		printf("}\n\n");
	}
	fclose(fp);
	return true;
}

int main()
{
	if (!Load_Smd("9crouchwalk_idle.smd")) {
		printf("Error open file!\n");
		return -1;
	}
	return 0;
}