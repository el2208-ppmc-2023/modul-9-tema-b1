/*EL2208 Praktikum Pemecahan Masalah dengan C 2022/2023
*Modul              : 9 - Tugas Besar
*Kelompok           : B1
*Hari dan Tanggal   : Rabu, 26/04/2023
*Asisten (NIM)      : Muhammmad Daffa Rasyid (13220059)
*Nama File          : Traveling Salesman Problem
*Deskripsi          : Program menyelesaikan permasalahan Traveling Salesman Problem dengan persyaratan jarak yang ditempuh kurang dari 2500 KM
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LENGTH 100 // Maksimal panjang
#define RADIUS 6371.0 // Jari jari bumi

typedef struct {
    char address[MAX_LENGTH];
    double x;
    double y;
} Point;

double to_radians(double degrees) {
    return degrees * 3.1415926 / 180.0;
}

double haversine(Point pos1, Point pos2) {
    double lat_dist = to_radians(pos2.x - pos1.x);
    double long_dist = to_radians(pos2.y - pos1.y);
    double a = sin(lat_dist / 2) * sin(lat_dist / 2) + cos(to_radians(pos1.x)) * cos(to_radians(pos2.x)) * sin(long_dist / 2) * sin(long_dist / 2);
    double r = 2 * RADIUS * atan2(sqrt(a), sqrt(1 - a));
    return r;
}

void define_path(Point *points, int num_points, int* path, int* visited){
    int current_point = 0;

    for (int i = 1; i < num_points; i++) {
        int nearest_neighbor = -1;
        double nearest_neighbor_distance = -1;

        // Menentukan jarak terdekat
        for (int j = 0; j < num_points; j++) {
                if (!visited[j]){
                    double distance = haversine(points[current_point], points[j]);
                    if (nearest_neighbor == -1 || distance < nearest_neighbor_distance){
                        nearest_neighbor = j;
                        nearest_neighbor_distance = distance;
                    }
                }
            }
        
        // Menyeleksi jarak agar kurang dari 2500 km
        if(nearest_neighbor_distance < 2500){
            current_point = nearest_neighbor;
            visited[current_point] = 1;
            path[i] = current_point;
        }
    }

    // Menyeleksi jarak agar kembali ke titik awal kurang dari 2500 km
    for (int i = num_points-1; i >= 0; i--) {
        if (path[i] != 0)
        {
            double distance = haversine(points[i], points[0]);
            if(distance < 2500)
            {
                break;
            }
            else
            {
                path[i] = 0;
            }
        }
    }
}

void print_path(Point *points, int *visited, int *path, int num_points){
    printf("\nRute Pelayaran Optimal:\n");
    double total_distance = 0;
    for (int i = 0; i < num_points; i++) {
        if (path[i] != 0 || i == 0)
        {
            printf("%s -> ", points[path[i]].address);
            total_distance += haversine(points[path[i]], points[path[i+1]]);
        }
        else {
            printf("%s\n\n", points[path[0]].address);
            printf("Jarak total rute pelayaran : %.2lf km \n\n", total_distance);
            printf("Pelabuhan yang tidak terjangkau : \n");
            for (int j = 0; j < num_points; j++) {
                if (!visited[j]){
                    printf("%s\n", points[j]);
                }
            }
            break;
        }
    }
}

void neirest_neighbour_tsp(Point *points, int num_points) {
    // Deklarasi array status tempat dan jalur
    int *visited = (int*)calloc(num_points, sizeof(int));
    int *path = (int*)calloc(num_points, sizeof(int));
    
    // Inisialisasi titik sekarang
    visited[0] = 1;
    path[0] = 0;
    
    // Fungsi pencari jalan
    define_path(points, num_points, path, visited);
    
    // Fungsi print jalur, total jarak, dan pelabuhan yang tidak bisa dikunjungo
    print_path(points, visited, path, num_points);

    free(visited);
    free(path);
}

int main() {
    // Inisialisasi
    FILE *file;
    Point *points = NULL;
    char line[MAX_LENGTH], namafile[MAX_LENGTH];
    int num_points = 0;
    
    // Input FILE
    printf("Masukkan file pelabuhan: ");
    scanf("%s", namafile);
    
    // Membuka FILE
    file = fopen(namafile, "r");
    if (file == NULL) {
        printf("Cannot open file.\n");
        return 1;
    }

    // Membaca FILE
    while (fgets(line, MAX_LENGTH, file) != NULL) {
        if (strstr(line, "Pelabuhan") != NULL) {
            continue;
        }

        char *token = strtok(line, ",");
        Point point;

        strcpy(point.address, token);
        token = strtok(NULL, ","); point.x = atof(token);
        token = strtok(NULL, ","); point.y = atof(token);
        points = (Point*)realloc(points, (num_points+1)*sizeof(Point));
        points[num_points] = point;
        num_points++;
    }
    fclose(file);

    // Algoritma TSP
    neirest_neighbour_tsp(points, num_points);

    free(points);
    return 0;
}
