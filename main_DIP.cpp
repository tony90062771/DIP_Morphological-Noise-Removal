#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "bmp.h"

int R[MaxBMPSizeX][MaxBMPSizeY];
int r[MaxBMPSizeX][MaxBMPSizeY];

void erosion(int input[][MaxBMPSizeY], int output[][MaxBMPSizeY], int width, int height, int kernel_size) {
    int offset = kernel_size / 2;
    for (int j = offset; j < height - offset; j++) {
        for (int i = offset; i < width - offset; i++) {
            int min = INT_MAX;
            for (int y = -offset; y <= offset; y++) {
                for (int x = -offset; x <= offset; x++) {
                    if (input[i + x][j + y] < min) {
                        min = input[i + x][j + y];
                    }
                }
            }
            output[i][j] = min;
        }
    }
}

void dilation(int input[][MaxBMPSizeY], int output[][MaxBMPSizeY], int width, int height, int kernel_size) {
    int offset = kernel_size / 2;
    for (int j = offset; j < height - offset; j++) {
        for (int i = offset; i < width - offset; i++) {
            int max = INT_MIN;
            for (int y = -offset; y <= offset; y++) {
                for (int x = -offset; x <= offset; x++) {
                    if (input[i + x][j + y] > max) {
                        max = input[i + x][j + y];
                    }
                }
            }
            output[i][j] = max;
        }
    }
}

int main() {
    int width, height;
    int kernel_size = 3; // 內核大小，可以根據需要修改

    // 開啟並讀取全彩 bmp 圖檔
    open_bmp("noisy_rectangle.bmp", R, R, R, width, height);

    // 先進行侵蝕
    for (int i = 0; i < 11; i++) {
        erosion(R, r, width, height, kernel_size);
        
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < width; k++) {
                R[j][k] = r[j][k];
            }
        }
    }
	save_bmp("2.bmp", r, r, r);
    // 然後進行膨脹
    for (int i = 0; i < 20; i++) {
        dilation(R, r, width, height, kernel_size);
        
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < width; k++) {
                R[j][k] = r[j][k];
            }
        }
    }

    // 再次進行侵蝕以去除內部黑點
    for (int i = 0; i < 10; i++) {
        erosion(R, r, width, height, kernel_size);
        
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < width; k++) {
                R[j][k] = r[j][k];
            }
        }
    }

    // 將結果保存到新文件
    save_bmp("l.bmp", r, r, r);
    printf("Job Finished!\n");
    // 關閉 bmp 圖檔
    close_bmp();
    return 0;
}

