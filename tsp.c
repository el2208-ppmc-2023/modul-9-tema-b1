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
        for (int j = 0; j < num_points; j++) {
                if (!visited[j]) {
                    double distance = haversine(points[current_point], points[j]);
                    if (nearest_neighbor == -1 || distance < nearest_neighbor_distance) {
                        nearest_neighbor = j;
                        nearest_neighbor_distance = distance;
                    }
                }
            }
        current_point = nearest_neighbor;
        visited[current_point] = 1;
        path[i] = current_point;
    }
}

void print_path(Point *points, int *visited, int *path, int num_points){
    printf("\nRute Pelayaran Optimal:\n");
    double total_distance = 0;
    for (int i = 0; i < num_points; i++) {
        printf("%s -> ", points[path[i]].address);
        if (i < num_points-1) {
            total_distance += haversine(points[path[i]], points[path[i+1]]);
            if(haversine(points[path[i]], points[path[i+1]]) > 2500)
            {
                visited[i] = 0;
            }
        }
        else{
            printf("%s", points[0]);
            total_distance += haversine(points[path[i]], points[0]);
            printf("\n\nJarak Total Rute Pelayaran: %.2lf\n", total_distance);

            printf("\nPelabuhan yang tidak terjangkau \n");
            int j = 1;
            for (int i = 0; i < num_points; i++)
            {
                if(visited[i] == 0)
                {
                    printf("%d. %s\n", j, points[i]);
                }
            }
            if(haversine(points[path[i]], points[0]) > 2500)
            {
                printf("%d. %s\n", j, points[i]);
            }
            
        }
    }
}

void neirest_neighbour_tsp(Point *points, int num_points) {
    // Array penyimpan status pelabuhan
    int *visited = (int*)malloc(num_points*sizeof(int));
    for (int i = 0; i < num_points; i++) {
        visited[i] = 0;
    }

    // Inisiasi titik awal
    int current_point = 0;
    visited[current_point] = 1;
    
    // Array penyimpan data jalur yang dilalui
    int *path = (int*)malloc(num_points*sizeof(int));
    path[0] = current_point;
    
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
        // Skip header
        if (strstr(line, "Pelabuhan") != NULL) {
            continue;
        }
        // Parsing setiap baris
        char *token = strtok(line, ",");
        Point point;
        strcpy(point.address, token);
        token = strtok(NULL, ",");
        point.x = atof(token);
        token = strtok(NULL, ",");
        point.y = atof(token);
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